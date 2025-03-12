// HFSM2 example implementation for a character AI with decision and body state machines.
// Improved for better extensibility and maintainability, allowing multiple characters to use this architecture.
// Integrated into the game framework to work with CharacterManager, GameTimeManager, SceneManager, etc.

#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm2/machine.hpp>
#include <iostream>
#include <string>
#include "CharacterManager.h"  // Include character manager for integration
#include "GameTimeManager.h"    // Include game time manager
#include "SceneManager.h"       // Include scene manager for scene interactions

//------------------------------------------------------------------------------

// Context shared across the state machines
struct Context {
	bool stomachPain = false;
	bool isHungry = true;
	std::string characterName;

	Context(const std::string& name) : characterName(name) {}
};

// Convenience typedef
using M = hfsm2::Machine<Context>;

#define S(s) struct s

//------------------------------------------------------------------------------
// State Machine Structure

// Decision-making state machine
using DecisionFSM = M::Orthogonal<
	S(Decision),
	S(Body)
>;

// Decision state structure
using Decision = M::Composite<S(DecisionRoot),
	S(Rest),
	S(Work),
	S(Entertainment)
>;

using Rest = M::Orthogonal<S(RestRoot),
	S(Sleep),
	S(Eat)
>;

using Entertainment = M::Composite<S(EntertainmentRoot),
	S(Play),
	S(Socialize)
>;

// Body part structure
using Body = M::Orthogonal<S(BodyRoot),
	S(Mouth),
	S(Hands),
	S(Stomach)
>;

// Body Sub-states
using Mouth = M::Composite<S(MouthRoot),
	S(Talking),
	S(Eating)
>;

using Hands = M::Composite<S(HandsRoot),
	S(Hugging),
	S(Operating)
>;

using Stomach = M::Composite<S(StomachRoot),
	S(Normal),
	S(Pain)
>;

#undef S

//------------------------------------------------------------------------------
// State Implementations

struct DecisionRoot : DecisionFSM::StateT<DecisionRoot> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - DecisionRoot: Enter" << std::endl;
	}
};

struct RestRoot : Decision::StateT<RestRoot> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Rest: Enter" << std::endl;
	}
};

struct Sleep : Rest::StateT<Sleep> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Sleep: Enter" << std::endl;
		GameTimeManager::instance()->set_time_scale(0.5f); // Example: slow down time during sleep
	}

	void exit(Control& control) {
		std::cout << control.context().characterName << " - Sleep: Exit" << std::endl;
		GameTimeManager::instance()->set_time_scale(1.0f); // Reset time scale after sleep
	}
};

struct Eat : Rest::StateT<Eat> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Eat: Enter" << std::endl;
		control.context().isHungry = false;
	}
};

struct Work : Decision::StateT<Work> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Work: Enter" << std::endl;
	}
};

struct EntertainmentRoot : Decision::StateT<EntertainmentRoot> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Entertainment: Enter" << std::endl;
	}
};

struct Play : Entertainment::StateT<Play> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Play: Enter" << std::endl;
	}
};

struct Socialize : Entertainment::StateT<Socialize> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Socialize: Enter" << std::endl;
	}
};

//------------------------------------------------------------------------------

struct BodyRoot : DecisionFSM::StateT<BodyRoot> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Body: Enter" << std::endl;
	}
};

struct MouthRoot : Body::StateT<MouthRoot> {};

struct Talking : Mouth::StateT<Talking> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Mouth: Talking" << std::endl;
	}
};

struct Eating : Mouth::StateT<Eating> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Mouth: Eating" << std::endl;
		if (control.context().isHungry) {
			control.changeTo<Decision::Eat>();
		}
	}
};

struct HandsRoot : Body::StateT<HandsRoot> {};

struct Hugging : Hands::StateT<Hugging> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Hands: Hugging" << std::endl;
	}
};

struct Operating : Hands::StateT<Operating> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Hands: Operating" << std::endl;
	}
};

struct StomachRoot : Body::StateT<StomachRoot> {};

struct Normal : Stomach::StateT<Normal> {
	void update(FullControl& control) {
		if (control.context().stomachPain) {
			control.changeTo<Pain>();
		}
	}
};

struct Pain : Stomach::StateT<Pain> {
	void enter(Control& control) {
		std::cout << control.context().characterName << " - Stomach: Pain - Need Rest" << std::endl;
		control.changeTo<Rest>();
	}
};

//------------------------------------------------------------------------------
// Integration with Character Manager and Scene Manager

class GameCharacter : public Character {
public:
	GameCharacter(int id, const std::string& name, const Position& start_position)
		: Character(id, name, start_position), context(name), stateMachine(context) {}

	void update() {
		stateMachine.update();
	}

private:
	Context context;
	DecisionFSM::Instance stateMachine;
};
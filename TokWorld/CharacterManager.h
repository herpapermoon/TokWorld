#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "MapManager.h"
#include "GameTimeManager.h"
class Character {
public:
	Character(int id, const std::string& name, const Position& start_position)
		: id(id), name(name), current_position(start_position) {}

    virtual ~Character() = default;

    int get_id() const { return id; }
    std::string get_name() const { return name; }

	void move_within_map(float new_x, float new_y) {
		current_position.x = new_x;
		current_position.y = new_y;
		std::cout << name << " 移动到了地图 " << current_position.map_id
			<< " 的位置 (" << current_position.x << ", " << current_position.y << ")" << std::endl;
	}

	// 切换地图
	void travel_to_map(int new_map_id) {
		if (new_map_id == current_position.map_id) {
			std::cout << "已经在目标地图，不需要移动" << std::endl;
			return;
		}

		float travel_time = MapManager::instance()->get_map_to_map_distance(current_position.map_id, new_map_id);
		if (travel_time > 0) {
			std::cout << name << " 正在从地图 " << current_position.map_id << " 移动到地图 " << new_map_id
				<< "，预计时间: " << travel_time << " 小时" << std::endl;
			GameTimeManager::instance()->update(travel_time);  // 更新游戏内时间
			current_position.map_id = new_map_id;
			current_position.x = 5;  // 假设移动到新地图后初始位置为 (5, 5)
			current_position.y = 5;
		}
	}

	// 获取角色的当前位置
	Position get_position() const {
		return current_position;
	}

protected:
	int id;
	std::string name;
	Position current_position;  // 包含地图编号和地图内坐标
    
};

 

                                                                                                                                                                                                                                                                                                                               class CharacterManager {
public:
    static CharacterManager* instance() {
        if (!manager) {
            manager = new CharacterManager();
        }
        return manager;
    }

    // 禁止拷贝和赋值操作
    CharacterManager(const CharacterManager&) = delete;
    CharacterManager& operator=(const CharacterManager&) = delete;

    //int create_character(const std::string& name) {
    //    int new_id = next_id++;
    //    auto character = std::make_shared<Character>(new_id, name);
    //    characters[new_id] = character;
    //    return new_id;
    //}

    std::shared_ptr<Character> get_character(int id) {
        if (characters.find(id) != characters.end()) {
            return characters[id];
        }
        return nullptr;
    }

    void delete_character(int id) {
        if (characters.find(id) != characters.end()) {
            characters.erase(id);
        }
    }

    void update_characters() {
    }

private:
    CharacterManager() = default;
    ~CharacterManager() = default;

    static CharacterManager* manager;
    int next_id = 0;
    std::unordered_map<int, std::shared_ptr<Character>> characters;
};
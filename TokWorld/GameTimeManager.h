// GameTimeManager.h
#pragma once
#include <chrono>
#include <imgui.h>
#include <iostream>
#include <iomanip>
#include <sstream>

class GameTimeManager
{
public:
	static GameTimeManager* instance();

	// 每帧调用，传入真实时间间隔以更新游戏内的时间
	void update(float real_delta_time);

	// 开始和停止游戏时间
	void start();
	void stop();

	// 设置时间流速，例如 2.0 表示加速两倍，0.5 表示减慢为一半
	void set_time_scale(float scale);

	// 获取当前的游戏时间流速
	float get_time_scale() const;

	// 获取当前的游戏时间和日期
	std::string get_current_date_time() const;

private:
	GameTimeManager();
	~GameTimeManager();

	static GameTimeManager* manager;

	float time_scale = 10.0f;        // 初始时间流速为 1.0 表示正常速度
	float delta_time = 0.0f;        // 当前帧的游戏时间间隔
	float total_game_time = 0.0f;   // 累计的总游戏时间（秒）

	// 游戏时间控制
	bool is_running = false;        // 是否累积游戏时间

	// 游戏时间相关变量
	int current_year = 1;
	int current_month = 1;
	int current_day = 1;
	int current_hour = 8;  // 初始时间为早上8点
	int current_minute = 0;

	// 游戏时间的常量设置
	const int minutes_in_hour = 60;
	const int hours_in_day = 24;
	const int days_in_month = 30;  // 假设每月有30天
	const int months_in_year = 12; // 假设每年有12个月
};

GameTimeManager* GameTimeManager::instance()
{
	if (!manager) {
		manager = new GameTimeManager();
	}
	return manager;
}

GameTimeManager::GameTimeManager() {}

GameTimeManager::~GameTimeManager() {}

void GameTimeManager::update(float real_delta_time)
{
	// 只有在时间管理器正在运行时才更新时间
	if (!is_running) {
		return;
	}

	// 根据时间缩放比例计算游戏内时间的增长
	delta_time = real_delta_time * time_scale;
	total_game_time += delta_time;

	// 累积游戏时间到分钟、小时、天、月、年
	float seconds_per_minute = 60.0f;
	while (total_game_time >= seconds_per_minute) {
		total_game_time -= seconds_per_minute;
		current_minute++;

		if (current_minute >= minutes_in_hour) {
			current_minute = 0;
			current_hour++;

			if (current_hour >= hours_in_day) {
				current_hour = 0;
				current_day++;

				if (current_day > days_in_month) {
					current_day = 1;
					current_month++;

					if (current_month > months_in_year) {
						current_month = 1;
						current_year++;
					}
				}
			}
		}
	}
}

void GameTimeManager::start()
{
	is_running = true;
}

void GameTimeManager::stop()
{
	is_running = false;
}

std::string GameTimeManager::get_current_date_time() const
{
	std::ostringstream oss;
	oss << "Year " << current_year
		<< ", Month " << current_month
		<< ", Day " << current_day
		<< ", " << std::setfill('0') << std::setw(2) << current_hour
		<< ":" << std::setfill('0') << std::setw(2) << current_minute;
	return oss.str();
}

void GameTimeManager::set_time_scale(float scale)
{
	time_scale = scale;
}

float GameTimeManager::get_time_scale() const
{
	return time_scale;
}

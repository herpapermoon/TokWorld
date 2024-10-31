#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

using json = nlohmann::json;

struct Position {
	int map_id;     // 地图编号
	float x;        // X 坐标
	float y;        // Y 坐标
};

struct MapInfo {
	std::string name;           // 地图名称
	float distance_to_hub;      // 到中心节点的距离
	json tile_data;             // 地图的tile数据
	int width;
	int height;
};

class MapManager {
public:
	static MapManager* instance() {
		if (!manager) {
			manager = new MapManager();
		}
		return manager;
	}

	// 加载地图
	void load_map(int map_id, const std::string& name, const std::string& file_path, float distance_to_hub) {
		std::ifstream map_file(file_path);
		if (!map_file.is_open()) {
			throw std::runtime_error("无法打开地图文件");
		}

		json map_data;
		map_file >> map_data;

		int width = map_data["width"];
		int height = map_data["height"];

		maps[map_id] = MapInfo{ name, distance_to_hub, map_data, width, height };
	}

	// 绘制地图
	void draw_map(int map_id) {
		if (maps.find(map_id) == maps.end()) {
			std::cerr << "地图未找到，无法绘制" << std::endl;
			return;
		}

		auto& map = maps[map_id];
		auto layers = map.tile_data["layers"];
		auto tile_layer = layers[0];  // 假设我们处理第一个图层
		auto data = tile_layer["data"];

		// 绘制地图 (字符画形式)
		for (int y = 0; y < map.height; ++y) {
			for (int x = 0; x < map.width; ++x) {
				int tile_id = data[y * map.width + x];
				char tile_char = tile_id == 0 ? ' ' : '#';  // 示例：ID为0表示空地，其他为墙壁
				std::cout << tile_char;
			}
			std::cout << std::endl;
		}
	}

	// 计算两个位置之间的距离（考虑不同地图间的传送）
	float calculate_total_distance(const Position& pos1, const Position& pos2) {
		if (pos1.map_id == pos2.map_id) {
			return calculate_distance(pos1, pos2);  // 在同一地图内直接计算
		}
		else {
			float inter_map_distance = get_map_to_map_distance(pos1.map_id, pos2.map_id);
			if (inter_map_distance < 0) {
				return -1; // 出错，地图间不可达
			}
			return inter_map_distance; // 如果需要，可以考虑加上地图内距离
		}
	}

	// 获取地图间通过中心节点的距离
	float get_map_to_map_distance(int map_id_1, int map_id_2) {
		if (maps.find(map_id_1) == maps.end() || maps.find(map_id_2) == maps.end()) {
			std::cerr << "地图未找到" << std::endl;
			return -1;
		}

		float distance_1 = maps[map_id_1].distance_to_hub;
		float distance_2 = maps[map_id_2].distance_to_hub;
		return distance_1 + distance_2;  // 从地图1通过中心节点到地图2的距离
	}

private:
	MapManager() = default;
	~MapManager() = default;

	static MapManager* manager;
	std::unordered_map<int, MapInfo> maps;  // 存储地图编号和对应的信息

	// 计算同一地图内两个位置的欧氏距离
	float calculate_distance(const Position& pos1, const Position& pos2) {
		return std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
	}
};



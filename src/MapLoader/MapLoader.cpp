#include "MapLoader.h"
#include <algorithm>

std::vector<std::vector<int>> MapLoader::LoadMap(const std::string& filePath) {
    std::vector<std::vector<int>> map;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "无法打开地图文件: " << filePath << std::endl;
        return map;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        
        // 按逗号分割每行
        while (std::getline(ss, cell, ',')) {
            try {
                row.push_back(std::stoi(cell));
            } catch (const std::exception& e) {
                std::cerr << "解析瓦片ID失败: " << cell << std::endl;
                row.push_back(0); // 默认值
            }
        }
        
        if (!row.empty()) {
            map.push_back(row);
        }
    }
    
    file.close();
    return map;
}

std::pair<int, int> MapLoader::GetMapSize(const std::string& filePath) {
    auto map = LoadMap(filePath);
    if (map.empty()) {
        return {0, 0};
    }
    
    int height = map.size();
    int width = map[0].size();
    
    // 验证所有行的宽度是否一致
    for (const auto& row : map) {
        if (row.size() != width) {
            std::cerr << "地图格式错误：行宽度不一致" << std::endl;
            return {0, 0};
        }
    }
    
    return {width, height};
}

bool MapLoader::ValidateMap(const std::string& filePath) {
    auto map = LoadMap(filePath);
    if (map.empty()) {
        return false;
    }
    
    int expectedWidth = map[0].size();
    if (expectedWidth == 0) {
        return false;
    }
    
    // 检查所有行是否有相同的宽度
    for (const auto& row : map) {
        if (row.size() != expectedWidth) {
            return false;
        }
    }
    
    return true;
} 
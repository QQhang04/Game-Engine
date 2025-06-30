#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class MapLoader {
public:
    // 读取地图文件，返回二维向量
    static std::vector<std::vector<int>> LoadMap(const std::string& filePath);
    
    // 获取地图尺寸
    static std::pair<int, int> GetMapSize(const std::string& filePath);
    
    // 验证地图文件格式
    static bool ValidateMap(const std::string& filePath);
};

#endif 
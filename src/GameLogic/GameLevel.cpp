/*******************************************************************
** This code has small modifications from Breakout by Joey de Vries:
*** Logs problems opening level files
******************************************************************/

#include "GameLevel.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "../ResourceManager.h"

void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight) {
    //clear old data
    this->Bricks.clear();

    // load from file
    unsigned int tileCode;
    std::ifstream fstream(file);

    if (!fstream) {
        std::cerr << "Error opening level file " << file << std::endl;
        return;
    }

    std::string line;
    std::vector<std::vector<unsigned int>> tileData;

    while (std::getline(fstream, line)) {
        std::istringstream sstream(line);
        std::vector<unsigned int> row;
        while (sstream >> tileCode) {
            row.push_back(tileCode);
        }
        tileData.push_back(row);
    }

    if (tileData.size() <= 0) {
        std::cout << "Level seems to be empty: " << file << std::endl;
        return;
    }

    this->init(tileData, levelWidth, levelHeight);
}

void GameLevel::Draw(SpriteRenderer &renderer) {
    for (GameObject2D &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted() {
    for (GameObject2D &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width  = tileData[0].size();
    float unit_width    = levelWidth / static_cast<float>(width);
    float unit_height   = levelHeight / height;
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; x ++) {

            if (tileData[y][x] == 1) // SOLID BLOCK
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject2D obj(pos, size,
                    ResourceManager::GetTexture(TextureName::BlockSolid),
                    glm::vec3(0.8f, 0.8f, 0.7f)
                );
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }

            else if (tileData[y][x] > 1) // OTHER BLOCK, sharing texture, destroyable
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)    // Blue
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)     // Green
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4) // Brown?
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5) // Red
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(GameObject2D(pos, size, ResourceManager::GetTexture(TextureName::Block), color));
            }
        }

    }
}

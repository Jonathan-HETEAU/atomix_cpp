#pragma once
#include "atomix.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

const std::string ASSETS_PATH_NIVEAUX = std::string(ASSETS_PATH) + "niveaux/";


namespace Atomix {

using json = nlohmann::json;

inline LevelData loadLevel(const int niv) {
    auto jsonText = LoadFileText((ASSETS_PATH_NIVEAUX+TextFormat("level_%d.json",niv)).c_str());
    json data = json::parse(jsonText);
    
    LevelData level;
    level.width = data["width"];
    level.height = data["height"];
    level.nbrAtoms = data["nbrAtoms"];
    
    // Lire la map
    level.map = new bool*[level.height];
    for (int y = 0; y < level.height; y++) {
        level.map[y] = new bool[level.width];
        for (int x = 0; x < level.width; x++) {
            level.map[y][x] = data["map"][y][x];
        }
    }
    
    // Lire les atomes
    level.atoms = new AtomData[level.nbrAtoms];
    for (int i = 0; i < level.nbrAtoms; i++) {
        level.atoms[i].value = data["atoms"][i]["value"];
        level.atoms[i].position.x = data["atoms"][i]["x"];
        level.atoms[i].position.y = data["atoms"][i]["y"];
    }
    
    // Lire la solution
    level.solution = new AtomData[level.nbrAtoms];
    for (int i = 0; i < level.nbrAtoms; i++) {
        level.solution[i].value = data["solution"][i]["value"];
        level.solution[i].position.x = data["solution"][i]["x"];
        level.solution[i].position.y = data["solution"][i]["y"];
    }
    return level;
}

inline void freeLevelData(LevelData& level) {
    for (int y = 0; y < level.height; y++) {
        delete[] level.map[y];
    }
    delete[] level.map;
    delete[] level.atoms;
    delete[] level.solution;
}

} // namespace Atomix
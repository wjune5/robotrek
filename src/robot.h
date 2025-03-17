#ifndef __ROBOT_AGENT__H
#define __ROBOT_AGENT__H 

#include <vector>
#include <set>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "enviro.h"
#include "puzzle.h"

using namespace enviro;
using namespace std;

class RobotController : public Process, public AgentInterface {

    public:
    RobotController() : Process(), AgentInterface(), puzzle_info() {}

    void init();
    void start() {}
    void update(){}
    void stop() {}
  
    const double CELL_SIZE = 50.0;
    Puzzle puzzle_info;

    Position robot_pos;
    char robot_name;
    char parts_name = 'r';
    vector<int> parts_ids; // id, position 
    vector<int> wall_ids; // id, position 
    vector<int> block_ids; // id, position 
    vector<int> goal_ids; // id, position 
    vector<Position> visited_positions; // all visited positions

    pair<int, int> transformToX(int row, int col);
    void remove_all_agents();

    bool load_map(const std::string& level_id);
    void parse_map(const json& level);
    void place_agent(char type, int row, int col);
    void place_wall(bool path, bool horizontal, int row, int col);
    bool can_move(const Position& p1, const Position& p2);

};

class Robot : public Agent {
    public:
    Robot(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    RobotController c;
};

DECLARE_INTERFACE(Robot)

#endif
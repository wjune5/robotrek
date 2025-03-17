#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <thread>

#include "robot.h"

using namespace enviro;

void RobotController::init() {

    robot_name = 'R';
    parts_name = tolower(robot_name);

    watch("keydown", [&](Event &e) {
        auto k = e.value()["key"].get<std::string>();
        if ( k == "w" ) {
            if (can_move(robot_pos, robot_pos-Position(1,0))) {
                // has visited
                visited_positions.push_back(robot_pos);
                // place path agent in robot's path
                place_wall(true, false, robot_pos.first, robot_pos.second);
                // change current robot's row
                robot_pos.first--;
                // convert (row, col) to (x, y)
                pair<int, int> xy = transformToX(robot_pos.first, robot_pos.second);
                // move to the next position
                teleport(xy.first, xy.second, -M_PI_2);
            }

        } else if ( k == "s" ) {
            if (can_move(robot_pos, robot_pos-Position(-1,0))) {
                visited_positions.push_back(robot_pos);

                place_wall(true, false, robot_pos.first, robot_pos.second);
                robot_pos.first++;
                pair<int, int> xy = transformToX(robot_pos.first, robot_pos.second);
                
                teleport(xy.first, xy.second, M_PI_2);
            }
        } else if ( k == "a" ) {
            if (can_move(robot_pos, robot_pos-Position(0,1))) {
                visited_positions.push_back(robot_pos);

                place_wall(true, true, robot_pos.first, robot_pos.second);
                robot_pos.second--;
                pair<int, int> xy = transformToX(robot_pos.first, robot_pos.second);
                teleport(xy.first, xy.second, M_PI);
            }
        } else if ( k == "d" ) {
            if (can_move(robot_pos, robot_pos-Position(0, -1))) {
                visited_positions.push_back(robot_pos);

                place_wall(true, true, robot_pos.first, robot_pos.second);
                robot_pos.second++;
                pair<int, int> xy = transformToX(robot_pos.first, robot_pos.second);
                teleport(xy.first, xy.second, 0);
            }
        }
    });
   
    watch("button_click", [&](Event& e) {
        if ( e.value()["value"] == "start" ) {
            // start the game
            puzzle_info.level = 1;
            load_map(std::to_string(puzzle_info.level));
        } else if ( e.value()["value"] == "load" ) {
            // go to the next level
            if (puzzle_info.level < 10) {
                puzzle_info.level++;
            }
            load_map(std::to_string(puzzle_info.level));
        } else if ( e.value()["value"] == "loadn" ) {
            // go to the previous level
            if (puzzle_info.level > 0) {
                puzzle_info.level--;
            }

            load_map(std::to_string(puzzle_info.level));
        } else if ( e.value()["value"] == "reset" ) {
            // reload the map
            load_map(std::to_string(puzzle_info.level));
        }
    }); 

    notice_collisions_with("Parts", [&](Event &e) {
        int parts_id = e.value()["id"];
        Agent &parts = find_agent(parts_id);
        // remove the agent when the robot collides a machine part
        remove_agent(parts_id);
        auto it = std::find(parts_ids.begin(), parts_ids.end(), parts_id);
        if (it != parts_ids.end()) {
            parts_ids.erase(it);
        }
    });
    notice_collisions_with("Goal", [&](Event &e) {
        int goal_id = e.value()["id"];
        // if collected all parts
        if (parts_ids.size() == 0) {
            remove_agent(goal_id);
            find_agent(id()).label("Win!", -5, -15);
        }
    });
}

pair<int, int> RobotController::transformToX(int row, int col) {
    int x = col * CELL_SIZE+CELL_SIZE/2;
    int y = row * CELL_SIZE+CELL_SIZE/2;
    return make_pair(x, y);
}

bool RobotController::load_map(const std::string &level_id) {
    json maps = json_helper::read("puzzle.json");
    for (const auto& level: maps["levels"]) {
        if (level["id"].get<string>() == level_id) {
            // remove all agents
            remove_all_agents();
            parse_map(level);
            return true;
        }
    }
    return false;
}

void RobotController::remove_all_agents() {
    find_agent(id()).label("", -5, -15);

    puzzle_info.puzzle_layout = "";
    visited_positions.clear();    

    if (wall_ids.size() > 0) {
        // Remove existing walls
        for (int id : wall_ids) {
            if (agent_exists(id)) {
                remove_agent(id);
            }
        }
        wall_ids.clear();
    }
    if (parts_ids.size() > 0) {
        // Remove existing walls
        for (int id : parts_ids) {
            if (agent_exists(id)) {
                remove_agent(id);
            }
        }
        parts_ids.clear();
    }
    if (block_ids.size() > 0) {
        // Remove existing blocks
        for (int id : block_ids) {
            if (agent_exists(id)) {
                remove_agent(id);
            }
        }
        block_ids.clear();
    }
    if (goal_ids.size() > 0) {
        // Remove existing goal
        for (int id : goal_ids) {
            if (agent_exists(id)) {
                remove_agent(id);
            }
        }
        goal_ids.clear();
    }
}
void RobotController::parse_map(const json& level) {
    const auto& layout = level["layout"];
    int rows = layout.size();
    // calculate the size of the puzzle according to the layout in the json file
    puzzle_info.puzzle_size = Position(rows / 2, layout[0].get<string>().size() / 2);
    pair<int, int> xy = transformToX(puzzle_info.puzzle_size.first/2, puzzle_info.puzzle_size.second/2);
    // center the screen
    center(xy.first,xy.second);

    // emit an event to make the board at the top center
    emit(Event("puzzle_ready", { { "xy", xy}}));

    // place agents according to the map
    for (int r = 0; ; r++) {
        std::string row_str = layout[r*2].get<string>();
        // 1. get the wall sign in the layout, and place agents
        for (int c = 0; c < puzzle_info.puzzle_size.second; ++c)
            if (row_str[c * 2 + 1] == '-') {
                place_wall(false, true, r , c);
            }
        if (r == puzzle_info.puzzle_size.first) break;
        std::string str_v = layout[r * 2 + 1].get<string>();
        // 2. place the vertical walls, blocks, a goal, the robot and machine parts
        for (int c = 0; ; c++) {
            if (str_v[c * 2] == '|')
                place_wall(false, false, r , c);
            if (c == puzzle_info.puzzle_size.second) break;
            char ch = str_v[c * 2 + 1];
            puzzle_info.puzzle_layout.push_back(ch);
            if (ch == 'R' || ch == 'r' || ch == 'b' || ch == 'O') {
                place_agent(ch, r, c); 
            }           
        }
    } 
}
void RobotController::place_agent(char type, int row, int col) {
    pair<int, int> xy = transformToX(row, col);
    int x = xy.first;
    int y = xy.second;
    if (type == 'R') {
        // move current robot agent
        teleport(x, y, 0);
        puzzle_info.start = Position(row, col);
        robot_pos.first = row;
        robot_pos.second = col;
    } else if (type == 'r') {
        // parts agent
        Agent& parts = add_agent("Parts", x, y, 0, {{"fill", "orange"}});
        parts_ids.push_back(parts.get_id());
    } else if (type == 'b') {
        // block agent
        Agent& block = add_agent("Block", x, y, 0, {{"fill", "grey"}});
        block.decorate("<line x1='-20' y1='-20' x2='20' y2='20' style='stroke:black;'></line><line x1='20' y1='-20' x2='-20' y2='20' style='stroke:black;'></line>");
        block_ids.push_back(block.get_id());
    } else if (type == 'O') {
        // goal agent        puzzle_info.end = Position(row, col);
        Agent& goal = add_agent("Goal", x, y, 0, {{"fill", "green"}});
        goal_ids.push_back(goal.get_id());
    }
}
/**
 * @param path visited position
 * @param horizontal visited position
 */
void RobotController::place_wall(bool path, bool horizontal, int row, int col) {
    // path and wall agents have different calculations. Path should be placed at the center of each cell
    int x = path?(col * CELL_SIZE+CELL_SIZE/2) : col * CELL_SIZE;
    int y = path?(row * CELL_SIZE+CELL_SIZE/2)  : row * CELL_SIZE;
    // vertical and horizontal walls
    Agent& agent =  path? add_agent("Path", x, y, horizontal? 0:M_PI/2, {{"fill", "grey"}, {"stroke", "0"}}): add_agent("Wall", x, y, horizontal? 0:M_PI/2, {{"fill", "black"}});
    puzzle_info.walls_pos.insert(Position(row, col));
    wall_ids.push_back(agent.get_id());
}

bool RobotController::can_move(const Position& p1, const Position& target) {
    // Check if the position is valid and not blocked by walls
    if (target.first< 0 || target.second < 0 || target.first > puzzle_info.puzzle_size.first-1 || target.second > puzzle_info.puzzle_size.second-1) {
        return false;
    }
    char &chp1 = puzzle_info.cells(p1), &chp2 = puzzle_info.cells(target);
    auto it = std::find(visited_positions.begin(), visited_positions.end(), target);
    // check if the position has been visited 
    if (it != visited_positions.end()) {
        return false;
    }
    if (chp2 == PUZ_GOAL) {
        if (parts_ids.size() != 0) {
            // haven't collected all parts
            return false;
        }
        // reach goal
        return true;
    } else if (chp1 == 'r' && chp2 != 'r') {
        // parts should be collected continuously
        return false;
    } else if (chp2 == '|' || chp2 == '-') {
        // walls
        return false;
    } else if (chp2 == 'b') {
        // blocks
        return false;
    }
    return true;
}

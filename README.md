# RoboTrek
<image src="doc/2025-03-16_183034.png"></image>

## Overview
RoboTrek is a grid-based puzzle game where the player controls a robot to collect all required machine parts while ensuring **no backtracking**.

## Features
- **User-controlled movement** – navigate the robot manually.  
- **No backtracking** – players must plan their moves carefully.  
- **Item collection** – gather all required parts(orange) before reaching the goal.  
- **Different levels** – modify the maze layout for different challenges.
### Challenges
- Straight-line movement

Since movement is based on `(x, y)`, split the grid into cells, using rows and columns to control the robot and check its neighbour cells' situation.
- No backtracking
  
Maintain a visited cell tracker( a `vector`), and check each move doesn't revisit.
- Load many user-created maps
  
Store different layouts in a JSON file, and use different signs to represent different agents. So I can use a JSON parser to read maps.

## How to play
`W` – Move up

`S` – Move down

`A` – Move left

`D` – Move right
<br>
When you win, you can click the next level to change the layout.

## Install
### Requirements
See <a href="https://github.com/klavinslab/enviro">here</a> to install enviro first.
### Build the project
1. Clone this repo
```sh
git clone https://github.com/wjune5/robotrek
```
2. Make the file
Go to the project directory, and enter `make`
3. Start the project
```sh
esm start
make
enviro
```
### Project structure
```
RoboTrek
-- defs
-- lib
-- src # core functions
-- puzzle.json # puzzle maps
-- config.json # enviro configuration
```
Defining Agents
===

Files
---
An agent, called `Robot`, consists of the following
- `defs/robot.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/robot.h`: A header file containing at the very least a class declaration of a class called `Robot` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Robot)` which allows enviro to use the class.
- `src/robot.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.robot.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Block`, consists of the following
- `defs/block.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/block.h`: A header file containing at the very least a class declaration of a class called `Block` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Block)` which allows enviro to use the class.
- `src/block.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.block.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Parts`, consists of the following
- `defs/parts.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/parts.h`: A header file containing at the very least a class declaration of a class called `Parts` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Parts)` which allows enviro to use the class.
- `src/parts.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.parts.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Path`, consists of the following
- `defs/path.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/path.h`: A header file containing at the very least a class declaration of a class called `Path` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Path)` which allows enviro to use the class.
- `src/path.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.path.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Wall`, consists of the following
- `defs/wall.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/wall.h`: A header file containing at the very least a class declaration of a class called `Wall` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Wall)` which allows enviro to use the class.
- `src/wall.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.wall.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Goal`, consists of the following
- `defs/goal.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/goal.h`: A header file containing at the very least a class declaration of a class called `Goal` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Goal)` which allows enviro to use the class.
- `src/goal.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.goal.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project. 

An agent, called `Board`, consists of the following
- `defs/board.json`: This file contains information about the agent's shape and physical properties such as its mass and friction coefficients.
- `src/board.h`: A header file containing at the very least a class declaration of a class called `Goal` inheriting from `enviro::Agent` and a call to the macro `DECLARE_INTERFACE(Board)` which allows enviro to use the class.
- `src/board.cc`: A source file with whatever code you want in it, usually the implementations of methods in the header file.
- `lib.board.so`: The shared object library for the agent. This is made when you run `make` from the main directory of your project.
- 
JSON Definition
---

The puzzle's maps are in the `puzzle.json`. It should contain an object with the following fields:

> `id`<br>
A string defining the id of the map.

> `layout`<br>
> A list of the strings. `-` and `|` represents walls, `r` represents parts, `R` represents a robot, and `O` is the goal.
> ```json
> "layout": [
>   " - - - - - ",
>   "|         |",
>   "           ",
>   "|O r r r r|",
>   "           ",
>   "|r     r r|",
>   "           ",
>   "|r r R r r|",
>   "           ",
>   "|r r     r|",
>   "           ",
>   "|r r r r r|",
>   "           ",
>   "|         |",
>   " - - - - - "
>]
> ```

The AgentInterface Class
===

Robot Agent
---

`const double CELL_SIZE = 50.0;`: the size of a grid in the puzzle.
`Puzzle puzzle_info;`: information of puzzle, such as id of level, width, height, start position, string format of layout
`Position robot_pos;`: current position of robot (row, col).
`vector<int> parts_ids;`: a list of parts agent id
`vector<int> wall_ids;`: a list of walls agent id
`vector<int> block_ids;`: a list of walls agent id
`vector<int> goal_ids;`: a list of goal agent id

`vector<Position> visited_positions;`: all visited positions by the robot

Functions
---
`pair<int, int> transformToX(int row, int col);`
This method transforms grid coordinates (row, col) into (x, y), for rendering
`void remove_all_agents();`
This method removes all agents to reload the map or go to the next level challenge.
`bool load_map(const std::string& level_id);`
This method loads a map based on the provided level_id. It reads map data from a file.
`void parse_map(const json& level);`
This method parses a map configuration provided in JSON format to set up the game. It calculates the puzzle's size, stringifies the map and calls `place_agent`.
`void place_agent(char type, int row, int col);`
This method places all the needed agents(robot, blocks, parts, path, goal) in the specific positions. The p
`void place_wall(bool path, bool horizontal, int row, int col);`
This method places all the walls in the specific positions. If the object is a path symbol, it won't use `transformToX` to get (x,y) because the path agent is circular.
`bool can_move(const Position& p1, const Position& p2);`
This method determines if movement from position p1 to position p2 is possible within the map's constraints. It checks factors such as map boundaries, presence of walls or obstacles, and possibly the movement capabilities of the agent.

## License
MIT License – Free to use and modify

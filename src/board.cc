#include <iostream>
#include <math.h>
#include "board.h"

using namespace enviro;

void BoardController::init() {
    // make the board decoration on the top center of the map
    decorate(R"(<g><rect x='-400' y='-100' width='800' height='120'
        style='stroke:black;
           fill: #fff;
               stroke-width:6px;
                stroke-opacity:0.25;
          '
       /><text y='-70'>
          <tspan x='-380' style='font-weight: bold;'>Game Rule</tspan>
          <tspan x='-360' dy='20'>1. <tspan style='font-weight: bold;'>No backtracking</tspan> The robot must not revisit any previously positions.</tspan>
          <tspan x='-360' dy='20'>2. <tspan style='font-weight: bold;'>Collect all parts before reaching the goal</tspan></tspan>
          <tspan x='-360' dy='20'>3. <tspan style='font-weight: bold;'>No backtracking</tspan> The path should be continuous, collecting all items in a single, unbroken traversal without any interruptions.</tspan>
      </text>
      </g>)");
    watch("puzzle_ready", [&](Event& e) {
        std::pair<int, int> xy = e.value()["xy"];
        
        board_x  = xy.first;
        board_y  = -100;
        // move board agent
        set_board();
    });
    
}
void BoardController::set_board() {
    teleport(board_x, board_y, 0);
}
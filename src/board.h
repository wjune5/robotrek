#ifndef __BOARD_AGENT__H
#define __BOARD_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BoardController : public Process, public AgentInterface {

    public:
    BoardController() : Process(), AgentInterface() {}

    void init();
    void start() {}
    void update() {}
    void stop() {}

    int board_x, board_y;
    void set_board();
};

class Board : public Agent {
    public:
    Board(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    BoardController c;
};

DECLARE_INTERFACE(Board)

#endif
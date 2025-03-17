#ifndef __GOAL_AGENT__H
#define __GOAL_AGENT__H 

#include "enviro.h"

using namespace enviro;

class GoalController : public Process, public AgentInterface {

    public:
    GoalController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Goal : public Agent {
    public:
    Goal(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    GoalController c;
};

DECLARE_INTERFACE(Goal)

#endif
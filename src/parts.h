#ifndef __PARTS_AGENT__H
#define __PARTS_AGENT__H 

#include "enviro.h"

using namespace enviro;

class PartsController : public Process, public AgentInterface {

    public:
    PartsController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Parts : public Agent {
    public:
    Parts(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    PartsController c;
};

DECLARE_INTERFACE(Parts)

#endif
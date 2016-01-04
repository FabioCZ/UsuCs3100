#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>
#include <memory>
#include <cstdio>
#include <algorithm>
#include "sim_event.hpp"

const int NUM_TASKS = 20;

class simulator_fifo {

public:
    simulator_fifo() {

        remove("simulator.log");

        sim_state::get_inst().currTime = 0;
        sim_state::get_inst().availCpu = sim_state::get_inst().numCpu;

    }

    void run_simulation() {

        for (int i = 0; i < NUM_TASKS; ++i) {
            std::shared_ptr<add_task> a;
            a = std::make_shared<add_task>(i * sim_state::get_inst().freqJobCreation);
            global::events.push(a);
        }
        std::shared_ptr<sim_event> done = std::make_shared<sim_done>(10000.0);
        global::events.push(done);

        while (!global::events.empty()) {
            std::shared_ptr<sim_event> a = global::events.top();
            global::events.pop();
            if (auto b = std::dynamic_pointer_cast<exec_task>(a)) {

                b->execute();
            }
            else if (auto c = std::dynamic_pointer_cast<add_task>(a)) {

                c->execute();
            }
            else if (auto d = std::dynamic_pointer_cast<cpu_done>(a)) {
                d->execute();
            }
            else if (auto e = std::dynamic_pointer_cast<io_done>(a)) {
                e->execute();
            }
            else if (auto f = std::dynamic_pointer_cast<sim_done>(a)) {
                f->execute();
            }
        }
        std::cout << std::endl << "done with simulation" << std::endl;
    }
};


#endif

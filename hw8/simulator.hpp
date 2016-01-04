//hw7, Keano Gottlicher, A01647928, CS3100 Sp2015
#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>
#include <memory>
#include <cstdio>
#include <algorithm>
#include "sim_event.hpp"

const int NUM_TASKS = 20;

class simulator {

public:
    simulator() {

        remove("simulator.log");

        sim_state::get_inst().currTime = 0;
        sim_state::get_inst().availCpu = sim_state::get_inst().numCpu;

    }

private:
    /**
    * 1 = fifo, 2 = round robin
    */
    void run_simulation_base(int type) {

        global::memory.clear();
        for(int i =0; i < sim_state::get_inst().pageSize; i++){
            global::memory.push_back(-1);
        }

        while (!global::events.empty()) {
            global::events.pop();
        }

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
                if (type == 1)b->execute_fifo();
                else if (type == 2)b->execute_round_robin();
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

public:
    void run_simulation_fifo() {
        run_simulation_base(1);
    }

    /**
    * Shortest job first.
    * When it's time to start a new task, we look all the tasks and choose the shortest one.
    */
    void run_simulation_sjf() {

        global::memory.clear();
        for(int i =0; i < sim_state::get_inst().pageSize; i++){
            global::memory.push_back(-1);
        }

        while (!global::events.empty()) {
            global::events.pop();
        }

        for (int i = 0; i < NUM_TASKS; ++i) {
            std::shared_ptr<add_task> a;
            a = std::make_shared<add_task>(i * sim_state::get_inst().freqJobCreation);
            global::events.push(a);
        }
        std::shared_ptr<sim_event> done = std::make_shared<sim_done>(10000.0);
        global::events.push(done);

        while (!global::events.empty()) {

            std::shared_ptr<sim_event> a = global::events.top(); //get shortest thing here

            if (auto b = std::dynamic_pointer_cast<exec_task>(a)) {
                if (b->curr_burst == 0) {
                    int min_dur_index = -1;
                    double min_dur = INT32_MAX;
                    std::vector<std::shared_ptr<sim_event> > events_vec_copy;
                    while (!global::events.empty()) {
                        events_vec_copy.push_back(global::events.top());
                        global::events.pop();
                    }

                    for (int i = 0; i < events_vec_copy.size(); i++) {

                        if (auto curr_event_cpy = std::dynamic_pointer_cast<exec_task>(events_vec_copy[i])) {
                            double curr_dur = 0;
                            for (int j = 0; j < curr_event_cpy->bursts.size(); j++) {
                                curr_dur += curr_event_cpy->bursts[j]->duration;
                            }
                            if (curr_dur < min_dur) {
                                min_dur_index = i;
                            }
                        }
                    }
                    b = std::dynamic_pointer_cast<exec_task>(events_vec_copy[min_dur_index]);
                    events_vec_copy.erase(events_vec_copy.begin() + min_dur_index);
                    for (auto &&e: events_vec_copy) {
                        global::events.push(e);
                    }
                    b->execute_fifo();
                } else {
                    global::events.pop();
                    b->execute_fifo();
                }


            }
            else if (auto c = std::dynamic_pointer_cast<add_task>(a)) {
                global::events.pop();
                c->execute();
            }
            else if (auto d = std::dynamic_pointer_cast<cpu_done>(a)) {
                global::events.pop();
                d->execute();
            }
            else if (auto e = std::dynamic_pointer_cast<io_done>(a)) {
                global::events.pop();
                e->execute();
            }
            else if (auto f = std::dynamic_pointer_cast<sim_done>(a)) {
                global::events.pop();
                f->execute();
            }
        }
        std::cout << std::endl << "done with simulation" << std::endl;
    }

    /**
    * Approximate shortest job first. Here we look at past cpu burst times.
    */
    void run_simulation_approx_sjf() {

        global::memory.clear();
        for(int i =0; i < sim_state::get_inst().pageSize; i++){
            global::memory.push_back(-1);
        }

        while (!global::events.empty()) {
            global::events.pop();
        }

        for (int i = 0; i < NUM_TASKS; ++i) {
            std::shared_ptr<add_task> a;
            a = std::make_shared<add_task>(i * sim_state::get_inst().freqJobCreation);
            global::events.push(a);
        }
        std::shared_ptr<sim_event> done = std::make_shared<sim_done>(10000.0);
        global::events.push(done);

        while (!global::events.empty()) {

            std::shared_ptr<sim_event> a = global::events.top(); //get shortest thing here

            if (auto b = std::dynamic_pointer_cast<exec_task>(a)) {
                std::vector<std::shared_ptr<sim_event> > events_vec_copy;
                while (!global::events.empty()) {
                    events_vec_copy.push_back(global::events.top());
                    global::events.pop();
                }

                int desired_index = 0;
                double min_avg = 1000000;   //big number maybe?
                for (int i = 0; i < events_vec_copy.size(); i++) {

                    if (auto curr_event_cpy = std::dynamic_pointer_cast<exec_task>(events_vec_copy[i])) {
                        double all_times_total = 0;
                        int total_ct = curr_event_cpy->curr_burst;
                        for (int j = 0; j < curr_event_cpy->curr_burst; j++) {
                            all_times_total += curr_event_cpy->bursts[j]->duration;
                        }
                        double avg_time = all_times_total / total_ct;
                        //std::cout << "avg :" << avg_time << std::endl;
                        if (avg_time < min_avg) {
                            min_avg = avg_time;
                            desired_index = i;
                        }
                    }
                }
                b = std::dynamic_pointer_cast<exec_task>(events_vec_copy[desired_index]);
                events_vec_copy.erase(events_vec_copy.begin() + desired_index);
                for (auto &&e: events_vec_copy) {
                    global::events.push(e);
                }
                b->execute_fifo();


            }
            else if (auto c = std::dynamic_pointer_cast<add_task>(a)) {
                global::events.pop();
                c->execute();
            }
            else if (auto d = std::dynamic_pointer_cast<cpu_done>(a)) {
                global::events.pop();
                d->execute();
            }
            else if (auto e = std::dynamic_pointer_cast<io_done>(a)) {
                global::events.pop();
                e->execute();
            }
            else if (auto f = std::dynamic_pointer_cast<sim_done>(a)) {
                global::events.pop();
                f->execute();
            }
        }
        std::cout << std::endl << "done with simulation" << std::endl;
    }


    void run_simulation_round_robin() {
        run_simulation_base(2);
    }
};


#endif

//hw7, Keano Gottlicher, A01647928, CS3100 Sp2015
#ifndef EVENT_HPP
#define EVENT_HPP

#include <queue>
#include <memory>
#include <iostream>
#include <map>
#include <algorithm>
#include "sim_state.hpp"
#include "calculators.h"


enum burst_type {
    B_CPU, B_IO
};


class sim_event {
public:
    bool operator<(const sim_event &rhs);
    double get_start_time()const;
    void set_start_time(double start_time) {
        this->start_time = start_time;
    }

    void execute();

    sim_event(double start_time) : start_time(start_time){};
    virtual ~sim_event() = default;

protected:
    double start_time;
};

struct com_sim_event{
    bool operator()(std::shared_ptr<sim_event> a, std::shared_ptr<sim_event> b);
};

struct burst {
public:
    burst(burst_type type, double duration, int owner_task_id, int io_device = -1) :
            type(type), duration(duration), owner_task_id(owner_task_id), io_device(io_device){
        for(int i = 0; i < 10; i++){    //10 memory pages/task
            memberMemory.push_back(sim_state::get_inst().get_mem_int());
        }
    }
    virtual ~burst() = default;

    burst_type type;
    std::vector<int> memberMemory;
    double duration;
    int io_device;
    int owner_task_id;
};

class exec_task : public sim_event {
public:
    exec_task(double start_time, int num_bursts, task_type type, int task_id);
    //virtual ~exec_task() = default;
    void execute_fifo();
    void execute_round_robin();
    int get_task_id(){ return task_id;}
    int num_bursts;
    int curr_burst;
    double orig_start_time;

    std::deque<std::shared_ptr<burst> > bursts;

    int task_id;
};

class add_task : public sim_event {
public:
    add_task(double start_time) : sim_event(start_time){};
    //virtual ~add_task() = default;

    void execute();
    void execute_sjf();

};

class cpu_done : public sim_event {
public:
    cpu_done(double start_time, int owner_task_id) : sim_event(start_time), owner_task_id(owner_task_id){};
    //virtual ~cpu_done() = default;

    void execute();
private:
    int owner_task_id;

};

class io_done : public sim_event {
public:
    io_done(double start_time, int owner_task_id, int io_num) : sim_event(start_time), owner_task_id(owner_task_id), io_num(io_num){};
    //virtual ~io_done() = default;

    void execute();
private:
    int io_num;
    int owner_task_id;

};

class sim_done : public sim_event {
public:
    sim_done(double start_time) : sim_event(start_time){}
    //virtual ~sim_done() = default;

    //print/output results
    void execute();

    void process_res(std::string s){
        std::cout << s << std::endl;
        sim_state::log_msg(s);
    }
};

namespace global{
    extern std::priority_queue< std::shared_ptr<sim_event> , std::vector<std::shared_ptr<sim_event> >, com_sim_event > events;
    extern std::vector<std::shared_ptr<sim_event> > events_sjf;
    extern std::vector<double> cpu_bursts_times;
    extern std::map<int, std::shared_ptr<exec_task> > all_tasks;
    extern std::queue<std::shared_ptr<burst> > cpu_queue;
    extern std::queue<std::shared_ptr<burst> > io_queue[50];
    extern std::shared_ptr<exec_task> last_task;
    extern std::vector<double> latency;
    extern std::vector<double> response_time;
    extern double utilStart;
    extern double utilTotal;
    extern std::deque<int> memory;
    extern int totalPageReads;
    extern int totalpageFaults;
}


#endif

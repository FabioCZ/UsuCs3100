//hw7, Keano Gottlicher, A01647928, CS3100 Sp2015
#include <fstream>
#include <sstream>
#include "sim_event.hpp"


std::priority_queue<std::shared_ptr<sim_event>, std::vector<std::shared_ptr<sim_event> >, com_sim_event> global::events;
std::vector<std::shared_ptr<sim_event> > global::events_sjf;
std::vector<double> global::cpu_bursts_times;
std::map<int, std::shared_ptr<exec_task> > global::all_tasks;
std::queue<std::shared_ptr<burst> > global::cpu_queue;
std::queue<std::shared_ptr<burst> > global::io_queue[50];
std::shared_ptr<exec_task> global::last_task;
std::deque<int> global::memory;

std::vector<double> global::latency;
std::vector<double> global::response_time;
double global::utilStart = 0;
double global::utilTotal = 0;
int global::totalPageReads = 0;
int global::totalpageFaults = 0;


//base class stuff
bool sim_event::operator<(const sim_event &rhs) {
    return start_time < rhs.start_time;
}

double sim_event::get_start_time() const {
    return start_time;
}

void sim_event::execute() {

    sim_state::get_inst().currTime = start_time;

}


exec_task::exec_task(double start_time, int num_bursts, task_type type, int task_id)
        : num_bursts(num_bursts), sim_event(start_time), task_id(task_id) {
    bursts.push_back(std::make_shared<burst>(B_CPU, sim_state::get_inst().getDur_CPU(type), task_id));
    for (int i = 1; i < num_bursts; ++i) {
        if (i % 2 == 1 && i - 1 != num_bursts) {
            bursts.push_back(std::make_shared<burst>(B_IO, sim_state::get_inst().getDur_IO(type), task_id,
                                                     sim_state::get_inst().getDevice_IO()));
        } else if (i % 2 == 0) {
            bursts.push_back(std::make_shared<burst>(B_CPU, sim_state::get_inst().getDur_CPU(type), task_id));
        }
    }
    curr_burst = 0;
    orig_start_time = get_start_time();

}

void exec_task::execute_fifo() {
    sim_event::execute();
    sim_state::log_msg(std::string("executing task, id: ") + std::to_string(task_id) + std::string(" , currBurst:") +
                       std::to_string(curr_burst));
    if (curr_burst < num_bursts) {
        if (bursts[curr_burst]->type == B_CPU) {

            //region memory
            double pageFaultPenaltyTotal = 0;
            for (int i = 0; i < bursts[curr_burst]->memberMemory.size(); i++) {
                global::totalPageReads++;
                auto found = std::find(global::memory.begin(),
                                       global::memory.end(), bursts[curr_burst]->memberMemory[i]);
                if (found == global::memory.end()) {  //element NOT found == page fault
                    global::totalpageFaults++;
                    global::memory.pop_front();
                    global::memory.push_back(bursts[curr_burst]->memberMemory[i]);
                    pageFaultPenaltyTotal += sim_state::get_inst().costPageFault;
                }
            }
            //endregion

            if (sim_state::get_inst().availCpu <= sim_state::get_inst().numCpu) {
                global::cpu_bursts_times.push_back(bursts[curr_burst]->duration);
                std::shared_ptr<cpu_done> a = std::make_shared<cpu_done>(
                        sim_state::get_inst().currTime + bursts[curr_burst]->duration +
                        sim_state::get_inst().costContext + pageFaultPenaltyTotal, task_id);
                global::utilStart = sim_state::get_inst().currTime;
                global::events.push(a);
                sim_state::get_inst().numCpu--;
            } else {
                global::cpu_queue.push(bursts[curr_burst]);
            }
        } else {    //B_IO
            if (sim_state::get_inst().availIO[bursts[curr_burst]->io_device]) {
                std::shared_ptr<io_done> a = std::make_shared<io_done>(
                        sim_state::get_inst().currTime + bursts[curr_burst]->duration +
                        sim_state::get_inst().costContext, task_id, bursts[curr_burst]->io_device);
                global::events.push(a);
                sim_state::get_inst().availIO[bursts[curr_burst]->io_device] = false;
            } else {
                global::io_queue[bursts[curr_burst]->io_device].push(bursts[curr_burst]);
            }
        }

        if (curr_burst == 1) {
            global::response_time.push_back(sim_state::get_inst().currTime - orig_start_time);
        }
        if (curr_burst == num_bursts - 1) {
            global::latency.push_back(sim_state::get_inst().currTime - orig_start_time);    //TODO
        }
        curr_burst++;
    }
}

void exec_task::execute_round_robin() {
    sim_event::execute();
    sim_state::log_msg(std::string("executing task, id: ") + std::to_string(task_id) + std::string(" , currBurst:") +
                       std::to_string(curr_burst));
    bool whole_task_fit = true;
    if (curr_burst < num_bursts) {
        if (bursts[curr_burst]->type == B_CPU) {


            //region memory
            double pageFaultPenaltyTotal = 0;
            for (int i = 0; i < bursts[curr_burst]->memberMemory.size(); i++) {
                global::totalPageReads++;
                auto found = std::find(global::memory.begin(),
                                       global::memory.end(), bursts[curr_burst]->memberMemory[i]);
                if (found == global::memory.end()) {  //element NOT found == page fault
                    global::totalpageFaults++;
                    global::memory.pop_front();
                    global::memory.push_back(bursts[curr_burst]->memberMemory[i]);
                    pageFaultPenaltyTotal += sim_state::get_inst().costPageFault;
                }
            }
            //endregion

            if (sim_state::get_inst().availCpu <= sim_state::get_inst().numCpu) {
                std::shared_ptr<cpu_done> a;
                if (bursts[curr_burst]->duration <
                    sim_state::get_inst().roundRobinLimit) {   //whole CPU burst is shorter than round robin max
                    a = std::make_shared<cpu_done>(sim_state::get_inst().currTime + bursts[curr_burst]->duration +
                                                   sim_state::get_inst().costContext, task_id);
                } else {
                    whole_task_fit = false;
                    a = std::make_shared<cpu_done>(
                            sim_state::get_inst().currTime + sim_state::get_inst().roundRobinLimit +
                            sim_state::get_inst().costContext, task_id);
                    bursts[curr_burst]->duration -= sim_state::get_inst().roundRobinLimit;
                }
                global::utilStart = sim_state::get_inst().currTime;
                global::events.push(a);
                sim_state::get_inst().numCpu--;
            } else {
                global::cpu_queue.push(bursts[curr_burst]);
            }
        } else {    //B_IO
            if (sim_state::get_inst().availIO[bursts[curr_burst]->io_device]) {
                std::shared_ptr<io_done> a;
                if (bursts[curr_burst]->duration < sim_state::get_inst().roundRobinLimit) {
                    a = std::make_shared<io_done>(sim_state::get_inst().currTime + bursts[curr_burst]->duration +
                                                  sim_state::get_inst().costContext, task_id,
                                                  bursts[curr_burst]->io_device);
                } else {
                    whole_task_fit = false;
                    a = std::make_shared<io_done>(
                            sim_state::get_inst().currTime + sim_state::get_inst().roundRobinLimit +
                            sim_state::get_inst().costContext, task_id, bursts[curr_burst]->io_device);
                    bursts[curr_burst]->duration -= sim_state::get_inst().roundRobinLimit;
                }
                global::events.push(a);
                sim_state::get_inst().availIO[bursts[curr_burst]->io_device] = false;
            } else {
                global::io_queue[bursts[curr_burst]->io_device].push(bursts[curr_burst]);
            }
        }

        if (curr_burst == 1) {
            global::response_time.push_back(sim_state::get_inst().currTime - orig_start_time);
        }
        if (curr_burst == num_bursts - 1) {
            global::latency.push_back(sim_state::get_inst().currTime - orig_start_time);    //TODO
        }
        if (whole_task_fit) curr_burst++;
    }
}

void add_task::execute() {
    sim_event::execute();
    int task_id = sim_state::get_inst().get_next_task_id();

    std::shared_ptr<exec_task> new_task;
    //determine if we need a CPU/IO heavy task
    double new_start_time = sim_state::get_inst().currTime + 1.0;
    if (sim_state::get_inst().get_task_distro() <= sim_state::get_inst().taskMix) { //CPU heavy task
        new_task = std::make_shared<exec_task>(new_start_time, sim_state::get_inst().get_burst_ct(), CPU, task_id);
        sim_state::log_msg(
                "Adding CPU heavy task, which starts at: " + std::to_string(new_start_time) + std::string(" id: ") +
                std::to_string(new_task->get_task_id()) + std::string(" num bursts: ") +
                std::to_string(new_task->num_bursts));

    } else {    //IO heavy task
        new_task = std::make_shared<exec_task>(new_start_time, sim_state::get_inst().get_burst_ct(), IO, task_id);
        sim_state::log_msg(
                "Adding IO heavy task, which starts at: " + std::to_string(new_start_time) + std::string(" id: ") +
                std::to_string(new_task->get_task_id()) + std::string(" num bursts: ") +
                std::to_string(new_task->num_bursts));
    }
    global::events.push(new_task);
    global::all_tasks[new_task->get_task_id()] = new_task;
}

void add_task::execute_sjf() {

}

void cpu_done::execute() {
    sim_event::execute();

    sim_state::get_inst().availCpu++;
    if (!global::cpu_queue.empty()) {

        //region memory
        double pageFaultPenaltyTotal = 0;
        for (int i = 0; i < global::cpu_queue.front()->memberMemory.size(); i++) {
            global::totalPageReads++;

            auto found = std::find(global::memory.begin(),
                                   global::memory.end(), global::cpu_queue.front()->memberMemory[i]);
            if (found == global::memory.end()) {  //element NOT found == page fault
                global::totalpageFaults++;
                global::memory.pop_front();
                global::memory.push_back(global::cpu_queue.front()->memberMemory[i]);
                pageFaultPenaltyTotal += sim_state::get_inst().costPageFault;
            }
        }
        //endregion

        std::shared_ptr<cpu_done> a = std::make_shared<cpu_done>(
                global::cpu_queue.front()->duration + sim_state::get_inst().currTime +
                sim_state::get_inst().costContext, global::cpu_queue.front()->owner_task_id);
        sim_state::get_inst().availCpu--;
        global::events.push(a);
        global::cpu_queue.pop();
    } else {
        global::utilTotal += ((sim_state::get_inst().currTime -
                               global::utilStart));// * (sim_state::get_inst().numCpu - sim_state::get_inst().availCpu - 1));
    }
    if (global::all_tasks[owner_task_id]->curr_burst - 1 < global::all_tasks[owner_task_id]->num_bursts) {
        global::all_tasks[owner_task_id]->set_start_time(sim_state::get_inst().currTime);
        global::events.push(global::all_tasks[owner_task_id]);
    }
    sim_state::log_msg("done cpu");
}

void io_done::execute() {
    sim_event::execute();
    sim_state::log_msg(std::string("io done, io_num:") + std::to_string(io_num) + std::string(" owner task id: ") +
                       std::to_string(owner_task_id));

    sim_state::get_inst().availIO[io_num] = true;

    if (!global::io_queue[io_num].empty()) {
        std::shared_ptr<io_done> a = std::make_shared<io_done>(
                global::io_queue[io_num].front()->duration + sim_state::get_inst().currTime +
                sim_state::get_inst().costContext, global::io_queue[io_num].front()->owner_task_id, io_num);
        sim_state::get_inst().availIO[io_num] = false;
        global::events.push(a);
        global::io_queue[io_num].pop();
    }

    if (global::all_tasks[owner_task_id]->curr_burst - 1 < global::all_tasks[owner_task_id]->num_bursts) {
        global::all_tasks[owner_task_id]->set_start_time(sim_state::get_inst().currTime);
        global::events.push(global::all_tasks[owner_task_id]);
    }

}


bool com_sim_event::operator()(std::shared_ptr<sim_event> a, std::shared_ptr<sim_event> b) {
    return (*a).get_start_time() > (*b).get_start_time();
}

void sim_done::execute() {
    sim_state::log_msg(std::string("sim_done event"));


    process_res(std::string("response time: avg: ") + std::to_string(calculators::mean(global::response_time)));
    process_res(std::string("response time: std dev: ") + std::to_string(calculators::stdDev(global::response_time)));

    process_res(std::string("response time: min: ") +
                std::to_string(*std::min_element(global::response_time.begin(), global::response_time.end())));
    process_res(std::string("response time: max: ") +
                std::to_string(*std::max_element(global::response_time.begin(), global::response_time.end())));

    process_res(std::string("latency: avg: ") + std::to_string(calculators::mean(global::latency)));
    process_res(std::string("latency: std dev: ") + std::to_string(calculators::stdDev(global::latency)));

    process_res(std::string("latency: min: ") +
                std::to_string(*std::min_element(global::latency.begin(), global::latency.end())));
    process_res(std::string("latency: max: ") +
                std::to_string(*std::max_element(global::latency.begin(), global::latency.end())));

    //process_res(std::string("utilized time: ") + std::to_string(global::utilTotal / sim_state::get_inst().numCpu));

    process_res(std::string("utilization: ") + std::to_string(
            (global::utilTotal / (sim_state::get_inst().currTime /* sim_state::get_inst().numCpu*/)) * 100) +
                std::string("%"));
    process_res(std::string("throughput: ") +
                std::to_string(sim_state::get_inst().taskCt / sim_state::get_inst().currTime) +
                std::string("tasks/unit time"));

    process_res(std::string("Total memory reads: ") + std::to_string(global::totalPageReads));
    process_res(std::string("Total memory faults: ") + std::to_string(global::totalpageFaults));

    //reset stuff for next simulation. I know this is bad. I am sorry.
    global::response_time.clear();
    global::latency.clear();
    global::utilTotal = 0;
    global::totalpageFaults = 0;
    global::totalPageReads = 0;
    sim_state::get_inst().currTime = 0;
    sim_state::get_inst().availCpu = sim_state::get_inst().numCpu;
    sim_state::get_inst().taskCt = 0;
}
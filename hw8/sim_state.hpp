//hw7, Keano Gottlicher, A01647928, CS3100 Sp2015
#ifndef RANDOMIZER_HPP
#define RANDOMIZER_HPP

#include <random>
#include <memory>
#include <sstream>
#include <fstream>

enum task_type {
    CPU, IO
};

//singleton instance
class sim_state {
public:

    //parameters of simualtion
    int numCpu;
    int numIo;
    double costContext;
    double taskMix;
    double freqJobCreation;
    double roundRobinLimit;
    double costPageFault;
    int pageSize;

    //state information
    int availCpu;
    bool availIO[50];   //screw it, no need for 50+ IOs
    double currTime;
    int taskCt; //really indexes for task ids

    static sim_state& get_inst();

    static void log_msg(std::string msg);

    double getDur_CPU(task_type type) const;

    double getDur_IO(task_type type) const;

    int getDevice_IO() const;

    double get_task_distro() const;

    int get_burst_ct() const;

    void set_num_io(int num_io);

    int get_next_task_id();

    int get_mem_int();

private:
    sim_state();
    //~sim_state();
    sim_state(const sim_state&) = delete;
    void operator=(sim_state const&) = delete;

    std::random_device randD;
    std::shared_ptr<std::mt19937> mt;
    std::shared_ptr<std::normal_distribution<double> > dur_CPU_CPU;
    std::shared_ptr<std::normal_distribution<double> > dur_CPU_IO;
    std::shared_ptr<std::normal_distribution<double> > dur_IO_CPU;
    std::shared_ptr<std::normal_distribution<double> > dur_IO_IO;
    std::shared_ptr<std::uniform_int_distribution<> > device_IO; //or
    std::shared_ptr<std::uniform_int_distribution<> > burst_ct; //or
    std::shared_ptr<std::uniform_int_distribution<> > memoryInts; //or
    std::shared_ptr<std::uniform_real_distribution<double> > task_distro;

};

#endif
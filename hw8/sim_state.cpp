//hw7, Keano Gottlicher, A01647928, CS3100 Sp2015
#include "sim_state.hpp"

sim_state::sim_state() {
    mt = std::make_shared<std::mt19937>(randD());
    dur_CPU_CPU = std::make_shared<std::normal_distribution<double> >(15.0, 1.0);
    dur_CPU_IO = std::make_shared<std::normal_distribution<double> >(3.0, 1.0);
    dur_IO_CPU = std::make_shared<std::normal_distribution<double> >(3.0, 1.0);
    dur_IO_IO = std::make_shared<std::normal_distribution<double> >(20.0, 1.0);
    burst_ct = std::make_shared<std::uniform_int_distribution<> >(5, 10);
    memoryInts = std::make_shared<std::uniform_int_distribution<> >(0, 100);    //memory contents can be 1-100
    task_distro = std::make_shared<std::uniform_real_distribution<double> >(0, 100);
    taskCt = 0;
}


sim_state &sim_state::get_inst() {
    static sim_state instance;
    return instance;
}

double sim_state::getDur_CPU(task_type type) const {
    double dur;
    do {
        dur = type == CPU ? (*dur_CPU_CPU)(*mt) : (*dur_IO_CPU)(*mt);
    } while (dur < 0);
    return dur;
}

double sim_state::getDur_IO(task_type type) const {
    double dur;
    do {
        dur = type == CPU ? (*dur_CPU_IO)(*mt) : (*dur_IO_IO)(*mt);
    } while (dur < 0);
    return dur;
}

int sim_state::getDevice_IO() const {
    int io = (*device_IO)(*mt);
    return io;
}

double sim_state::get_task_distro() const {
    return (*task_distro)(*mt);
}

int sim_state::get_burst_ct() const {
    return (*burst_ct)(*mt);
}

void sim_state::set_num_io(int num_io) {
    numIo = num_io;
    device_IO = std::make_shared<std::uniform_int_distribution<> >(0, numIo - 1);
    for (int i = 0; i < numIo; ++i) {
        availIO[i] = true;
    }
}

void sim_state::log_msg(std::string msg) {
    {
        std::ofstream outfile;

        outfile.open("simulator.log", std::ios_base::app);
        outfile << "[" << get_inst().currTime << "]: " << msg << std::endl;
    }
}

int sim_state::get_next_task_id() {
    int toReturn = taskCt;
    ++taskCt;
    return toReturn;
}

int sim_state::get_mem_int() {
    return (*memoryInts)(*mt);
}

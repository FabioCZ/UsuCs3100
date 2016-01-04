//hw8, Keano Gottlicher, A01647928, CS3100 Sp2015

#include <iostream>
#include <vector>
#include <fstream>

#include "simulator.hpp"

static void show_help();


int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_help();
            return 0;
        }
        if(arg == "-c" || arg == "--cpu"){
            sim_state::get_inst().numCpu = atoi(argv[i+1]);
            if(sim_state::get_inst().numCpu < 1){
                std::cout << "Invalid number of CPUs";
                return -1;
            }
            ++i;
        }
        if(arg == "-i" || arg == "-io"){
            sim_state::get_inst().set_num_io( atoi(argv[i+1]));
            if(sim_state::get_inst().numIo < 1){
                std::cout << "Invalid number of IOs";
                return -1;
            }
            ++i;
        }
        if(arg == "-s" || arg == "--switch"){
            char* end;
            sim_state::get_inst().costContext = std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().costContext <= 0.0){
                std::cout << "Invalid context switch cost";
                return -1;
            }
            ++i;
        }
        if(arg == "-t" || arg == "--task-mix"){
            char* end;
            sim_state::get_inst().taskMix = std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().taskMix <= 0.0 || sim_state::get_inst().taskMix >= 100){
                std::cout << "Invalid task mix ratio";
                return -1;
            }
            ++i;
        }
        if(arg == "-f" || arg == "-frequency"){
            char* end;
            sim_state::get_inst().freqJobCreation = std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().freqJobCreation <= 0.0){
                std::cout << "Invalid frequency of job creation";
                return -1;
            }
            ++i;
        }
        if(arg == "-r" || arg == "--roundrobin"){
            char* end;
            sim_state::get_inst().roundRobinLimit = std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().roundRobinLimit <= 0.0 ){
                std::cout << "Invalid round robin limit";
                return -1;
            }
            ++i;
        }
        if(arg == "-p" || arg == "--pagefaultcost"){
            char* end;
            sim_state::get_inst().costPageFault = std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().costPageFault <= 0.0 ){
                std::cout << "Invalid page fault cost";
                return -1;
            }
            ++i;
        }

        if(arg == "-q" || arg == "--pagesize"){
            char* end;
            sim_state::get_inst().pageSize = (int) std::strtod(argv[i+1], &end);
            if(sim_state::get_inst().pageSize <= 0.0 ){
                std::cout << "Invalid page fault cost";
                return -1;
            }
            ++i;
        }
    }

    std::cout << "Creating a simulator, CPUs: " << sim_state::get_inst().numCpu << ", IOs: " << sim_state::get_inst().numIo << ", context switch cost: " << sim_state::get_inst().costContext;
    std::cout << ", task mix: " << sim_state::get_inst().taskMix << ", frequency of job creation: " << sim_state::get_inst().freqJobCreation << std::endl;

    simulator sim;
    std::cout << "Now running FIFO: " << std::endl;
    sim.run_simulation_fifo();
    std::cout << std::endl << "Now running Round Robin" << std::endl;
    sim.run_simulation_round_robin();
    std::cout << std::endl << "Now running Shortest Job First" << std::endl;
    sim.run_simulation_sjf();
    std::cout << std::endl << "Now running Approx Shortest Job First" << std::endl;
    sim.run_simulation_approx_sjf();
    return 0;
}

static void show_help(){
    std::ifstream fStream("help.txt");
    std::cout << fStream.rdbuf();
    fStream.close();
}
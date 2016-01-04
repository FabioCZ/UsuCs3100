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
    }

    std::cout << "Creating a simulator, CPUs: " << sim_state::get_inst().numCpu << ", IOs: " << sim_state::get_inst().numIo << ", context switch cost: " << sim_state::get_inst().costContext;
    std::cout << ", task mix: " << sim_state::get_inst().taskMix << ", frequency of job creation: " << sim_state::get_inst().freqJobCreation << std::endl;

    simulator_fifo sim;

    sim.run_simulation();
    return 0;
}

static void show_help(){
    std::ifstream fStream("help.txt");
    std::cout << fStream.rdbuf();
    fStream.close();
}
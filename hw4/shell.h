#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <numeric>


namespace shell{

    template<typename Container>
    double mean(Container const &container) {
        double total = 0;

        for (auto &&i : container) {
            total += i;
        }
        return ((double) total) / ((double) container.size());
    }

    void runShell(){
        bool exit = false;
        std::vector<std::string> commands;
        std::vector<double> times;
        std::cout << "Welcome to Fabio's Fun Awesome Shell (FFAS). For help, type s-help" << std::endl;

        while (!exit) {
            std::chrono::high_resolution_clock::time_point timeBegin, timeEnd;
            std::string currCommand;
            std::cout << "(Fabio's shell) $ ";
            std::getline(std::cin, currCommand);
            commands.push_back(currCommand);

            if (commands.back() == "history") {
                std::cout << "History of commands: " << std::endl;
                for (int i = 0; i < commands.size() - 1; i++) {   //size - 1 so we don't show "history"
                    std::cout << i + 1 << ". : " << commands[i] << std::endl;
                }

            } else if (commands.back().substr(0, 1) == "^") {
                if (commands.back().substr(1, 1) != " ") {
                    std::cout << "Improper command format" << std::endl;
                } else {
                    int hist_count = atoi(commands.back().substr(2, commands.back().length() - 2).c_str());
                    if (hist_count > commands.size()) {
                        std::cout << "There are not " << hist_count << " commands in the history. Try again with lower number." << std::endl;
                    } else {
                        std::cout << "The " << hist_count << "th command in the history is: " << commands[hist_count - 1] << std::endl;
                    }
                }
            } else if(commands.back() == "ptime"){
                std::cout << "Last process exec time: " << times.back() << "s" << std::endl;
                std::cout << "Avg. exec time: " << mean(times)  << "s"<< std::endl;
                std::cout << "Total time spent executing: " << std::accumulate(times.begin(), times.end(), 0.0) << "s" << std::endl;
            } else if (commands.back() == "s-help") {
                std::cout << "Help for Fabio's Fun Awesome Shell (FFAS), available commands:" << std::endl;
                std::cout << "history - prints out all past commands" << std::endl;
                std::cout << "^ n - prints out nth command in history" << std::endl;
                std::cout << "ptime - get time for last command and average time of execution" << std::endl;
                std::cout << "exit - exit the shell" << std::endl;
                std::cout << "anything else - execute that command" << std::endl;
            } else if (commands.back() == "exit") {
                exit = true;
            } else {
                std::vector<char *> arg;

                std::string buf; // Have a buffer string
                std::stringstream ss(commands.back()); // Insert the string into a stream

                while (ss >> buf) {
                    arg.push_back(strdup(buf.c_str()));
                }
                arg.push_back(nullptr);     //terminate with nullptr
                //start timer here
                timeBegin = std::chrono::high_resolution_clock::now();
                auto pid = fork();

                if (pid < 0) {
                    std::cerr << "Could not fork, error: " << pid << std::endl;
                }
                if (pid == 0) {
                    int code = execvp(arg[0], arg.data());
                    if (code != -1) {
                        std::cout << "Error executing, code: " << code << " , exiting now";
                        exit = true;
                    }
                }
                //wait for pid to finish
                waitpid(pid, nullptr, 0);
                timeEnd = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> time_span = duration_cast<std::chrono::duration<double>>(timeEnd - timeBegin);
                times.push_back(time_span.count());
                //end timer here

            }
            std::cout << std::endl;


        }

    }
}
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <numeric>
#include <array>
#include <chrono>
#include <sys/wait.h>


namespace shell {

    const int READ_END = 0;
    const int WRITE_END = 1;
    enum pipeMode{ INPUT, OUTPUT, IN_AND_OUT,  NORMAL};

    struct cmd {
        std::vector<std::string> commands;
        std::string inputfileName = "";
        std::string outputfileName = "";
        int inputFileDesc = -1;
        int outputFileDesc = -1;

        cmd(std::string wholeCommandString) {
            //string parse begin
            std::vector<std::string> cmdWords;

            std::string buf; // Have a buffer string
            std::stringstream ss(wholeCommandString); // Insert the string into a stream
            while (ss >> buf) {
                cmdWords.push_back(buf);
            }

            for (int i = 0; i < cmdWords.size(); ++i) {
                if (cmdWords[i].compare("<") == 0) { //input file
                    inputfileName = cmdWords[i + 1];
                    ++i;
                } else if (cmdWords[i].compare(">") == 0) {  //output file
                    outputfileName = (cmdWords[i + 1]);
                    ++i;
                } else if (cmdWords[i].compare("|") != 0) {
                    commands.push_back(cmdWords[i]);
		if(i < cmdWords.size()){
                    while (cmdWords[i + 1].compare("|") != 0 && cmdWords[i + 1].compare("<") != 0 &&
                            cmdWords[i + 1].compare(">") != 0) {
                        ++i;
                        commands.back().append(" ");
                        commands.back().append(cmdWords[i]);
			if(i > cmdWords.size() -1){
				break;
			}
                    }
		}
                }
            }
            //string parse end
        }
    };

    std::vector<char *> getArgCharPtr(std::string cmd) {
        std::vector<char *> arg;

        std::string buf; // Have a buffer string
        std::stringstream ss(cmd);
        while (ss >> buf) {
            arg.push_back(strdup(buf.c_str()));
        }
        arg.push_back(nullptr);     //terminate with nullptr
        return arg;
    }

    template<typename Container>
    double mean(Container const &container) {
        double total = 0;

        for (auto &&i : container) {
            total += i;
        }
        return ((double) total) / ((double) container.size());
    }

    pid_t exec_process(std::string cmd, int const pipes[][2], int n, pipeMode p,  std::string inputFileName, std::string outputFileName) {
        auto pid = fork();
        if (pid == 0) {

            if((p == INPUT || p == IN_AND_OUT) && inputFileName.compare("") != 0){
                freopen(inputFileName.c_str(), "r", stdin);
            }
            if((p == OUTPUT || p == IN_AND_OUT) && outputFileName.compare("") != 0 ){
                freopen(outputFileName.c_str(), "w", stdout);
            }

            if (pipes[n][READ_END] >= 0) dup2(pipes[n][READ_END], STDIN_FILENO);
            if (pipes[n][WRITE_END] >= 0) dup2(pipes[n][WRITE_END], STDOUT_FILENO);

            //close all pipes
            for (int j = 0; j < 64; ++j) {
                if (pipes[j][READ_END] >= 0) close(pipes[j][READ_END]);
                if (pipes[j][WRITE_END] >= 0) close(pipes[j][WRITE_END]);
            }

            //DEBUG std::cout << "0/in: " << pipes[n][READ_END] << " 1/out: " << pipes[n][WRITE_END] << std::endl;
            std::vector<char *> arg = getArgCharPtr(cmd);
            execvp(arg[0], arg.data());

            if((p == INPUT || p == IN_AND_OUT) && inputFileName.compare("") != 0){
                fclose(stdin);
            }
            if((p == OUTPUT || p == IN_AND_OUT) && outputFileName.compare("") != 0){
                fclose(stdout);
            }
        }
        return pid;
    }

    void runShell() {
        bool reExec = false;
        bool exit = false;
        int pipes[64][2];
        std::vector<std::string> commands;
        std::vector<double> times;
        std::cout << "Welcome to Fabio's Fun Awesome Shell (FFAS). For help, type s-help" << std::endl;

        while (!exit) {
            std::chrono::high_resolution_clock::time_point timeBegin, timeEnd;
            std::string currCommand;
            std::cout << "(Fabio's shell) $ ";

            if (!reExec) {
                std::getline(std::cin, currCommand);
                commands.push_back(currCommand);
            }
            reExec = false;

            if (commands.back().compare("history") == 0) {
                std::cout << "History of commands: " << std::endl;
                for (int i = 0; i < commands.size() - 1; i++) {   //size - 1 so we don't show "history"
                    std::cout << i + 1 << ". : " << commands[i] << std::endl;
                }

            } else if (commands.back().substr(0, 1).compare("^") == 0) {
                if (commands.back().substr(1, 1).compare(" ") != 0) {
                    std::cout << "Improper command format" << std::endl;
                } else {
                    int hist_count = atoi(commands.back().substr(2, commands.back().length() - 2).c_str());
                    if (hist_count > commands.size()) {
                        std::cout << "There are not " << hist_count << " commands in the history. Try again with lower number." << std::endl;
                    } else {
                        std::cout << "Executing the " << hist_count << "th command in the history: " << commands[hist_count - 1] << std::endl;
                        commands.push_back(commands[hist_count - 1]);
                        reExec = true;
                    }
                }
            } else if (commands.back() .compare("ptime") == 0) {
                std::cout << "Last process exec time: " << times.back() << "s" << std::endl;
                std::cout << "Avg. exec time: " << mean(times) << "s" << std::endl;
                std::cout << "Total time spent executing: " << std::accumulate(times.begin(), times.end(), 0.0) << "s" << std::endl;
            } else if (commands.back() == "s-help") {
                std::cout << "Help for Fabio's Fun Awesome Shell (FFAS), available commands:" << std::endl;
                std::cout << "history - prints out all past commands" << std::endl;
                std::cout << "^ n - prints out nth command in history" << std::endl;
                std::cout << "ptime - get time for last command and average time of execution" << std::endl;
                std::cout << "exit - exit the shell" << std::endl;
                std::cout << "anything else - execute that command. You can use pipes (|) and I/O redirection (</>)" << std::endl;
            } else if (commands.back().compare("exit") == 0) {
                exit = true;
            } else {

                cmd c(commands.back());
                //std::vector<std::array<int, 2> > pipes(c.commands.size() + 1);
                std::vector<pid_t> children_pids;

                //clear pipes
                for(int i =0; i < 64; ++i)
                {
                    pipes[i][READ_END]=-1;
                    pipes[i][WRITE_END]=-1;
                }


                timeBegin = std::chrono::high_resolution_clock::now();

                //create pipes
                for(int i = 0; i < c.commands.size() - 1; ++i){
                    int p[2];
                    pipe(p);
                    pipes[i][WRITE_END] = p[STDOUT_FILENO]; // Process N writes to the pipe
                    pipes[i + 1][READ_END] = p[STDIN_FILENO]; // Process N+1 reads from the pipe
                }

                //execute each command here
                for (int i = 0; i < c.commands.size(); ++i) {
                    if(i == 0){
                        if(c.commands.size() == 1){
                            children_pids.push_back(exec_process(c.commands[i], pipes, i,
                                IN_AND_OUT, c.inputfileName, c.outputfileName));  
                        } else {
                            children_pids.push_back(exec_process(c.commands[i], pipes, i,
                                INPUT, c.inputfileName, c.outputfileName));
                        }
                    } else if(i == c.commands.size() - 1){
                        children_pids.push_back(exec_process(c.commands[i], pipes, i,
                                OUTPUT, c.inputfileName, c.outputfileName));
                    }
                    else {
                        children_pids.push_back(exec_process(c.commands[i], pipes, i,
                                NORMAL, c.inputfileName, c.outputfileName));
                    }
                }

                //close all pipes
                for(int i = 0; i < 64; ++i){
                    close(pipes[i][READ_END]);
                    close(pipes[i][WRITE_END]);
                }

                //wait here
                for(auto&& child_pid : children_pids){
                    waitpid(child_pid, nullptr, 0);
                }
                timeEnd = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(timeEnd - timeBegin);
                times.push_back(time_span.count());

                std::cout << std::endl;
            }

        }
    }
}

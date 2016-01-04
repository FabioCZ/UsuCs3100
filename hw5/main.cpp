#include "shell.h"

//duplicate the pipe to standard out
//< always applies to first command, > always applies to last command in chain.
//They can be anywhere in the command.
//use dup2 to associate output for pipe. dup2(old, new);
//2 pipes only and loop? or can make multiple pipes when parsing string input.
//have a data structure for parsed command line input.
//use fileno(FILE* stream) to convert filestream to file descriptor, use fOpen()
int main() {


    shell::runShell();

    //
    //std::string line;
    //std::getline(std::cin, line);


//    int toParent[2];   //read on zero, write on one
//    int toChild[2];
//
//    pipe(toParent);
//    pipe(toChild);
//    auto pid = fork();
//    if(pid < 0) {
//        std::cerr << "error";
//        return 0;
//    }if(pid == 0){
//        close(toParent[0]);
//        close(toChild[0]);
//        write(toParent[1],"Hello\n", 6);
//        close(toParent[1]);
//        char c;
//        while (read(toChild[0], &c, 1)) {
//            std::cout << c;
//        }
//
//    } else {        //parent
//        close(toParent[1]);
//        close(toChild[0]);
//
//        waitpid(pid, nullptr, 0);
//
//        char c;
//        while (read(toParent[0], &c, 1)) {
//            std::cout << c;
//        }
//        write(toChild[1],"child\n", 6);
//        close(toChild[1]);
//    }

    return 0;
}


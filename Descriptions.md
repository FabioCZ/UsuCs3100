## HW 1 - Modern C++ style

Create the following tools which will be used in later assignments.  Avoid the for and while keywords

A function that computes the standard deviation of elements in any container
A function that computes the average (mean) of elements in any container
A function that times the processing of another function
A function that repeatedly invokes another function, and returns the average and standard deviation of the time taken.
The called function should be able to have any reasonable signature
Also prepare a main function that demonstrates the capabilities of your toolset.

## HW 2 - Serial Mandelbrot

Generate an image of some portion of the Mandelbrot set.  Turn in the image along with the code.  Also prepare a graph of the performance of your code with respect to image size.

## HW 3 - Concurrent Mandelbrot

Add threading to your generation of the mandelbrot set.  Along with the code and image prepare a short (~1 page) report on the speedup you were able to achieve.

## HW 4 - Shell

Introduction
A shell is a user program or environment provides for user interaction. The shell is a command language interpreter that accepts commands from standard input (the keyboard) and executes them. A shell may simply accept user commands and pass them off to the operating system to execute them, it may additionally provide mechanisms, such as redirecting the output of one program to the input of another.

Assignment
Write a C++ program that will act as a command line interpreter for Linux. Use the “[cmd]:” string for your prompt. For example, when a user types a line such as…

    word1 word2 word3

...the shell interprets 'word1' as the name of an executable program and 'word2' and 'word3' as parameters to be passed to that program in argv. Your shell should accept any number of command line parameters.

The shell should use the directory system in the order specified by the PATH environment variable to locate the executable file (i.e. You don't have to do anything, don't send me an email asking how to do this, it automatically happens). If the file is found, the shell should pass all parameters in argv and be executed. When the executable file has completed or terminated, the shell program should again resume with the next prompt. Your shell program must wait to display the next command prompt until the previous command has finished executing.

The shell should track the amount of time spent executing child processes. Whenever the command 'ptime' is entered into your shell, the number of seconds, accurate to microseconds, spent executing (waiting for) child processes must be displayed.

The shell should keep a history of the previous commands entered, with no maximum hard-coded history length. If the user enters the command 'history', the shell should provide a listing of the complete command shell history. If the user enters the command '^ 10', for example, the 10th command in the history buffer should be executed (a 1 based index).

The command 'exit' is used to terminate your shell program, when entered, your shell program should end.

The following is an example of what the command shell will look like and how programs will be executed.

    [cmd]: ls
          shell
          shell.cpp
    [cmd]: ptime
          Time spent executing child processes: 0 seconds 24 milliseconds and 380 microseconds
    [cmd]:
Notes
You must use fork, execvp, wait and std::chrono to complete this assignment.
Reference for std::chrono : http://en.cppreference.com/w/cpp/chrono (Links to an external site.)
Your shell must accept any number of parameters.
Your shell must not have any global variables.
Your shell must not crash when the user enters bad commands or bad command arguments.

## HW 5 - Shell with Redirection

Update your shell program from Assignment 4 to support IO redirection.  You need to support redirecting input and output to files and pipes.

 

The input to an executable can be from a file using this syntax:

foo < infilename

 

The output from a executable can be redirected to a file using this syntax:

foo > outfilename

 

The output from an executable can be redirected to the input of a second using this syntax:

foo | bar

 

These three operations can be chained to an arbitrary depth.  For example:

A < infilename | B | C | D > outfilename

## HW 6 - Scheduling Simulator - Framework

A CPU task will be represented as a series of CPU and IO bursts.  The simulated scheduler may only see whether a task is requesting CPU or IO time -- not how much time. The values for this series should be drawn from two different populations.  A CPU bound population with many large CPU bursts and few IO bursts, and a IO bound population with small CPU bursts and many IO bursts.  Your first assignment will build a simulator for examining different scheduling algorithms.  Be sure to design your simulator to make it easy to implement new algorithms each week.  Note that different algorithms will want to store different data about tasks so be sure to include that in your design.

  The only algorithm required for this first week is a non-interruptible FIFO schedule.  Your simulator needs to compute average, minimum, and maximum values as well as their standard deviations for the following measurements:

Latency - How long does it take between when a task is created and when it completes
Response Time - How long does it take between when a task is created and when it first responds (first IO burst finishes)
Utilization - How many system resources (expressed as a percent) are being used.
Throughput - How many tasks / unit time can the system process.
Your simulator should have the following options which can be changed from the command line or a menu:

Number of CPU's -- you may assume that all CPU's are homogeneous
Number of IO devices -- an IO burst must be associated with a given device
Cost of a context switch -- What is the overhead paid for each context switch between tasks
Task mix -- what percentage of tasks are CPU vs IO bound
Frequency of job creation -- how often are new tasks added to the queue
 

Also turn in a brief report (1-2 pages) outlining your conclusions regarding the FIFO scheduling algorithm.

# HW 7 - CPU Scheduling Algorithms

Add the following scheduling algorithms to your simulator:

Round Robin -- The quantum size should be configurable from the command line
Shortest Job First -- This is the theoretical optimal solution
Approximate SJF -- Keep track of the average CPU burst of a task and use that as an estimate of its next CPU burst for the SJF algorithm
Create a short report (1-2 pages) with graphs comparing the four algorithms implemented so far.

# HW 8 - Memory Scheduling

Add memory to your simulator.  Use the FIFO page replacement algorithm.  The user should be able to set the time penalty for a page fault and the size of memory available in terms of pages.  Submit a report (1-2 pages) on your findings.
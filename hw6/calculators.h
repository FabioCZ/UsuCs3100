#include <vector>
#include <functional>
#include <math.h>
#include<chrono>

#ifndef CALCULATORS_H
#define CALCULATORS_H

/*
   Useful tools. Any function used as a paramater should be passed in as a lambda.
   e.g.:
   getFunctRunTimeSecs([&](){
        mean(container);
    })
 */
namespace calculators {
    struct StatData {
        double stdDev;
        double mean;
    };

    /*
        Calculates mean in any non-associative container (assuming primitive types).
    */
    template<typename Container>
    double mean(Container const &container) {
        double total = 0;

        for (auto &&i : container) {
            total += i;
        }
        return ((double) total) / ((double)container.size());
    }

    /*
     Calculates std dev in any non-associative container (assuming primitive types).
     Uses this formula:
     stdDev = sqrt ( sum( (x - mean(x)^2 ) / n -1 )
    */
    template<typename Container>
    double stdDev(Container const &container) {
        double avg = mean(container);
        double sum = 0;
        for (auto &&i : container) {
            sum += pow(i - avg, 2.0);
        }
        return sqrt(sum / (double)(container.size() - 1));
    }


    /*
        Returns runtime of a passed in function in seconds. passed in function should be lambda
     */
    template<typename Fn>
    double getFunctRunTimeSecs(Fn f) {
        using namespace std::chrono;
        high_resolution_clock::time_point timeBegin = high_resolution_clock::now();

        f();

        high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(timeEnd - timeBegin);

        return time_span.count();
    }

    /*
        Helper class to avoid looping and use recursion
     */
    template<typename Fn>
    void invokeFunctionAndGetDataHelper(int iterations, std::vector<double> &times, Fn f) {

        if (iterations < 0) return;
        times.push_back(getFunctRunTimeSecs(f));
        invokeFunctionAndGetDataHelper(iterations - 1, times, f);
    }

    /*
     Returns mean and avg of a passed in function over a number of iterations. passed in function should be lambda
    */
    template<typename Fn>
    StatData invokeFunctionAndGetData(int iterations, Fn f) {
        std::vector<double> times;
        invokeFunctionAndGetDataHelper(iterations, times, f);
        StatData data;
        data.mean = mean(times);
        data.stdDev = stdDev(times);
        return data;
    }


};

#endif

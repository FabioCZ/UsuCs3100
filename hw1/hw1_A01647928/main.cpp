//Keano F Gottlicher, A01647928, CS3100 Sp2015, HW1

#include <iostream>
#include <vector>
#include "Calculators.h"
#include <cmath>
#include <bits/stl_list.h>
#include <stdlib.h>


int main() {
    std::cout << "Creating a vector with 1,000,000 random ints" << std::endl;
    std::vector<int> test;
    //for loop only for demo purposes
    for(int i =0; i < 1000000; i++){
        test.push_back(rand());
    }

    std::cout << "Mean of this vector is: " << Calculators::mean(test) << std::endl;
    std::cout << "Std dev of this vector is: " << Calculators::stdDev(test) << std::endl;
    std::cout << "Runtime of std dev is(sec): " << Calculators::getFunctRunTimeSecs([&](){
        Calculators::mean(test);
    }) << std::endl;
    Calculators::StatData statData = Calculators::invokeFunctionAndGetData(50, [&](){
        Calculators::stdDev(test);
    });
    std::cout << "Mean of runtime for stdDev() over 50 iters(sec): " << statData.mean << std::endl;
    std::cout << "Std dev of runtime for stdDev() over 50 iters(sec): " << statData.stdDev << std::endl;


    std::cout << "Creating a vector with 1,000,000 random doubles" << std::endl;
    std::vector<double> test2;
    //for loop only for demo purposes
    for(int i =0; i < 1000000; i++){
        test2.push_back(rand());
    }

    std::cout << "Mean of this vector is: " << Calculators::mean(test2) << std::endl;
    std::cout << "Std dev of this vector is: " << Calculators::stdDev(test2) << std::endl;
    std::cout << "Runtime of std dev is(sec): " << Calculators::getFunctRunTimeSecs([&](){
        Calculators::mean(test2);
    }) << std::endl;
    Calculators::StatData statData2 = Calculators::invokeFunctionAndGetData(50, [&](){
        Calculators::stdDev(test2);
    });
    std::cout << "Mean of runtime for stdDev() over 50 iters(sec): " << statData2.mean << std::endl;
    std::cout << "Std dev of runtime for stdDev() over 50 iters(sec): " << statData2.stdDev << std::endl;
    return 0;
}







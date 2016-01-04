//#include "Calculators.h"
//#include <cmath>
//#include <chrono>
//namespace Calculators {
//    void testF(int a, int b){
//        int h =0;
//        for(int i = 0; i < 22222222; i++){
//            h +=i;
//        }
//    }
//
///*
//     Calculates std dev in any non-associative container (assuming primitive types).
//     Uses this formula:
//     stdDev = sqrt ( sum( (x - mean(x)^2 ) / n -1 )
// */
//    template<typename Container>
//    double stdDev(Container const &container) {
//        double mean = mean(container);
//        auto sum = 0;
//        for (auto &&i : container) {
//            sum += pow(i - mean, 2.0);
//        }
//        return sqrt(sum / (container.size() - 1));
//    }
//
///*
//   Calculates mean in any non-associative container (assuming primitive types).
// */
//    template<typename Container>
//    double mean(Container const &container) {
//        auto total = 0;
//
//        for (auto &&i : container) {
//            total += i;
//        }
//        return ((double) total) / container.size();
//    }
//
//
//    template <typename Fn, typename ... Args>
//    double getFunctRunTimeMillis(Fn&& f, Args&& ... args) {
//        using namespace std::chrono;
//        high_resolution_clock::time_point timeBegin = high_resolution_clock::now();
//
//        std::bind( std::forward<Fn>(f), std::forward<Args>(args)... )();
//
//        high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
//        duration<double> time_span = duration_cast<duration<double>>(timeEnd - timeBegin);
//
//        return time_span.count();
//    }
//
//    template <typename T, typename ... Args>
//    StatData invokeFunctionAndGetData(int iterations, T&& f, Args&& ... args){
//        std::vector<double> times;
//        invokeFunctionAndGetDataHelper(iterations, times, f, args...);
//        StatData data;
//        data.mean = mean(times);
//        data.stdDev = stdDev(times);
//        return data;
//    }
//
//
//    template <typename T, typename ... Args>
//    void invokeFunctionAndGetDataHelper(int iterations, std::vector<double> & times, T&& f, Args&& ... args){
//
//        if(iterations < 0) return;
//        times.push_back(getFunctRunTimeMillis(f, args...));
//        invokeFunctionAndGetDataHelper(iterations - 1, times, f, args...);
//    }
//
//
//}

//Keano F Gottlicher, A01647928, CS3100 Sp2015, HW2
#include <iostream>
#include "Mandelbrot.h"
#include "Calculators.h"

using namespace std;


int main() {
    std::ofstream csv;
    csv.open("output.csv");

    Mandelbrot::Cplx m1(-0.745, 0.231);
    Mandelbrot::Cplx m2(-0.723, 0);
    m2.im = m1.im + (m2.rl - m1.rl);
    std::vector<std::vector<Mandelbrot::Pixel>> a;
    //only doing square images to make the graphing of perfomance easier
    std::vector<std::pair<int, int>> sizes{
            std::pair<int,int>(32,32),
            std::pair<int, int>(64, 64),
            std::pair<int, int>(128, 128),
            std::pair<int, int>(192, 192),
            std::pair<int, int>(256, 256),
            std::pair<int, int>(384, 384),
            std::pair<int, int>(512, 512),
            std::pair<int, int>(768, 768),
            std::pair<int, int>(1024, 1024),
    };


    for(std::pair<int, int> i : sizes) {
        double time = Calculators::getFunctRunTimeSecs([&]() {
            a = Mandelbrot::generateMandelbrot(m1, m2, i.first, i.second);
        });
        Mandelbrot::writePpmToFile(a);
        csv << i.first << ", " << time << endl;
    }

    csv.close();
    return 0;
}
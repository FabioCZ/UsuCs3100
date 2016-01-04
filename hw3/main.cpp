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



    for(int i = 1; i < 10; i++) {
        double time = Calculators::getFunctRunTimeSecs([&]() {
                a = Mandelbrot::generateMandelbrot(m1, m2, 512, 512, i);
        });
        //Mandelbrot::writePpmToFile(a, i);
        csv << i << ", " << time << endl;
    }

    csv.close();
    return 0;
}
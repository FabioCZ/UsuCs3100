#ifndef MANDELBROT_H
#define MANDELBROT_H
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

namespace Mandelbrot{

    const int max_iters = 100;
    const int base_r = 51;
    const int base_g = 181;
    const int base_b = 229;

    struct Cplx {       //complex number structure
        double rl;
        double im;

        Cplx(double r, double i) : rl{r}, im{i}{

        }
    };

    struct Pixel{
        int red;
        int green;
        int blue;

        Pixel(){
        }
        Pixel(int r, int g, int b) : red{r}, green{g}, blue{b}{
        }
    };

    Cplx operator+(Cplx x, Cplx y){
        Cplx z(x.rl + y.rl, x.im + y.im);
        return z;
    }
    Cplx operator*(Cplx x, Cplx y){
        Cplx z(x.rl*y.rl - x.im*y.im, x.rl*y.im + y.rl*x.im);
        return z;
    }

    int iters(Cplx c){
        Cplx z(0,0);
        int i;
        for(i = 0; i<max_iters; i++){
            z=z*z+c;
            if(z.rl*z.rl+z.im*z.im > 4)break;
        }
        return i;
    }


    double getColorMult(int iter_diff){
        iter_diff--;
        if(iter_diff >= 50 ||iter_diff < 0) return 0.0;
        return 1 - (static_cast<double>(iter_diff) * 0.02);
    }

    Pixel getPixelForIters(int iters){
        Pixel clr(base_r, base_g, base_b);
        double multiplier = getColorMult(max_iters - iters);
        clr.red*=multiplier;
        clr.green*=multiplier;
        clr.blue*=multiplier;
        return clr;
    }



    std::string toPpmString(std::vector<std::vector<Pixel>> p){
        std::stringstream ppmPixel;
        for(auto i : p){
            for(auto j : i){
                ppmPixel << j.red << " " << j.green << " " << j.blue << " ";
            }
            ppmPixel << std::endl;
        }
        return ppmPixel.str();
    }

    void writePpmToFile(std::vector<std::vector<Pixel>> pixels){
        std::ofstream fout;      //filestream init
        fout.open("mandeloutput.ppm");

        fout << "P3\n#created by Fabio Gottlicher\n" << pixels.size() << " " << pixels[0].size() << "\n255\n";
        fout << toPpmString(pixels);
        fout.close();
    }

    std::vector<std::vector<Pixel>> generateMandelbrot(Cplx a, Cplx b, int height, int width){
        Cplx rs(0,0);
        int cIters;
        std::vector<std::vector<Pixel>> imageSet;
        imageSet.reserve(height);


        for(int i=0;i<height;++i) {
            std::vector<Pixel> imageSetWidth;
            imageSetWidth.reserve(width);
            for (int j = 0; j < width; ++j) {
                rs.im = a.im + (i * (b.im - a.im) / height);
                rs.rl = a.rl + (j * (b.rl - a.rl) / width);
                cIters = iters(rs);
                imageSetWidth.push_back(getPixelForIters(cIters));
            }
            imageSet.push_back(imageSetWidth);
        }
        return imageSet;
    }



}

#endif
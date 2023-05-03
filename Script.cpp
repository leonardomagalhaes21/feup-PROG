#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            }
            if (command == "replace") {
                Color cor1,cor2;
                input>> cor1;
                input>> cor2;
                replace(cor1.red(), cor1.green() , cor1.blue() , cor2.red(), cor2.green(), cor2.blue());
                continue;
            }
            if (command == "invert") {
                invert();
                continue;
            }

            if(command =="to_gray_scale"){
                to_gray_scale();
                continue;
            }
        }
    }

            
    
    void Script::invert(){      // Tranforma cada píxel (r, g, b) para (255-r,255-g,255-b).
        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image-> height(); j++){
                image->at(i,j).red()=255-image->at(i,j).red();
                image->at(i,j).green()=255- image->at(i,j).green();
                image->at(i,j).blue()=255- image->at(i,j).blue();
            }
        }
    }
    
    void Script::to_gray_scale(){ //Transforms cada píxel (r, g, b) em (v, v, v) tal que v = (r + g + b)/3.
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                rgb_value v = (image->at(i,j).red() + image->at(i,j).green() + image->at(i,j).blue())/3;
                image->at(i,j).red()= v;
                image->at(i,j).green()= v;
                image->at(i,j).blue()= v;
            }
        }
    }
    void Script::replace (rgb_value r1, rgb_value g1 , rgb_value b1 , rgb_value r2, rgb_value g2, rgb_value b2){
        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image-> height(); j++){
                if (image->at(i,j).red() == r1 && image->at(i,j).green() == g1 && image->at(i,j).blue() == b1){
                    image->at(i,j).red()= r2;
                    image->at(i,j).green()= g2;
                    image->at(i,j).blue()= b2;
                }
            }
        }
    }
    
        
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
}

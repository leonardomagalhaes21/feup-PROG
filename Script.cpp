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
                replace(rgb_value r1, rgb_value g1 , rgb_value b1 , rgb_value r2, rgb_value g2, rgb_value b2);
                continue;
}
            if (command == "invert") {
                invert();
                continue;
}
            if (command == "rotate") {
                int degrees;
                input >> degrees;
                rotate(degrees);
                continue;
}
            if (command == "resize") {
                int width, height;
                input >> width >> height;
                resize(width, height);
                continue;
            }
            
            if (command == "add"){
                add (filename ,r, g, b, x, y);
                continue;
            }
}
}
}
            
    
    void Script::invert(){      // Tranforma cada píxel (r, g, b) para (255-r,255-g,255-b).
        for (int i = 0; i < pixels.width(); i++) {
        for (int j = 0; j < height_; j++) {
            pixels[i][j].red()=255-pixels[i][j].red();
            pixels[i][j].green()=255-pixels[i][j].green();
            pixels[i][j].blue()=255-pixels[i][j].blue();
            }
        }
    }
    
    void Script::to_gray_scale(){ //Transforms cada píxel (r, g, b) em (v, v, v) tal que v = (r + g + b)/3.
        for (int i = 0; i < width(); i++){
            for (int j = 0; j < height(); j++){
                rgb_value v = (pixels[i][j].red() + pixels[i][j].green() + pixels[i][j].blue())/3;
                pixels[i][j].red()= v;
                pixels[i][j].green()= v;
                pixels[i][j].blue()= v;
            }
        }
    }
    void Script::replace (rgb_value r1, rgb_value g1 , rgb_value b1 , rgb_value r2, rgb_value g2, rgb_value b2){}
    void Script::fill (x ,y ,w ,h ,r ,g ,b){}
    void Script::h_mirror(){}
    void Script::v_mirror(){}
    void Script::add (filename ,r, g, b, x, y);
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

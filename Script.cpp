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
            if(command =="h_mirror"){
                h_mirror();
                continue;
            }
            if(command =="v_mirror"){
                v_mirror();
                continue;
            }
            if(command =="fill"){
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Color c;
                input>> c;
                fill (x,y , w , h , c.red() , c.green(), c.blue());
                continue;
            }
            if(command =="add"){
                std::string filename;
                int r;
                int g;
                int b;
                int x;
                int y;
                input>> filename;
                input>> r;
                input>> g;
                input>> b;
                input>> x;
                input>> y;
                add(filename,r,g,b,x,y);
                continue;    
           }
           if(command =="crop"){
                int x, y, w, h;
                input >> x >> y >> w >> h;
                crop (x, y, w, h);
                continue;
            }
           if(command =="rotate_left"){
                rotate_left();
                continue;
            }
            if(command =="rotate_right"){
                rotate_right();
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
    void Script::fill (int x,int y , int w , int h , rgb_value r , rgb_value g , rgb_value b){
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++){
                image->at(i,j).red()= r;
                image->at(i,j).green()= g;
                image->at(i,j).blue()= b;
            }
        }
    }
    void Script::h_mirror(){        //faz espelho da imagem horizontalmente
        for (int x=0; x< image->width()/2 ;x++){
            for (int y=0;y< image->height();y++){
                Color aux(image->at(x,y).red(),image->at(x,y).green(),image->at(x,y).blue());
                image->at(x,y).red()=image->at(image->width()-1-x,y).red();
                image->at(x,y).green()=image->at(image->width()-1-x,y).green();
                image->at(x,y).blue()=image->at(image->width()-1-x,y).blue();
                image->at(image->width()-1-x,y)=aux;
            }
        }
    }
    void Script::v_mirror(){        //faz espelho da imagem verticalmente
        for (int x=0; x< image->width() ;x++){
            for (int y=0;y< image->height()/2;y++){
                Color aux(image->at(x,y).red(),image->at(x,y).green(),image->at(x,y).blue());
                image->at(x,y).red()=image->at(x,image->height()-1-y).red();
                image->at(x,y).green()=image->at(x,image->height()-1-y).green();
                image->at(x,y).blue()=image->at(x,image->height()-1-y).blue();
                image->at(x,image->height()-1-y)=aux;
            }
        }
    }
    void Script::add (std::string &filename,int r, int g, int b, int x, int y){
        Image* imageB;
        imageB=loadFromPNG(filename);
        for (int i = 0; i < imageB->width(); i++){
            for (int j = 0; j < imageB->height(); j++){
                if ((imageB->at(i,j).red()==r) && (imageB->at(i,j).green() ==g) && (imageB->at(i,j).blue()==b)){
                    continue;
                }
                else{
                    image->at(x+i,y+j).red()=imageB->at(i,j).red(); 
                    image->at(x+i,y+j).green() =imageB->at(i,j).green(); 
                    image->at(x+i,y+j).blue()=imageB->at(i,j).blue();
                }
            }
        }
        delete imageB;
    }
    void Script::crop (int x, int y, int w, int h){ // Faz recorte da imagem
        vector<std::vector<Color>> tmp;
        Image temp (w, h);
        for (int i = x; i < x + w; i++) {
            vector<Color> l;
            for (int j = y; j < y + h; j++){
                l.push_back(image->at(i,j));
            }
            tmp.push_back(l);
        }
        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                temp.at(i,j) = tmp[i][j];
            }
        }
        *image = temp;
    }
    void Script::rotate_right() {
	vector<std::vector<Color>> tmp;
        Image temp (image->height(), image->width());
        for (int i = 0; i < image->height(); i++) {
            vector<Color> l;
            for (int j = 0; j < image->width(); j++){
                l.push_back(image->at(j,i));
            }
            tmp.push_back(l);
        }
        
        //troca a ordem das linhas    
        for (unsigned long y=0;y<tmp[y].size();y++){
            for (unsigned long x=0;x<tmp.size()/2;x++){
                swap(tmp[x][y],tmp[tmp.size()-y-1][y]);
            }
        }
        for (unsigned long j = 0; j < tmp.size(); j++){
            for (unsigned long i = 0; i < tmp[j].size(); i++){
                temp.at(j,i) = tmp[j][i];
            }
        }
          *image = temp; 
    }
    void Script::rotate_left(){
        vector<std::vector<Color>> tmp;
        Image temp (image->height(), image->width());
        for (int i = 0; i < image->height(); i++) {
            vector<Color> l;
            for (int j = 0; j < image->width(); j++){
                l.push_back(image->at(j,i));
            }
            tmp.push_back(l);
        }
        
        //troca a ordem das colunas       
        for (unsigned long x=0;x<tmp.size();x++){
            for (unsigned long y=0;y<tmp[x].size()/2;y++){
                swap(tmp[x][y],tmp[x][tmp[x].size()-y-1]);
            }
        }
        for (unsigned long j = 0; j < tmp.size(); j++){
            for (unsigned long i = 0; i < tmp[0].size(); i++){
                temp.at(j,i) = tmp[j][i];
            }
        }
          *image = temp; 
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


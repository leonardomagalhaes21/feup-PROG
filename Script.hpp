#ifndef __prog_Script_hpp__
#define __prog_Script_hpp__

#include <string>
#include <fstream>
#include "Image.hpp"

namespace prog
{
  class Script
  {
  public: 
    Script(const std::string &filename);
    ~Script();
    void run();
    // Implementação de novos métodos;
    void invert(); // Tranforma cada píxel (r, g, b) para (255-r,255-g,255-b).
    void to_gray_scale(); //Transforms cada píxel (r, g, b) em (v, v, v) tal que v = (r + g + b)/3.
    void replace (rgb_value r1, rgb_value g1 , rgb_value b1 , rgb_value r2, rgb_value g2, rgb_value b2); // Altera todos os píxeis (r1,  g1, b1) por (r2,  g2, b2).
    void fill (Color x,Color y , int w , int h , rgb_value r , rgb_value g , rgb_value b); // Preenche 
    void h_mirror();
    void v_mirror();
    void add (std::string &filename,rgb_value r, rgb_value g, rgb_value b, Color x, Color y);
  private:
    // Current image.
    Image *image;
    // Input stream for reading script commands.
    std::ifstream input;
  private:
    // Private functions
    void clear_image_if_any();
    void open();
    void blank();
    void save();
  };
}
#endif
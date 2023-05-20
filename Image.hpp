#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include <vector>
#include "Color.hpp"

namespace prog
{
  class Image
  {
  private:   // Incluir os parâmetros privados correspondentes às largura altura e píxeis da imagem                          
    int width_;   //variavel correspondente à largura da imagem
    int height_;  //variavel correspondente à altura da imagem
    std::vector<std::vector<Color>> pixels;   //vetor que vai conter os pares de posições(width_,height_), contendo todos os pontos da imagem 

  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif

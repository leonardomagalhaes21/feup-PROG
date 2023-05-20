#include "Image.hpp"


namespace prog
{
  Image::Image(int w, int h, const Color &fill)  // Construtor que coloca todos os pixeis da imagem com a cor fill
  {
    width_=w;                                  
    height_=h;
    for (int i = 0; i < w; i++) {
    std::vector<Color> linha;
      for (int j = 0; j < h; j++) {
        linha.push_back(fill);
      }
      pixels.push_back(linha);
    }
  }
  Image::~Image()       //Destruídor (Não utilizado)
  {
  }

  // Métodos que retornam o valor dos parâmetros largura e altura

  int Image::width() const
  {
    return width_;
  }
  int Image::height() const
  {
    return height_;
  }

  // Método que retorna referência à cor de um determinado pixel com largura x e altura y
  Color& Image::at(int x, int y)
  {
    return pixels[x][y];
  }

  // Método que retorna a cor de um determinado pixel com largura x e altura y
  const Color& Image::at(int x, int y) const
  {
    return pixels[x][y];
  }
}
#include "Color.hpp"

namespace prog {
    Color::Color() {  // Construtor default que inicia todos os parâmetros para 0;
	red_=0;
	green_=0;
	blue_=0;
    }
    Color::Color(const Color& other) { // Construtor copia os parâmetros de outra variável do tipo Color
	this->red_=other.red_;
	this->green_=other.green_;
	this->blue_=other.blue_;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) { // Construtor que altera as variáveis para os argumentos da função
	red_=red;
	green_=green;
	blue_=blue;
    }

    // Métodos que retornam o valor de cada parâmetro

    rgb_value Color::red() const {
        return red_;
    }
    rgb_value Color::green() const {
        return green_;
    }
    rgb_value Color::blue() const {
        return blue_; 
    }

    // Métodos que retornam referências aos parâmetros individuais RGB (para que possam ser alterados)

    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
        return green_;
    }
    rgb_value& Color::blue()  {
        return blue_;
    }
}
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <set>
#include <iomanip>
#include <unordered_map>
#include "XPM2.hpp" // Include the XPM2.hpp header file

namespace prog {
        struct ColorHash { 
        size_t operator()(const Color& c) const
        {
            return ( c.red() << 16) | (c.green()<< 8) | c.blue(); //calcula o valor hash para a cor dando shift left 16 bits na componente vermelha e 8 bits para a esquerda na componente da cor verde, e depois faz-se OR com a componente da cor azul
        }
    };
    struct ColorEqual {
        bool operator()(const Color& a, const Color& b) const //compara duas cores, comparando cada componente da cor individualmente, todas as parcelas forem iguais as cores são consideradas iguais
        {
            return a.red() == b.red() 
                && a.green() == b.green() 
                && a.blue() == b.blue();
        }
    };


Image* loadFromXPM2(const std::string& file) {
    std::ifstream input(file);       //abre o ficheiro para leitura, dá o nome input
    if (!input.is_open()) {     //verifica se o ficheiro não abriu
        std::cerr << "Failed to open file: " << file << std::endl;      //se não abiriu, emite uma mensagem a avisar
        return nullptr;     //retorna nada
    }
    std::string line;        //cria uma variavel para armazenar uma linha do ficheiro 
    getline(input, line);       //ignorar o header
    getline(input, line);       //recebe na line a segunda linha, corresponde à w,h,n,c
    int w,h,n;      //define w,h,n como int para receber valores de line
    std::istringstream iss(line);        //associa line à variavel istringstream iss
    iss>>w>>h>>n;       //cada variavel(w,h,n) recebe o respetivo valor
    Image* temp = new Image(w, h);      //cria um ponteiro para uma imagem chamada temp

    std::map<char,Color> mapa;       //cria um mapa onde o primeiro elemento é do tipo char e o segundo é do tipo Color
    for(int a=0;a<n;a++){       //cria um for do tamanho de n, para que possa ler os simbolos todos e adiciona-los ao mapa
        getline(input,line);    //coloca a próxima linha do ficheiro input em line
        std::istringstream iss(line);    //cria uma istringstream com o conteúdo de line
        char simbolo;       //cria a variavel simbolo, para que possa receber o simbolo que irá codificar a cor
        std::string c,hexadecimal;   //cria a variavel c(ignorada) e a variavel hexadecimal, que é usada para armazenar o código da cor
        iss>> simbolo;      //atribui o caracter ao símbolo
        iss>>c;             //atribui o caracter c à variavel c (para ignorá-lo) 
        iss>>hexadecimal;   //atribui a sequencia em hexadecimal à variavel hexadecimal
        std::string pri,sec,ter; //cria tres strings para dividir a sequencia hexadecimal em tres, correspondentes às variaveis(r,g,b)
        rgb_value r,g,b;    //cria as variaveis, para que possa armazenar depois da sequencia ser transformada em inteiro 
        pri=hexadecimal.substr(1,2);    //a pri é atribuida a sequencia começada no index 1 (ignora "#") e a sequencia vai consumir 2 caracteres da variavel hexadecimal
        r=stoi(pri,nullptr,16);         //atribui a r o valor correspondente à cor vermelha
        sec=hexadecimal.substr(3,2);    //a sec é atribuida a sequencia começada no index 3 e a sequencia vai consumir 2 caracteres da variavel hexadecimal
        g=stoi(sec,nullptr,16);         //atribui a g o valor correspondente à cor verde
        ter=hexadecimal.substr(5,2);    //a ter é atribuida a sequencia começada no index 5 e a sequencia vai consumir 2 caracteres da variavel hexadecimal
        b=stoi(ter,nullptr,16);         //atribui a b o valor correspondente à cor azul
        Color cor (r,g,b);              //cria a variavel cor com os argumentos r,g e b previamente definidos
        mapa[simbolo]=cor;              //adiciona ao mapa, codificando o respetivo simbolo à respetiva cor
    }
    for (int y=0;y<h;y++){      //ciclo for para percorrer a altura da imagem(e do ficheiro), e consumir todos os simbolos
        getline(input,line);    //adiciona a line uma nova linha do ficheiro, todas estas com simbolos para serem consumidos e descodificados em cores
        for (int x=0;x<w;x++){  //ciclo for para percorrer a largura da imagem(e do ficheiro), e consumir todos os simbolos
            Color cor=mapa[line[x]];     //cria a variavel cor, onde é armazena a cor correspondente ao simbolo lido no ficheiro e contido no mapa 
            temp->at(x,y).red()=cor.red();      //atribui a cada par (x,y) na imagem a cor vermelha que foi lida no mapa
            temp->at(x,y).green()=cor.green();  //atribui a cada par (x,y) na imagem a cor verde que foi lida no mapa
            temp->at(x,y).blue()=cor.blue();    //atribui a cada par (x,y) na imagem a cor azuk que foi lida no mapa
        }
    }
    input.close();      //fecha a leitura do ficheiro
    return temp;        //retorna o ponteiro para a imagem temp
}

void saveToXPM2(const std::string &file, const Image *image)
    {
        std::ofstream output(file); //abre o ficheiro para escrita, dá o nome output
        Color alist['~' - '!']; //cria um array com o tamanho de todos os "simbolos" possiveis para armazenar as cores nos enderessos dos simbolos
        int width = image->width(); //recebe a largura da imagem e associa à variavel width
        int height = image->height(); //recebe a altura da imagem e associa à variavel height
        unsigned char simbolo1 = '!'; //associa à variael simbolo1 o simbolo inicial (simbolo que será associado à primeira cor do ficheiro)
        std::unordered_map<Color,char,ColorHash,ColorEqual> mapa; //cria um unordered map em que as keys são as cores e os objetos são os simbolos. Nos dois parametros seguintes são mencionados structs que contêm operadores, uma delas(ColorHash) calcula os valores hash das cores e outra (ColorEqual) determina se duas cores são iguais. Basicamente servem para que o map consiga funcionar com as chaves do tipo Color.
        for (int i = 0; i < height; i++) //ciclo for para percorrer a altura da imagem
        {
            for (int j = 0; j < width; j++)//ciclo for para percorrer a largura da imagem
            {
                Color color = image->at(j, i); //a variavel color assume a cor do pixel atual da imagem 
                if (mapa.find(color) == mapa.end()) //se a cor atual não se encontrar no mapa
                {
                    mapa.insert({color, simbolo1}); // adiciona-se a cor ao mapa e assume-se um simbolo a essa cor (a cor é a key e o simbolo é o objeto)
                    alist[simbolo1 - '!'] = color; //no indice do simbolo atual (primeiro simbolo indice 0, segundo simbolo indice 1...) associar uma cor unica a esse simbolo
                    simbolo1++; //incrementar o simbolo (mudar o simbolo para que a proxima cor tenha um simbolo novo)
                }
            }
        }

        output << "! XPM2" << std::endl; // header
        output << width << " " << height << " " << mapa.size() << " " //segunda linha do ficheiro que contem as caracteristicas do ficheiro: largura, altura, numero de cores (que é o tamanho do mapa) e o número de caracteres por cor, que é um por definição
               << "1"
               << "\n";

        for (char car = '!'; car < simbolo1; car++) //para cada cor (que é desde o simbolo inicial('!') até ao simbolo atual) escreve-se uma linha com o simbolo e a respetiva cor
        {
            Color color = alist[car - '!']; //associa uma cor dependendo do simbolo selecionado
            output << car << " c #" //dá output do simbolo e de seguida a respetiva cor em hexadecimal
                << std::hex << std::uppercase 
                << std::setw(2) << std::setfill('0')
                << (int) color.red() 
                << std::setw(2) << std::setfill('0')
                << (int) color.green() 
                << std::setw(2) << std::setfill('0')
                << (int) color.blue() 
                << std::endl;   } //converte as diferentes cores para hexadecimal, preechendo os espaços caso o valor tenha zeros à esquerda


        for (int i = 0; i < height; i++) //ciclo for para percorrer a altura da imagem
        {
            for (int j = 0; j < width; j++)//ciclo for para percorrer a largura da imagem
            {
                Color color = image->at(j, i); //a variavel color assume a cor do pixel atual da imagem 
                output << mapa.at(color); //escreve o simbolo que está associado à chave (cada simbolo está associado a apenas uma cor)
            }
            output << "\n";// faz um paragrafo novo a cada fim de cada linha
        }

        output.close(); //fecha a escrita do ficheiro
    }
}
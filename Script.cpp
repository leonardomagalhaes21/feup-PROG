#include <iostream>
#include <fstream>
#include <algorithm>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"


namespace prog {
    // Use to read color values from a script file.
    std::istream& operator>>(std::istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const std::string& filename) :
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
        std::string command;
        while (input >> command) {
            std::cout << "Executing command '" << command << "' ..." << std::endl;
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

            //Comandos para aceder cada uma das funções definidas
            //Recebemos os inputs necessários para fornecer os argumentos a cada uma das funções

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
            if(command =="median_filter"){
                int ws;
                input >> ws;
                median_filter(ws);
                continue;
            }
            if(command =="xpm2_open"){
                std::string file;
                input>>file;
                clear_image_if_any();
                image=loadFromXPM2(file);
            }
            if(command =="xpm2_save"){
                std::string file;
                input>>file;
                saveToXPM2(file,image);
            }

        }
    }

            
    
    void Script::invert(){      // Tranforma cada píxel (r, g, b) para (255-r,255-g,255-b).
        for (int i = 0; i < image->width(); i++) {      //2 Loops para percorrer todas as posições da imagem
            for (int j = 0; j < image-> height(); j++){
                image->at(i,j).red()=255-image->at(i,j).red();      //troca a cor de cada uma das posições percorridas, pela sua oposta (255-red,255-green,255-blue)
                image->at(i,j).green()=255- image->at(i,j).green();
                image->at(i,j).blue()=255- image->at(i,j).blue();
            }
        }
    }
    
    void Script::to_gray_scale(){ //Transforma cada píxel (r, g, b) em (v, v, v) tal que v = (r + g + b)/3.
        for (int i = 0; i < image->width(); i++){       //2 Loops para percorrer todas as posições da imagem
            for (int j = 0; j < image->height(); j++){
                rgb_value v = (image->at(i,j).red() + image->at(i,j).green() + image->at(i,j).blue())/3;    //soma os inteiros correspondentes às 3 cores (r,g,b), dividindo o resultado por 3 e armazenando em v
                image->at(i,j).red()= v;        // substitui cada uma das variaveis (r,g,b) de cada posição por v
                image->at(i,j).green()= v;
                image->at(i,j).blue()= v;
            }
        }
    }
    void Script::replace (rgb_value r1, rgb_value g1 , rgb_value b1 , rgb_value r2, rgb_value g2, rgb_value b2){
        for (int i = 0; i < image->width(); i++) { //2 Loops para percorrer todas as posições da imagem
            for (int j = 0; j < image-> height(); j++){
                // caso a cor do píxel corresponda a (r1,g1,b1) substituir a cor por (r2,g2,b2)
                if (image->at(i,j).red() == r1 && image->at(i,j).green() == g1 && image->at(i,j).blue() == b1){
                    image->at(i,j).red()= r2;
                    image->at(i,j).green()= g2;
                    image->at(i,j).blue()= b2;
                }
            }
        }
    }
    void Script::fill (int x,int y , int w , int h , rgb_value r , rgb_value g , rgb_value b){
        for (int i = x; i < x + w; i++) { //2 Loops para percorrer todas as posições do retângulo
            for (int j = y; j < y + h; j++){
                // muda a cor do píxel para os valores rgb pretendidos
                image->at(i,j).red()= r;
                image->at(i,j).green()= g;
                image->at(i,j).blue()= b;
            }
        }
    }
    void Script::h_mirror(){        //espelha a imagem horizontalmente
        for (int x=0; x< image->width()/2 ;x++){    //Só percorre metade da largura, pois vai trocar os extremos um pelo outro, e as posições seguintes igualmente,(exemplo: cor da primeira posição da linha troca com a cor da última)
            for (int y=0;y< image->height();y++){   //2 Loops para percorrer metade da largura e a altura, invertendo todas as linhas
                Color aux(image->at(x,y).red(),image->at(x,y).green(),image->at(x,y).blue());   //guarda a cor da posição
                image->at(x,y).red()=image->at(image->width()-1-x,y).red();     //troca a cor da posição guardada, pela cor da posição oposta na linha(fim-x,y)
                image->at(x,y).green()=image->at(image->width()-1-x,y).green(); //o "-1" tem a ver com o index, pois width dá o size começando a contar em 1 e o index começa com 0
                image->at(x,y).blue()=image->at(image->width()-1-x,y).blue();
                image->at(image->width()-1-x,y)=aux;    //atribui a cor guardada à posição oposta na linha (fim-x,y)
            }
        }
    }
    void Script::v_mirror(){        //faz espelho da imagem verticalmente
        for (int x=0; x< image->width() ;x++){          //2 Loops para percorrer a largura e metade da altura, invertendo todas as linhas
            for (int y=0;y< image->height()/2;y++){     //Só percorre metade da altura, pois vai trocar os extremos um pelo outro, e as posições seguintes igualmente,(exemplo: cor da primeira posição da coluna troca com a cor da última)
                Color aux(image->at(x,y).red(),image->at(x,y).green(),image->at(x,y).blue());
                image->at(x,y).red()=image->at(x,image->height()-1-y).red();        //troca a cor da posição guardada, pela cor da posição oposta na coluna(x,fim-y)
                image->at(x,y).green()=image->at(x,image->height()-1-y).green();    //o "-1" tem a ver com o index, pois height dá o size começando a contar em 1 e o index começa com 0
                image->at(x,y).blue()=image->at(x,image->height()-1-y).blue();
                image->at(x,image->height()-1-y)=aux;       //atribui a cor guardada à posição oposta na coluna (x,fim-y)
            }
        }
    }
    void Script::add (std::string &filename,int r, int g, int b, int x, int y){
        Image* imageB;// pointer para um objeto Image
        imageB=loadFromPNG(filename); // dá load a um arquivo png para imageB
        for (int i = 0; i < imageB->width(); i++){ // percorre todos os píxeis de imageB
            for (int j = 0; j < imageB->height(); j++){
                if ((imageB->at(i,j).red()==r) && (imageB->at(i,j).green() ==g) && (imageB->at(i,j).blue()==b)){ // verifica se o píxel tem a cor (r,g,b)
                    continue; // caso isso aconteça passa para o píxel seguinte
                }
                else{ // caso isso não aconteça, coloca os valores de cor do pixel na imagem movendo x e y
                    image->at(x+i,y+j).red()=imageB->at(i,j).red(); 
                    image->at(x+i,y+j).green() =imageB->at(i,j).green(); 
                    image->at(x+i,y+j).blue()=imageB->at(i,j).blue();
                }
            }
        }
        delete imageB; // liberta a memória de imageB
    }
    void Script::crop (int x, int y, int w, int h){ 
        std::vector<std::vector<Color>> tmp; // define um novo vetor (representa uma matriz) receber os píxeis da imagem recortada
        Image temp (w, h); // cria uma nova imagem com as dimensões pretendidas para o recorte
        for (int i = x; i < x + w; i++) { // percorre os píxeis das linhas
            std::vector<Color> l; // vetor que contém os píxeis da linha atual
            for (int j = y; j < y + h; j++){ // percorre os píxeis das colunas
                l.push_back(image->at(i,j)); // adiciona o píxel ao vetor da linha atual
            }
            tmp.push_back(l); // adicona a linha atual à matriz 
        }
        for (int i = 0; i < w; i++){ // percorre os píxeis da matriz 
            for (int j = 0; j < h; j++){
                temp.at(i,j) = tmp[i][j]; // define os píxeis da nova imagem (recortada) através da matriz
            }
        }
        *image = temp; // O ponteiro de image passa a apontar para a imagem temp (recortada)
    }
    void Script::rotate_right() {       //Roda a imagem 90 graus para a direita
	std::vector<std::vector<Color>> tmp;        //define um novo vetor receber as coordenadas da imagem (como se fosse uma matriz da imagem)
        Image temp (image->height(), image->width());   //Cria uma nova imagem temporária
        for (int i = 0; i < image->height(); i++) {     //2 Loops para percorrer toda imagem e copiá-la para o vector tmp
            std::vector<Color> l;
            for (int j = 0; j < image->width(); j++){
                l.push_back(image->at(j,i));
            }
            tmp.push_back(l);
        }
        
        //Faz a transposta da matriz e inverte a ordens das linhas(um h_mirror no vector temp)
             for (unsigned long x=0;x<tmp.size()/2;x++){
             for (unsigned long y=0;y<tmp[x].size();y++){
                 std::swap(tmp[x][y],tmp[tmp.size()-x-1][y]);
             }
         }
        //percorre todas as coordenadas de tmp e passa-as para a imagem temp
        for (unsigned long j = 0; j < tmp.size(); j++){
            for (unsigned long i = 0; i < tmp[j].size(); i++){
                temp.at(j,i) = tmp[j][i];
            }
        }
          *image = temp;    //O ponteiro de image passa a apontar para a imagem temp
    }
    void Script::rotate_left(){     //Roda a imagem 90 graus para a esquerda
        std::vector<std::vector<Color>> tmp;    //define um novo vetor receber as coordenadas da imagem (como se fosse uma matriz da imagem)
        Image temp (image->height(), image->width());   //Cria uma nova imagem temporária
        for (int i = 0; i < image->height(); i++) {     //2 Loops para percorrer toda imagem e copiá-la para o vector tmp
            std::vector<Color> l;
            for (int j = 0; j < image->width(); j++){   
                l.push_back(image->at(j,i));
            }
            tmp.push_back(l);
        }
        
        //Faz a transposta da matriz e inverte a ordens das colunas(um v_mirror no vector temp)
        for (unsigned long x=0;x<tmp.size();x++){
            for (unsigned long y=0;y<tmp[x].size()/2;y++){
                std::swap(tmp[x][y],tmp[x][tmp[x].size()-y-1]);
            }
        }
        //percorre todas as coordenadas de tmp e passa-as para a imagem temp
        for (unsigned long j = 0; j < tmp.size(); j++){
            for (unsigned long i = 0; i < tmp[0].size(); i++){
                temp.at(j,i) = tmp[j][i];
            }
        }
          *image = temp;    //O ponteiro dr image passa a apontar para a imagem temp
    }
    void Script::median_filter(int ws){
        // inicializa as variàveis mediana das componetes rgb
        int mr = 0;
        int mg = 0;
        int mb = 0;
        
        Image tmp = *image; // cria imagem auxiliar (cópia da imagem original) que não vai ser alterada ao longo da função
        int szi = image->width(); // variável correspondente à largura da imagem
        int szj = image->height(); // variável correspondente à altura da imagem
        
        //percorre os píxeis da imagem
        for (int i = 0; i < szi; i++){
            for (int j = 0; j < szj; j++){
                int mov = ws / 2; // variável correspondente ao movimento máximo na vizinhança do ponto atual
                
                // cria os vetores das componetes rgb
                std::vector<int> rs; 
                std::vector<int> gs;
                std::vector<int> bs;
                

                // percorre a vizinhança do ponto atual
                for (int it = std::max(i - mov, 0) ; it <= std::min(i + mov, szi-1); it++){
                    for (int jt = std::max(j - mov,0) ; jt <= std::min(j + mov, szj-1) ; jt++){
                        // adiciona cada componente rgb ao vetor respetivo
                        rs.push_back(tmp.at(it, jt).red());
                        gs.push_back(tmp.at(it, jt).green());
                        bs.push_back(tmp.at(it, jt).blue());
                    }
                }

                // dá sort aos vetores utilizando o algoritmo sort da STL
                sort(rs.begin(), rs.end());
                sort(gs.begin(), gs.end());
                sort(bs.begin(), bs.end());

                // fazer a mediana de cada vetor tendo em conta o seu número de elementos

                if (rs.size()%2 == 1) mr = rs[rs.size()/2];       // caso do vetor ter tamanho ímpar 
                else mr = (rs[rs.size()/2 -1] + rs[rs.size()/2])/2;      // caso do vetor ter tamanho par
                

                if (gs.size()%2 == 1) mg = gs[gs.size()/2];       // caso do vetor ter tamanho ímpar 
                else mg = (gs[gs.size()/2 -1] + gs[gs.size()/2])/2;      // caso do vetor ter tamanho par
        

                if (bs.size()%2 == 1) mb = bs[bs.size()/2];       // caso do vetor ter tamanho ímpar
                else mb = (bs[bs.size()/2 -1] + bs[bs.size()/2])/2;      // caso do vetor ter tamanho par
                

                
                Color k(mr, mg, mb); // cria cor com as medianas das compontentes rgb

                image->at(i, j) = k; // atualiza a imagem principal substituindo cada píxel pela cor obtida
            }    
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        std::string filename;
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
        std::string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
}
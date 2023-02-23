#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char **argv)
{
    //Criando variaveis
    cv::Mat img;
    cv::Mat img_gray;
    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem cinza", cv::WINDOW_NORMAL);

    //Carregando e mostrando imagem
    img = cv::imread("lena.png", cv::IMREAD_COLOR);
    img_gray = cv::imread("lena.png", cv::IMREAD_GRAYSCALE);
    cv::imshow("Imagem", img);
    cv::imshow("Imagem cinza", img_gray);

    //Infos das imagens
    auto size = img.size();
    auto width = img.size[0];
    auto height = img.size[1];

    auto type_normal = img.type();
    auto ch_normal = img.channels();

    auto type_gray = img_gray.type();
    auto ch_gray = img_gray.channels();

    std::cout << "Info da imagem" << std::endl;
    std::cout << "Tamanho da imagem: " << size << std::endl;
    std::cout << "Num de linhas: " << width << std::endl;
    std::cout << "Num de colunas: " << height << std::endl;

    std::cout << std::endl <<"Foto normal" << std::endl;
    std::cout << "Tipo de dados: " << type_normal << std::endl;
    std::cout << "Num de canais: " << ch_normal << std::endl;

    std::cout << std::endl <<"Foto cinza" << std::endl;
    std::cout << "Tipo de dados: " << type_gray << std::endl;
    std::cout << "Num de canais: " << ch_gray << std::endl;

    cv::waitKey(0);

    cv::destroyAllWindows();


    return 0;
}
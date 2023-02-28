#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
    cv::Mat img;
    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);

    //Criando imagem verde
    img = cv::Mat(300, 500, CV_8UC3, cv::Scalar(40, 180, 70));

    //Vetor que armazena os pontos
    std::vector<cv::Point> points;

    //Desenhando o losango
    points.push_back(cv::Point(img.cols * 0.1, img.rows / 2));
    points.push_back(cv::Point(img.cols / 2, img.rows * 0.1));
    points.push_back(cv::Point(img.cols * 0.9, img.rows / 2));
    points.push_back(cv::Point(img.cols * 0.1, img.rows / 2));
    points.push_back(cv::Point(img.cols / 2, img.rows * 0.9));
    points.push_back(cv::Point(img.cols * 0.9, img.rows / 2)); 

    cv::fillConvexPoly(img, points, cv::Scalar(0, 255, 255));

    //Desenhando o Circulo
    auto radius = 80;
    cv::circle(img, cv::Point(img.cols / 2, img.rows / 2), radius, cv::Scalar(255, 0, 0), -1);

    //Desenhando linha
    cv::line(img, cv::Point(img.cols / 2 - radius, img.rows / 2 - img.rows * 0.07), cv::Point(img.cols / 2 + radius, img.rows / 2 + img.rows * 0.01), cv::Scalar(255, 255, 255), 15);


    cv::imshow("Imagem", img);

    cv::waitKey(0);
    cv::destroyAllWindows();

    cv::imwrite("Brasil.png", img);

    return 0;
}
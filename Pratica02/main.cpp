#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
    //Criando variaveis
    cv::Mat img;
    cv::Mat img_gray;
    cv::Mat img_gray_modified;
    cv::Mat img_hsv;
    cv::Mat img_white;
    cv::Mat img_resized;

    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem Cinza", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem Cinza Modificada", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem HSV", cv::WINDOW_NORMAL);

    //Carregando e convertendo
    img = cv::imread("semear.png", cv::IMREAD_COLOR);

    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img, img_hsv, cv::COLOR_BGR2HSV);

    img_gray_modified = img_gray.clone();
    for (auto i = 0; i < img_gray.size[0]; i++)
        for (auto j = 0; j < img_gray.size[1]; j++)
            img_gray_modified.at<uint8_t>(i, j) *= 0.5;

    img_white = cv::Mat(300, 300, CV_8UC3, cv::Scalar(255,255,255));
    cv::resize(img_white, img_resized, cv::Size(500,500));
    for (auto i = 0; i < img_resized.size[0]; i++)
        for (auto j = 0; j < img_resized.size[1]; j++)
            img_resized.at<cv::Vec3b>(i, j) = cv::Vec3b(255, rand()%255, rand()%255);

    //Mostrando primeira parte
    cv::imshow("Imagem", img);
    cv::imshow("Imagem Cinza", img_gray);
    cv::imshow("Imagem Cinza Modificada", img_gray_modified);
    cv::imshow("Imagem HSV", img_hsv);

    cv::waitKey(0);

    cv::destroyAllWindows();

    //Mostrando segunda parte
    cv::namedWindow("Imagem branca", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem redimensionada", cv::WINDOW_NORMAL);

    cv::imshow("Imagem branca", img_white);
    cv::imshow("Imagem redimensionada", img_resized);

    cv::waitKey(0);

    cv::destroyAllWindows();

    cv::imwrite("Pratica03.png", img_resized);


    return 0;
}
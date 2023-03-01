#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#define CV_PI   3.1415926535897932384626433832795

void detect_lines(cv::Mat img, cv::Mat img_edge, std::vector<cv::Vec4i> lines);
void detect_circles(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> circles);

int main(int argc, char **argv)
{
    cv::Mat img, img_gray, img_edge, img_res;

    namedWindow("Imagem", cv::WINDOW_NORMAL);
    namedWindow("Imagem cinza", cv::WINDOW_NORMAL);
    namedWindow("Canny", cv::WINDOW_NORMAL);
    namedWindow("Resultado", cv::WINDOW_NORMAL);

    img = cv::imread("Moedas e linhas.jpeg");
    cv::resize(img, img, cv::Size(720, 900));

    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    //Transforma a imagem em binária e aplica blur
    cv::Canny(img_gray, img_edge, 150, 300, 3);
    cv::GaussianBlur(img_edge, img_edge, cv::Size(3,3), 0, 0);

    //Copia a imagem
    img.copyTo(img_res);

    //Cria vetores
    std::vector<cv::Vec4i> lines;
    std::vector<cv::Vec3f> circles;

    //Aplica a transformada de Hough
    detect_lines(img_res, img_edge, lines);
    detect_circles(img_res, img_gray, circles);

    //Mostra as imagens
    cv::imshow("Imagem", img);
    cv::imshow("Imagem cinza", img_gray);
    cv::imshow("Canny", img_edge);
    cv::imshow("Resultado", img_res);

    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}


//Deteca as linhas com a transformada de Hough e contorna as linhas
void detect_lines(cv::Mat img, cv::Mat img_edge, std::vector<cv::Vec4i> lines)
{
    cv::HoughLinesP(img_edge, lines, 1, CV_PI / 180, 50, 50, 10);
    for(auto i = 0; i < lines.size(); i++)
        cv::line(img, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0, 255), 3);
}

//Deteca os circulos com a transformada de Hough e circula elas
void detect_circles(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> circles)
{
    cv::HoughCircles(img_gray, circles, cv::HOUGH_GRADIENT, 1, img_gray.rows / 16, 100, 40, 20, 130);
    for(auto i = 0; i < circles.size(); i++)
    {
        auto x = circles[i][0];
        auto y = circles[i][1];
        auto radius = circles[i][2];

        cv::circle(img, cv::Point(x, y), radius, cv::Scalar(255, 0, 0), 10);
        cv::circle(img, cv::Point(x, y), 3, cv::Scalar(255, 0, 0), 3);
    }
}
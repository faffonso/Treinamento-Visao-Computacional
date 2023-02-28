#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat img;
cv::Mat img_modified;

int val_avg, val_gau, val_med, val_bil;

void Average_Filter(int, void *);
void Gaussian_Filter(int, void *);
void Median_Filter(int, void *);
void Bilateral_Filter(int, void *);

int main(int argc, char **argv)
{
    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem modificada", cv::WINDOW_NORMAL);

    img = cv::imread("painel.png");

    //Move imagem e altera seu tamanho
    cv::moveWindow("Imagem", 700, 500);

    //Modifica o tamanho
    cv::resizeWindow("Imagem modificada", cv::Size(150, 150));
    cv::cvtColor(img ,img , cv::COLOR_BGR2GRAY);

    //Cria trackbars em uma imagem
    cv::createTrackbar("Average filter", "Imagem modificada", &val_avg, 15, Average_Filter);
    cv::createTrackbar("Gaussian filter", "Imagem modificada", &val_gau, 15, Gaussian_Filter);
    cv::createTrackbar("Median filter", "Imagem modificada", &val_med, 15, Median_Filter);
    cv::createTrackbar("Bilateral filter", "Imagem modificada", &val_bil, 15, Bilateral_Filter);

    cv::imshow("Imagem", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

void Average_Filter(int, void *)
{
    int i;
    i = 2 * val_avg + 1;
    cv::blur(img, img_modified, cv::Size(i, i));
    cv::imshow("Imagem modificada", img_modified);
}

void Gaussian_Filter(int, void *)
{
    int i;
    i = 2 * val_avg + 1;
    cv::GaussianBlur(img, img_modified, cv::Size(i, i), 0, 0);
    cv::imshow("Imagem modificada", img_modified);
}

void Median_Filter(int, void *)
{
    int i;
    i = 2 * val_med + 1;
    cv::medianBlur(img, img_modified, i);
    cv::imshow("Imagem modificada", img_modified);
}

void Bilateral_Filter(int, void *)
{
    int i;
    i = 2 * val_bil + 1;
    cv::bilateralFilter(img, img_modified, i, i*2, i/2);
    cv::imshow("Imagem modificada", img_modified);
}
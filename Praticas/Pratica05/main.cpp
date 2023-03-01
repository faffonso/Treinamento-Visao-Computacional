#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

void callback(int event, int x, int y, int flags, void *user_data);

int main(int argc, char **argv)
{
    cv::Mat img, dst, transf;

    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem Transformada", cv::WINDOW_NORMAL);

    img = cv::imread("triangle.jpg");
    cv::setMouseCallback("Imagem", callback, NULL);

    cv::Point2f img_points[3];
    img_points[0] = cv::Point2f(60, 470);
    img_points[1] = cv::Point2f(296, 66);
    img_points[2] = cv::Point2f(524, 471);

    cv::circle(img, img_points[0], 10, cv::Scalar(0, 255, 0), 15);
    cv::circle(img, img_points[1], 10, cv::Scalar(0, 0, 255), 15);
    cv::circle(img, img_points[2], 10, cv::Scalar(255, 0, 0), 15);

    cv::Point2f transf_points[3];
    transf_points[0] = cv::Point2f(249, 503);
    transf_points[1] = cv::Point2f(443, 224);
    transf_points[2] = cv::Point2f(80, 137);

    transf = cv::getAffineTransform(img_points, transf_points);
    cv::warpAffine(img, dst, transf, img.size());

    cv::imshow("Imagem", img);
    cv::imshow("Imagem Transformada", dst);

    cv::Mat img2, dst2, transf2;

    img2 = cv::imread("carta.jpeg");

    resize(img2, img2, cv::Size(500, 500));

    cv::Point2f img_points2[4];
    img_points2[0] = cv::Point2f(98, 307);
    img_points2[1] = cv::Point2f(167, 139);
    img_points2[2] = cv::Point2f(374, 135);
    img_points2[3] = cv::Point2f(449, 301);

    cv::circle(img2, img_points2[0], 5, cv::Scalar(0, 255, 0), 1);
    cv::circle(img2, img_points2[1], 5, cv::Scalar(0, 255, 0), 1);
    cv::circle(img2, img_points2[2], 5, cv::Scalar(0, 255, 0), 1);
    cv::circle(img2, img_points2[3], 5, cv::Scalar(0, 255, 0), 1);

    cv::Point2f transf_points2[4];
    transf_points2[0] = cv::Point2f(120, 400);
    transf_points2[1] = cv::Point2f(120, 50);
    transf_points2[2] = cv::Point2f(440, 50);
    transf_points2[3] = cv::Point2f(440, 400);


    transf2 = cv::getPerspectiveTransform(img_points2, transf_points2);
    cv::warpPerspective(img2, dst2, transf2, img2.size());

    cv::imshow("Imagem 2", img2);
    cv::imshow("Imagem Transformada 2", dst2);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

void callback(int event, int x, int y, int flags, void *user_data)
{
    if(event ==cv::EVENT_LBUTTONDOWN)
        std::cout << ": (" << x << ", " << y << std::endl;
}
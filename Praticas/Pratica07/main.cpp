#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

void tracking(cv::Mat img, cv::Mat img_binary);

int main(int argc, char **argv)
{
    cv::VideoCapture capture("geometria.mp4");

    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    cv::namedWindow("Video HSV", cv::WINDOW_NORMAL);
    cv::namedWindow("Control", cv::WINDOW_NORMAL);
    cv::namedWindow("Mask", cv::WINDOW_NORMAL);
    
    if(!capture.isOpened())
    {
        std::cout << "[ERROR] Video not opened" << std::endl;
        return -1;
    }    

    //Limites da cor das features
    auto h_low = 97, s_low = 80, v_low = 0;
    auto h_high = 162, s_high = 255, v_high = 255;

    //Utilizacao para encontrar a combinacao correta
    cv::createTrackbar("h_Low", "Control", &h_low, 179);
    cv::createTrackbar("s_low", "Control", &h_low, 179);
    cv::createTrackbar("v_low", "Control", &v_low, 255);
    cv::createTrackbar("h_high", "Control", &h_high, 255);
    cv::createTrackbar("s_high", "Control", &s_high, 255);
    cv::createTrackbar("v_high", "Control", &v_high, 255);

    while(1)
    {
        cv::Mat frame, hsv, threshold;
        capture.read(frame);

        if(frame.empty())
        {
            std::cout << "[INFO] Video empty" << std::endl;
            break;
        }

        //Transformacao da imagem para hsv
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), threshold);

        //Aplicando transformacao
        cv::erode(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
        cv::dilate(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

        //Calcular a centroide das features
        tracking(frame, threshold);

        cv::imshow("Video", frame);
        cv::imshow("Video HSV", hsv);
        cv::imshow("Mask", threshold);

        if(cv::waitKey(100) == 'q')
            break;

    }

    cv::destroyAllWindows();

    return 0;
}

void tracking(cv::Mat img, cv::Mat img_binary)
{
    std::vector<std::vector<cv::Point> > points;
    std::vector<cv::Vec4i> masks;

    //Contorno das features
    cv::findContours(img_binary, points, masks, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

    //Calculo dos momentos
    std::vector<cv::Moments> m(points.size());
    for(auto i = 0; i < points.size(); i++)
        m[i] = cv::moments(points[i]);

    //Calculos dos centroides
    std::vector<cv::Point2f> centroids(points.size());
    for(auto i = 0; i < points.size(); i++)
    {
        auto x = m[i].m10 / m[i].m00;
        auto y = m[i].m01 / m[i].m00;
        centroids[i] = cv::Point2f(x, y);
    }

    //Desenha o contorno e os circulos
    for(auto i = 0; i < points.size(); i++)
    {
        if(cv::contourArea(points[i]) > 100)
        {
            cv::drawContours(img, points, i, cv::Scalar(0, 0 ,255), 2, 8, masks, 0, cv::Point());
            cv::circle(img, centroids[i], 5, cv::Scalar(0, 255, 0), -1);
        }
    }
}
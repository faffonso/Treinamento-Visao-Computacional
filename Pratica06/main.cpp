#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>


int main(int argc, char **argv)
{
    cv::VideoCapture capture(0);

    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    cv::namedWindow("Video Cinza", cv::WINDOW_NORMAL);

    double fps;
    fps = capture.get(cv::CAP_PROP_FPS);

    capture.set(cv::CAP_PROP_FRAME_WIDTH, 160);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 120);

    if(!capture.isOpened())
    {
        std::cout << "[ERROR] Video not opened" << std::endl;
        return -1;
    }    

    std::cout << capture.get(3) << std::endl;
    std::cout << capture.get(4) << std::endl;

    while(1)
    {
        cv::Mat frame, frame_gray;
        
        capture.read(frame);
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

        //std::cout << "[INFO] Video FPS: " << fps << std::endl;

        cv::imshow("Video", frame);
        cv::imshow("Video Cinza", frame_gray);

        if (cv::waitKey(10) == 'e')
        {
            std::cout << "[INFO] Video was closed";
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}
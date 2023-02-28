#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#define BUTTON_ON 225

/**
 * @brief Detect circles in image using Hough transform
 * 
 * @param img Image with draw representations
 * @param img_gray Gray source image 
 * @param circles Circles found with Hough transform
 */
void detect_circles(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> &circles);

/**
 * @brief Calculates attached buttons
 * 
 * @param img Image with draw representations
 * @param img_gray Gray source image 
 * @param circles Circles in image
 * @param circles_on Circles with attached buttons 
 * @param show Choose whether information will be printed
 */
void calculate_circles_on(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> circles, std::vector<cv::Vec3f> &circles_on, bool show=false);


int main(int argc, char **argv)
{
    //Declaring variables
    cv::Mat img;                                //Source image
    cv::Mat img_gray, img_edge, img_res;        //Image auxiliary
    std::vector<cv::Vec3f> circles, circles_on; //Vector with circles and circles with buttons attached
    std::vector<std::string> states;            //Buttons states

    //Creating windows
    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Imagem resultante", cv::WINDOW_NORMAL);

    //Load and verify image
    img = cv::imread("painel.png", cv::IMREAD_COLOR);
    if(!img.data)
    {
        std::cout << "[ERROR] Could not open image" << std::endl;
        return -1;
    }
    
    //Treat data
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    img.copyTo(img_res);

    //Apply function to get circles with buttons attached
    detect_circles(img_res, img_gray, circles);
    calculate_circles_on(img_res, img_gray, circles, circles_on);

    //Checks if the attached button represents on or off
    //If the button is top falh of the image tge circuit on
    for(auto circle : circles_on)
        states.push_back( circle[0] > img.rows / 2 ? "Ligado" : "Desligado" );
    
    //Print circuit states
    std::cout << "[INFO] Estados dos botoes: ";
    for(auto state : states)
        std::cout << state << ", ";
    std::cout << "\b\b " << std::endl;
    
    //Show images
    cv::imshow("Imagem", img);                  //Source image
    cv::imshow("Imagem resultante", img_res);   //Image with draws

    //Finish program
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

void detect_circles(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> &circles)
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


void calculate_circles_on(cv::Mat img, cv::Mat img_gray, std::vector<cv::Vec3f> circles, std::vector<cv::Vec3f> &circles_on, bool show)
{
    for(auto circle : circles)
    {
        int x = circle[0];
        int y = circle[1];
        auto radius = circle[2];

        auto value = img_gray.at<uint8_t>(y, x);

        if (show == true)
            std::cout << "[INFO] Pixel image with color " << value << std::endl;
        
        if(value > BUTTON_ON)
        {
            cv::circle(img, cv::Point(x, y), radius, cv::Scalar(0, 0, 255), 10);
            circles_on.push_back(circle);
        }
    }
}
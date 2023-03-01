#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#define RESIZE_DIV 1

#define BLUE_LOW_H 100
#define RED_LOW_H 170
#define GREEN_LOW_H 45
#define YELLOW_LOW_H 20

#define BLUE_HIGH_H 130
#define RED_HIGH_H 180
#define GREEN_HIGH_H 85
#define YELLOW_HIGH_H 34

struct Mask {
    std::string name;
    int low;
    int high;
    cv::Scalar draw_color;
    cv::Mat img = cv::Mat();
    std::vector<cv::Point2f> centroids = std::vector<cv::Point2f>();
};

struct Block {
    std::string name;
    std::string type;
    int quadrant;
    //int sub_quadrant;
};

void calculateCentroids(Mask *mask, cv::Mat img, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, bool draw = true);

int main(int argc, char **argv)
{
    //Variables
    cv::Mat img, img_hsv, img_res;

    std::vector<Mask *> masks;
    std::vector<Block *> blocks;

    masks.push_back(new Mask{"Blue", BLUE_LOW_H, BLUE_HIGH_H, cv::Scalar(255, 0, 0)});
    masks.push_back(new Mask{"Red", RED_LOW_H, RED_HIGH_H, cv::Scalar(0, 0, 255)});
    masks.push_back(new Mask{"Green", GREEN_LOW_H, GREEN_HIGH_H, cv::Scalar(0, 255, 0)});
    masks.push_back(new Mask{"Yeelow", YELLOW_LOW_H, YELLOW_HIGH_H, cv::Scalar(0, 255, 255)});

    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Resultado", cv::WINDOW_NORMAL);

    img = cv::imread("cenario_open.png", cv::IMREAD_COLOR);

    int width = img.cols;
    int height = img.rows;

    cv::resize(img, img, cv::Size(width / RESIZE_DIV, height / RESIZE_DIV));
    img_res = cv::Mat::zeros(height / RESIZE_DIV, width / RESIZE_DIV, CV_8UC3);

    cv::cvtColor(img ,img_hsv , cv::COLOR_BGR2HSV);

    int quadrant;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    for(auto mask : masks)
    {
        cv::inRange(img_hsv, cv::Scalar(mask->low, 50, 50), cv::Scalar(mask->high, 255, 255), mask->img);
        cv::findContours(mask->img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
        calculateCentroids(mask, img_res, contours, hierarchy);
        for (auto centroid : mask->centroids)
        {
            if (centroid.x < width / 2)
            {
                quadrant = (centroid.y < height / 2) ? 3 : 4;
            }
            else
            {
                quadrant = (centroid.y < height / 2) ? 2 : 1;
            }
            blocks.push_back(new Block{mask->name, "Colorido", quadrant});

        }
    }
    
    //Found forms blocks


    //Found binary blocks


    //Define sub quadrants


    cv::imshow("Imagem", img);
    cv::imshow("Resultado", img_res);
       
    for (auto mask : masks)
    {
        //cv::namedWindow(mask->name, cv::WINDOW_NORMAL);
        //cv::imshow(mask->name, mask->img);
        //std::cout << "[INFO] Bloco cololrido (" << mask->name << ") - Quadrante: " << mask->quadrant << std::endl;
    }

    for(auto block: blocks)
        std::cout << "[INFO] Color block: " << block->name << ", Type: " << block->type << ", Quadrant: " << quadrant << std::endl;

    cv::waitKey(0);

    return 0;
}

void calculateCentroids(Mask *mask, cv::Mat img, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, bool draw)
{
    //Calculo dos momentos
    std::vector<cv::Moments> m(contours.size());
    for(auto i = 0; i < contours.size(); i++)
        m[i] = cv::moments(contours[i]);

    //Calculos dos centroides
    for(auto i = 0; i < contours.size(); i++)
        if(m[i].m00 > 6000*4)
        {
            auto x = m[i].m10 / m[i].m00;
            auto y = m[i].m01 / m[i].m00;
            mask->centroids.push_back(cv::Point2f(x, y));
            if (draw == true)
                cv::drawContours(img, contours, i, mask->draw_color, 2, 8, hierarchy, 0, cv::Point());
        }
}
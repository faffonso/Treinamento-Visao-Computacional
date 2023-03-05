#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "assets.h"

// Range of HSV colors

#define BLUE_LOW_H 100
#define RED_LOW_H 170
#define GREEN_LOW_H 45
#define YELLOW_LOW_H 20

#define BLUE_HIGH_H 130
#define RED_HIGH_H 180
#define GREEN_HIGH_H 85
#define YELLOW_HIGH_H 34

int main(int argc, char **argv)
{
    // Variables
    cv::Mat img,
            img_gray, img_hsv, img_binary,
            img_res;

    // Vectors
    std::vector<Mask *> masks;
    std::vector<Block *> blocks;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    // Adding colors masks
    masks.push_back(new Mask{"Blue", BLUE_LOW_H, BLUE_HIGH_H, cv::Scalar(255, 0, 0)});
    masks.push_back(new Mask{"Red", RED_LOW_H, RED_HIGH_H, cv::Scalar(0, 0, 255)});
    masks.push_back(new Mask{"Green", GREEN_LOW_H, GREEN_HIGH_H, cv::Scalar(0, 255, 0)});
    masks.push_back(new Mask{"Yeelow", YELLOW_LOW_H, YELLOW_HIGH_H, cv::Scalar(0, 255, 255)});

    // Load and verify source image
    img = cv::imread("images/cenario_open.png", cv::IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "[ERROR] Cannot read image" << std::endl;
        return -1;
    }

    // Get image dimensions and resize by factor
    int width = img.cols;
    int height = img.rows;

    cv::resize(img, img, cv::Size(width / RESIZE_DIV, height / RESIZE_DIV));
    img_res = cv::Mat::zeros(height / RESIZE_DIV, width / RESIZE_DIV, CV_8UC3);

    // Convert image to HSV and Gray
    cv::cvtColor(img ,img_hsv , cv::COLOR_BGR2HSV);
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // Get colored and shapes blocks for each mask
    for(auto mask : masks)
    {
        cv::inRange(img_hsv, cv::Scalar(mask->low, 50, 50), cv::Scalar(mask->high, 255, 255), mask->img);
        cv::findContours(mask->img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
        get_color_blocks(mask, blocks, img_res, contours, hierarchy);
    }

    // Get binary blocks
    cv::threshold(img_gray, img_binary, 0, 255, cv::THRESH_BINARY_INV);
    cv::findContours(img_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    get_binary_blocks(img_binary, blocks, img_res, contours, hierarchy);

    // Find quadrant and sub-quadrant of all blocks
    get_quadrants(blocks, img.cols, img.rows);

    // Show source and result images
    cv::namedWindow("Imagem", cv::WINDOW_NORMAL);
    cv::namedWindow("Resultado", cv::WINDOW_NORMAL);
    cv::imshow("Imagem", img);
    cv::imshow("Resultado", img_res);
       
    // Show threshold masks images
    for (auto mask : masks)
    {
        cv::namedWindow(mask->name, cv::WINDOW_NORMAL);
        cv::imshow(mask->name, mask->img);
    }

    // Print blocks information
    std::cout << "[INFO] Block size: " << blocks.size() << std::endl;
    for(auto block: blocks)
        switch (block->block_type)
        {
            case COLOR:
                std::cout << "[INFO] Color block! - Color: " << block->color << ", Quadrant: " << block->quadrant << ", Sub-Quadrant: " << block->sub_quadrant << std::endl;
                break;
            case SHAPE:
                std::cout << "[INFO] Shape block! - Shape " << block->forms_type << ", Quadrant: " << block->quadrant <<  ", Sub-Quadrant: " << block->sub_quadrant << std::endl;
                break;
            case BINARY:
                std::cout << "[INFO] Binary block! - Value " << block->binary_value << ", Quadrant: " << block->quadrant <<  ", Sub-Quadrant: " << block->sub_quadrant << std::endl;
                break;
            default:
                std::cout << "[ERROR] Block don't have type" << std::endl; 
        }

    cv::waitKey(0);

    return 0;
}


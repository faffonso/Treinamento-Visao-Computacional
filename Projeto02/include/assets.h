#ifndef ASSETS_H
#define ASSETS_H

#include <iostream>
#include <numeric>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

//Original image resize factor
#define RESIZE_DIV 4

/* Enumerations */

// Enum of block types
enum BlocksType {
    COLOR = 1,
    SHAPE = 2,
    BINARY = 3, 
};

// Enum of block shapes
enum Shapes {
    NONE = 0,
    TRIANGLE = 3,
    LOSANGE = 4,
    STAR = 5,
};

/* Structs */

// HSV masks struct
struct Mask {
    std::string name;           // Name of the mask
    int low;                    // Low range of H value
    int high;                   // High range of H value
    cv::Scalar draw_color;      // Color to draw block outline
    cv::Mat img = cv::Mat();    // Image to return threshold of mask
};

// Block struct
struct Block {
    cv::Point2f centroid;       // Centroid
    BlocksType block_type;      // Block type (enum)
    std::string color;          // If is colored block, receive color info
    std::string forms_type;     // If is shape block, receive shape info
    int binary_value = int();   // If is binary block, receive binary value
    int quadrant = int();       // Quadrant of block
    int sub_quadrant = int();   // Sub quadrant of block
};


/* Functions */

/**
 * @brief Get the color blocks object
 * 
 * @param mask Struct with all mask information
 * @param blocks Struct will sabe identify blocks
 * @param img Image to draw blocks identified by mask
 * @param contours Detected contours stored in points
 * @param hierarchy Information about the image topology
 * @param draw Boolean to define if will contour blocks
 */
void get_color_blocks(Mask *mask, std::vector<Block *> &blocks, cv::Mat img, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, bool draw=true);

/**
 * @brief Get the binary blocks object
 * 
 * @param img_binary Binarized source image
 * @param blocks Struct will sabe identify blocks
 * @param img Image to draw blocks identified by mask
 * @param contours Detected contours stored in points
 * @param hierarchy Information about the image topology
 * @param draw Boolean to define if will contour blocks
 */
void get_binary_blocks(cv::Mat img_binary, std::vector<Block *> &blocks, cv::Mat img, std::vector<std::vector<cv::Point> > contours, bool draw=true);

/**
 * @brief Get the quadrants object
 * 
 * @param blocks Struct will sabe identify blocks
 * @param width Width of source image
 * @param height Height of source image
 */
void get_quadrants(std::vector<Block *> &blocks, int width, int height);

/* Auxiliary functions */

std::vector<cv::Moments> calculate_moments(std::vector<std::vector<cv::Point> > contours);
std::vector<cv::Point2f> calculate_centroids(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Moments> m);
std::string get_shape(std::vector<cv::Point> vertices);
double calculate_mean (std::vector<cv::Moments> m);

#endif /* ASSETS_H */

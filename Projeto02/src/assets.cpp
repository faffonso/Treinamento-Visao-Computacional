#include "assets.h"

void get_color_blocks(Mask *mask, std::vector<Block *> &blocks,cv::Mat img, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, bool draw)
{
    auto m = calculate_moments(contours);
    auto m_mean = calculate_mean(m);
    auto centroids = calculate_centroids(contours, m);
    double epsilon = 0.03 * cv::arcLength(contours[0], true);
    std::vector<cv::Point> vertices;

    for(auto i = 0; i < contours.size(); i++)
    {
        if (draw == true)
            cv::drawContours(img, contours, i, mask->draw_color, cv::FILLED);

        if(m[i].m00 >= m_mean) 
            blocks.push_back(new Block{centroids[i], COLOR, mask->name, ""});

        else
        {
            cv::approxPolyDP(contours[i], vertices, epsilon, true);
            blocks.push_back(new Block{centroids[i], SHAPE, "", get_shape(vertices)});
        }
    }
}

void get_binary_blocks(cv::Mat img_binary, std::vector<Block *> &blocks, cv::Mat img, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, bool draw)
{
    auto m = calculate_moments(contours);
    auto m_mean = calculate_mean(m);
    auto centroids = calculate_centroids(contours, m);
    std::vector<double> widths;

    for(auto i = 0; i < contours.size(); i++)
    {
        if (m[i].m00 <=  m_mean)
        {
            if (draw == true)
                cv::drawContours(img, contours, i, cv::Scalar(255, 255, 255), cv::FILLED);

            cv::drawContours(img_binary, contours, i, cv::Scalar(0, 0, 0), 2, 8, hierarchy, 0, cv::Point());
            widths.push_back(cv::boundingRect(contours[i]).width);
        }
        else
            cv::drawContours(img_binary, contours, i, cv::Scalar(0, 0, 0), cv::FILLED);
    }


    cv::findContours(img_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    m = calculate_moments(contours);
    auto centroids2 = calculate_centroids(contours, m);

    int binary = 0;
    for (auto i = 0; i < contours.size(); ++i) 
    {
        auto width = cv::boundingRect(contours[i]).width;
        auto ref = centroids[i / 2].x - (widths[i / 2] * 3 / 5);
        int bytes = std::round(5*width/widths[i / 2]);

        int byte_position = std::round(( centroids2[i].x - ref) * 5 / widths[i / 2]);
        byte_position--;

        for (auto j = 0; j < bytes; j++)
            binary |= (1 << byte_position+j);
        
        if (((i+1) % widths.size()) == 0)
        {
            blocks.push_back(new Block{centroids2[i], BINARY, "", "", binary});
            binary = 0;
        }

    }            

}

void get_quadrants(std::vector<Block *> &blocks, int width, int height)
{
    int sub_width;
    int sub_height;
    for (auto block : blocks)
    {
        if (block->centroid.x < width / 2 && block->centroid.y < height / 2) 
        {
            block->quadrant = 3;
            sub_width = width / 4;
            sub_height = height / 4;
            if (block->centroid.x < sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 3;
            } else if (block->centroid.x >= sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 2;
            } else if (block->centroid.x < sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 4;
            } else if (block->centroid.x >= sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 1;
            }
        } 
        else if (block->centroid.x >= width / 2 && block->centroid.y < height / 2) 
        {
            block->quadrant = 2;
            sub_width = width * 3 / 4;
            sub_height = height / 4;
            if (block->centroid.x < sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 3;
            } else if (block->centroid.x >= sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 2;
            } else if (block->centroid.x < sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 4;
            } else if (block->centroid.x >= sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 1;
            }
        } 
        else if (block->centroid.x < width / 2 && block->centroid.y >= height / 2) 
        {
            block->quadrant = 4;
            sub_width = width / 4;
            sub_height = height * 3 / 4;
            if (block->centroid.x < sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 3;
            } else if (block->centroid.x >= sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 2;
            } else if (block->centroid.x < sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 4;
            } else if (block->centroid.x >= sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 1;
            }
        } 
        else if (block->centroid.x >= width / 2 && block->centroid.y >= height / 2) 
        {
            block->quadrant = 1;
            sub_width = width * 3/ 4;
            sub_height = height * 3/ 4;
            if (block->centroid.x < sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 3;
            } else if (block->centroid.x >= sub_width && block->centroid.y < sub_height) {
                block->sub_quadrant = 2;
            } else if (block->centroid.x < sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 4;
            } else if (block->centroid.x >= sub_width && block->centroid.y >= sub_height) {
                block->sub_quadrant = 1;
            }
        }
    }
}

std::vector<cv::Moments> calculate_moments(std::vector<std::vector<cv::Point> > contours)
{
    std::vector<cv::Moments> m(contours.size());
    for(auto i = 0; i < contours.size(); i++)
        m[i] = cv::moments(contours[i]);

    return m;
}

std::vector<cv::Point2f> calculate_centroids(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Moments> m)
{
    std::vector<cv::Point2f> centroids;
    for(auto i = 0; i < contours.size(); i++)
    {
        
        auto x = m[i].m10 / m[i].m00;
        auto y = m[i].m01 / m[i].m00;
        centroids.push_back(cv::Point2f(x, y));
    }

    return centroids;
}


std::string get_shape(std::vector<cv::Point> vertices)
{
    std::string type;
    switch (vertices.size())
    {
        case TRIANGLE:
            type = "Triangle";
            break;
        case LOSANGE:
            type = "Losange";
            break;
        case STAR:
            type = "Star";
            break;
        default:
            std::cout << "[ERROR] Unkown shape";
    }
    return type;
}

double calculate_mean (std::vector<cv::Moments> m)
{
    double mean = 0;
    for (auto value : m)
        mean += value.m00;

    return mean / m.size();
}
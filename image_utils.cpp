#include "image_utils.h"


void DisplayImage(const std::string description, cv::Mat image)
{
    if (!image.data)
    {
        std::cout << "Could not to open or find image" << std::endl;
    }
    cv::namedWindow(description,cv::WINDOW_AUTOSIZE);
    cv::imshow(description,image);
}

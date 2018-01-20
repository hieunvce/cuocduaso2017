#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <opencv2/opencv.hpp>
#include <fstream>
using namespace cv;

cv::Mat RemoveSmallObjects(const cv::Mat& bin_image, const cv::Mat& original_image);
std::vector<cv::Rect> detectRoadSign(const cv::Mat& bin_image);

#endif // POSTPROCESSING_H

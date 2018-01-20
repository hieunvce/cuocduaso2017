#ifndef IHLSANDNHS_H
#define IHLSANDNHS_H

#include <opencv2/opencv.hpp>

/**
 * Returning hue value of RGB.
 * Returning luminance value of RGB.
 * Returning saturation value of RGB.
 */
inline float retrieve_normalised_hue(unsigned int r, unsigned int g, unsigned int b);
inline float retrieve_luminance(unsigned int r, unsigned int g, unsigned int b);
inline float retrieve_saturation(unsigned int r, unsigned int g, unsigned int b);

/**
 * This function converts the an RGB image to IHLS and returns it.
 */
cv::Mat convert_rgb_to_ihls(cv::Mat rgb_image);

/**
  * This function converts an IHLS image to BW image
  * use NHS segmentation and returns it.
  *
  * RED: (H<15 || (H>183 && S>16))
  * BLUE: 143<H<170 && S>36
  */
cv::Mat nhs_red_segmentation(cv::Mat ihls_image);
cv::Mat nhs_blue_segmentation(cv::Mat ihls_image);
cv::Mat nhs_white_segmentation(cv::Mat ihls_image);

#endif // IHLSANDNHS_H

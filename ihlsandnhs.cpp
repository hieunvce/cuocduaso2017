#include "ihlsandnhs.h"
#include "math_utils.h"
#include "opencv/ml.h"
#include "opencv2/opencv.hpp"

#include <cmath>

#include <iostream>

using namespace cv;
using namespace std;

/**
 * It calculates theta bases on the equation provided in Valentine thesis.
 *
 * The returned theta is radian.
 */
float retrieve_theta( unsigned int r, unsigned int g, unsigned int b )
{
    float theta;

    // The numerator part of equation
    float numerator = r - (g * 0.5) - (b * 0.5);

    // The denominator part of equation
    float denominator = (r * r) + (g * g) + (b * b) - (r * g) - (r * b) - (g * b);

    float temp = numerator / sqrtf(denominator);
    theta = acos(temp);

    return theta;
}

/**
 * Calculating the hue value based on the blow formula:
 *
 * H = θ if B <= G
 * H = 2 * pi − θ if B > G
 *
 * The return value is normalised between 0 to 255.
 */
float retrieve_normalised_hue( unsigned int r, unsigned int g, unsigned int b )
{
    float hue;
    if ( b <= g )
        hue = retrieve_theta(r, g, b);
    else
        hue = (2 * M_PI) - retrieve_theta(r, g, b);

    return hue * 255 / (2 * M_PI);
}

/**
 * Luminance is calculated as:
 *
 * L = 0.2126R + 0.7152G + 0.0722B
 */
float retrieve_luminance( unsigned int r, unsigned int g, unsigned int b )
{
    return (0.2126f * r) + (0.7152f * g) + (0.0722f * b);
}

/**
 * Saturation is calculates as below:
 *
 * S = max(R, G, B) − min(R, G, B)
 */
float retrieve_saturation( unsigned int r, unsigned int g, unsigned int b )
{
    float saturation;
    unsigned int max = get_maximum(r, g, b);
    unsigned int min = get_minimum(r, g, b);

    saturation = max - min;

    return saturation;
}

cv::Mat convert_rgb_to_ihls( cv::Mat rgb_image )
{
    assert(rgb_image.channels() == 3);

    cv::Mat ihls_image(rgb_image.rows, rgb_image.cols, CV_8UC3);

    for ( int i = 0; i < rgb_image.rows; ++i )
    {
        const uchar* rgb_data = rgb_image.ptr<uchar> (i);
        uchar* ihls_data = ihls_image.ptr<uchar> (i);

        for ( int j = 0; j < rgb_image.cols; ++j )
        {
            unsigned int b = *rgb_data++;
            unsigned int g = *rgb_data++;
            unsigned int r = *rgb_data++;
            if (r >= 180) {
                *ihls_data++ = (uchar) retrieve_saturation(r, g, b);
                *ihls_data++ = (uchar) retrieve_luminance(r, g, b);
                *ihls_data++ = (uchar) retrieve_normalised_hue(r, g, b);
            }
            else {
                r = 0;
                g = 0;
                b = 0;
                *ihls_data++ = (uchar) retrieve_saturation(r, g, b);
                *ihls_data++ = (uchar) retrieve_luminance(r, g, b);
                *ihls_data++ = (uchar) retrieve_normalised_hue(r, g, b);
            }
        }
    }

    return ihls_image;
}


cv::Mat nhs_red_segmentation ( cv::Mat ihls_image)
{
    assert(ihls_image.channels() == 3);

    cv::Mat seg_red_image(ihls_image.rows, ihls_image.cols, CV_8UC3);
    cv::Mat medianImage(ihls_image.rows, ihls_image.cols, CV_8UC3);

    for ( int i = 0; i < ihls_image.rows; ++i )
    {
        const uchar* ihls_data = ihls_image.ptr<uchar> (i);
        uchar* seg_data = seg_red_image.ptr<uchar> (i);

        for ( int j = 0; j < ihls_image.cols; ++j )
        {
            unsigned int s = *ihls_data++;
            *ihls_data++;
            unsigned int h = *ihls_data++;
            if ((h < 15 || (h > 183 && s > 16)))
            {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            } else {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }

    medianBlur(seg_red_image, medianImage, 5);
    return seg_red_image;
}

cv::Mat nhs_blue_segmentation ( cv::Mat ihls_image )
{
    assert(ihls_image.channels() == 3);

    cv::Mat seg_blue_image(ihls_image.rows, ihls_image.cols, CV_8UC3);
    cv::Mat medianImage(ihls_image.rows, ihls_image.cols, CV_8UC3);

    for ( int i = 0; i < ihls_image.rows; ++i )
    {
        const uchar* ihls_data = ihls_image.ptr<uchar> (i);
        uchar* seg_data = seg_blue_image.ptr<uchar> (i);

        for ( int j = 0; j < ihls_image.cols; ++j )
        {
            unsigned int s = *ihls_data++;
            *ihls_data++;
            unsigned int h = *ihls_data++;
            if (h > 143 && s > 36) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }
            else {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }

    medianBlur(seg_blue_image, medianImage, 5);
    return seg_blue_image;
}

cv::Mat nhs_white_segmentation(cv::Mat ihls_image)
{
    assert(ihls_image.channels() == 3);

    cv::Mat seg_blue_image(ihls_image.rows, ihls_image.cols, CV_8UC3);
    cv::Mat medianImage(ihls_image.rows, ihls_image.cols, CV_8UC3);

    for ( int i = 0; i < ihls_image.rows; ++i )
    {
        const uchar* ihls_data = ihls_image.ptr<uchar> (i);
        uchar* seg_data = seg_blue_image.ptr<uchar> (i);

        for ( int j = 0; j < ihls_image.cols; ++j )
        {
            unsigned int s = *ihls_data++;
            unsigned int l = *ihls_data++;
            unsigned int h = *ihls_data++;
            if (l >= 95 && l <= 100) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }
            else {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }

    medianBlur(seg_blue_image, medianImage, 5);
    return seg_blue_image;
}
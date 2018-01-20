#include <iostream>
#include <string>
#include <vector>
#include "post-processing.h"
#include "define.h"

#include <sstream>

namespace patch
{
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}
}

using namespace std;
using namespace cv;


cv::Mat RemoveSmallObjects(const cv::Mat& bin_image, const cv::Mat& original_image)
{
    cout << "Remove Small Objects Module" << endl;
    ofstream outFile;
    outFile.open("./The6769ProTeam.txt",ios::out | ios::app);
    if (!outFile)
    {
        cerr << "Unable to open file The6769ProTeam.txt";
        exit(1);
    }

    cv::Mat gray_image = bin_image.clone();
    cvtColor(gray_image, gray_image, CV_BGR2GRAY);
    cv::Mat img = original_image.clone();

    //Erode and Dilate
    Mat dst,tmp;
    int erosion_size = EROSION_SIZE;
    Mat element = getStructuringElement(cv::MORPH_CROSS,cv::Size(2*erosion_size+1,2*erosion_size+1),cv::Point(erosion_size,erosion_size));
    morphologyEx(gray_image,tmp,2,element);
    morphologyEx(tmp,dst,3,element);
    //namedWindow("dst_image",CV_WINDOW_AUTOSIZE);
    //imshow("dst_image",dst);
    //end erode and dilate


    std::vector<std::vector<Point> > contours, contours_temp;
    std::vector<Vec4i> hierarchy;
    cv::Mat img_tmp = dst.clone();
    findContours(dst, contours_temp, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (size_t i = 0; i < contours_temp.size(); i++)
    {
    	double area_temp = contourArea(contours_temp[i]);
    	if (area_temp >= MIN_AREA && area_temp <= MAX_AREA)
    			drawContours(img_tmp, contours_temp, (int)i, (255, 255, 255), CV_FILLED, 8, hierarchy, 0, Point());
    }
    //namedWindow("contourfilled_image",CV_WINDOW_AUTOSIZE);
    //imshow("contourfilled_image",img_tmp);
    findContours(img_tmp, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area >= MIN_AREA && area <= MAX_AREA)
        {
            Rect boundRect;
            boundRect = boundingRect(contours[(int)i]);
            int w = boundRect.width;
            int h = boundRect.height;
            if ((((double)w/h >= 0.7) && ((double)w/h <=1.2)))
            {
                cout << "Rectangle[" << (int)i << "]: " << w << ":" << h << " => w/h= " << (double)w/h << "  Area: "<< area << endl;
                outFile << "x " << boundRect.br().x << " " << boundRect.br().y << " " << boundRect.tl().x << " " << boundRect.tl().y << "\n";
                //drawContours(img, contours, (int)i, (255, 255, 255), 1, 8, hierarchy, 0, Point());
                rectangle(img,boundRect.tl(), boundRect.br(), (255, 255, 255), 2, 8, 0);
                //string st = patch::to_string(i);
                //putText(img,st,boundRect.tl(),1,1,(255,255,255),2,8,false);
            }
        }
    }
    outFile.close();
    return img;
}

std::vector<cv::Rect> detectRoadSign(const cv::Mat& bin_image)
{
    cout << "Detect Road Sign" << endl;
    vector<Rect> signDetected;
    int index=0;
    cv::Mat gray_image = bin_image.clone();
    cvtColor(gray_image, gray_image, CV_BGR2GRAY);

    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours(gray_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area >= MIN_AREA)
        {
            Rect boundRect;
            boundRect = boundingRect(contours[(int)i]);
            int w = boundRect.width;
            int h = boundRect.height;
            if ((double)w/h >= 0.333 &&  (double)h/w >= 0.333)
            {
                signDetected.push_back(boundRect);
            }
        }
    }
    return signDetected;
}

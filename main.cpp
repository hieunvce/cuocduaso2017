#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

#include "image_utils.h"
#include "ihlsandnhs.h"
#include "post-processing.h"

using namespace std;
using namespace cv;

namespace patch
{
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}
}

int main(int argc, char *argv[])
{
    cout << "Road Sign Recognition" << endl;
    if (argc != 2)
    {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    
    //---------------------------------For video------------------------------------------------
    //For video
    const  string source = argv[1];
    VideoCapture inputVideo(source);
    if (!inputVideo.isOpened())
    {
    	cout << "Could not open the input video: " << source << endl;
    	return -1;
    }

    //Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
                  //(int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    double fps = inputVideo.get(CV_CAP_PROP_FPS);
    Size S = Size(640,480);
    VideoWriter outputVideo("The6769ProTeam.avi",CV_FOURCC('M','J','P','G'),fps,S);
    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << source << endl;
        return -1;
    }
    ofstream outFile;
    for (;;)
    {
        outFile.open("./The6769ProTeam.txt",ios::out | ios::app);
        if (!outFile)
        {
            cerr << "Unable to open file The6769ProTeam.txt";
            exit(1);
        }
    	Mat frame;
    	inputVideo >> frame;
    	if (frame.empty()) break;
    	//Process
        int framID = inputVideo.get(CV_CAP_PROP_POS_FRAMES);       
        outFile << framID << " ";
        outFile.close();
    	Mat ihls_image = convert_rgb_to_ihls(frame);
        imshow("IHLS",ihls_image);
    	Mat seg_image = nhs_red_segmentation(ihls_image);
    	Mat outframe = RemoveSmallObjects(seg_image,frame);
    	//EndProcess
    	outputVideo.write(outframe);
    	imshow("Frame",outframe);


    	char c = (char)waitKey(1);
    	if (c==27)
    		break;
    	
    }
    inputVideo.release();
    outputVideo.release();
    destroyAllWindows();
    //---------------------------------For video------------------------------------------------

    //---------------------------------For image------------------------------------------------
    /*
    cv::Mat originalImage = cv::imread(argv[1],CV_LOAD_IMAGE_COLOR);
    if (!originalImage.data)
    {
        std::cout << "Could not to open or find image" << std::endl;
        return -1;
    }
	

    Mat ihls_image = convert_rgb_to_ihls(originalImage);
    Mat red_image = nhs_red_segmentation(ihls_image);
    Mat img_red = RemoveSmallObjects(red_image,originalImage);
    Mat blue_image = nhs_blue_segmentation(ihls_image);
    Mat img_blue = RemoveSmallObjects(blue_image,originalImage);
    
    //Mat white_image = nhs_white_segmentation(ihls_image);
    //Mat img_white = RemoveSmallObjects(white_image,originalImage);
    /*vector<Rect> signDetected = detectRoadSign(bin_image);
    //for (int i=0; i<signDetected.size(); ++i)
    //{
        cout << "(" << signDetected[i].br().x << ", " << signDetected[i].br().y << ")";
        cout << "(" << signDetected[i].tl().x << ", " << signDetected[i].tl().y << ")" << endl;
    }
    //DisplayImage("ihls_image",ihls_image);
    //DisplayImage("bin_image",bin_image);
    DisplayImage("Image RED",img_red);
    DisplayImage("Image BLUE",img_blue);
    //DisplayImage("Image WHITE",img_white);

    waitKey(0);
    */
     //---------------------------------For image------------------------------------------------
    return 0;
}


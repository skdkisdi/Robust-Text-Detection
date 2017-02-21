//
//  main.cpp
//  RobustTextDetection
//
//  Created by Saburo Okita on 05/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <iostream>
#include <iterator>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "RobustTextDetection.h"
#include "ConnectedComponent.h"

using namespace std;
using namespace cv;

void help(void)
{
	cout << "Usage: Robust_Text_Detection 1.png [path to save intermediate images]" << endl;
}

int main(int argc, const char * argv[])
{

    if (argc != 2 && argc != 3)
    {
	help();
	return 0;
    }

    namedWindow( "" );
    moveWindow("", 0, 0);
    
    Mat image = imread( argv[1] );
    
    /* Quite a handful or params */
    RobustTextParam param;
    param.minMSERArea        = 10;
    param.maxMSERArea        = 2000;
    param.cannyThresh1       = 20;
    param.cannyThresh2       = 100;
    
    param.maxConnCompCount   = 3000;
    param.minConnCompArea    = 75;
    param.maxConnCompArea    = 600;
    
    param.minEccentricity    = 0.1;
    param.maxEccentricity    = 0.995;
    param.minSolidity        = 0.4;
    param.maxStdDevMeanRatio = 0.5;
    
    /* Apply Robust Text Detection */
    /* ... remove this temp output path if you don't want it to write temp image files */
    string temp_output_path;
    if (argc == 3) 
    	temp_output_path = argv[2];
    RobustTextDetection detector(param, temp_output_path );
    pair<Mat, Rect> result = detector.apply( image );
    
    /* Get the region where the candidate text is */
    Mat stroke_width( result.second.height, result.second.width, CV_8UC1, Scalar(0) );
    Mat(result.first, result.second).copyTo( stroke_width);
    
    
    /* Use Tesseract to try to decipher our image */
    tesseract::TessBaseAPI tesseract_api;
    tesseract_api.Init(NULL, "eng"  );
    tesseract_api.SetImage((uchar*) stroke_width.data, stroke_width.cols, stroke_width.rows, 1, stroke_width.cols);
    
    string out = string(tesseract_api.GetUTF8Text());

    /* Split the string by whitespace */
    vector<string> splitted;
    istringstream iss( out );
    copy( istream_iterator<string>(iss), istream_iterator<string>(), back_inserter( splitted ) );
    
    /* And draw them on screen */
    //CvFont font = cvFontQt("Helvetica", 24.0, CV_RGB(0, 0, 0) );
    Point coord = Point( result.second.br().x + 10, result.second.tl().y );
    for( string& line: splitted ) {
	/*puttext*/
        putText(image, line, coord, FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0, 0, 0));//, double fontScale, Scalar color, int thickness=1, int lineType=LINE_8, bool bottomLeftOrigin=false )
        //addText( image, line, coord, font );
        coord.y += 25;
    }
    
    rectangle( image, result.second, Scalar(0, 0, 255), 2);   

    /* Append the original and stroke width images together */
    cvtColor( stroke_width, stroke_width, CV_GRAY2BGR );
    Mat appended( image.rows, image.cols + stroke_width.cols, CV_8UC3 );
    image.copyTo( Mat(appended, Rect(0, 0, image.cols, image.rows)) );
    stroke_width.copyTo( Mat(appended, Rect(image.cols, 0, stroke_width.cols, stroke_width.rows)) );
    
    imshow("", appended );	
    /* show the text */
    cout << out << endl; 
    waitKey();
    
    return 0;
}

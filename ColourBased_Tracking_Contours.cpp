/*
 * take2.cpp
 *
 *  Created on: 20-Sep-2014
 *      Author: admin
 */


#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <iostream>
#include "opencv2/photo/photo.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

#define abc Size(2,2)


using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    int iLowH = 170;
    int iHighH = 179;

     int iLowS = 150;
    int iHighS = 255;

     int iLowV = 60;
    int iHighV = 255;

  Mat im = imread(argc == 2 ? argv[1] : "C:/Users/admin/Documents/My Bluetooth/nidhi.png", 1);
  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }
  imshow("image", im);
  Mat im1;
  Mat im2;
  Mat dst;
  Scalar col = Scalar(0,0,255);
  int flag=0;


  while(1)
  {
  cvtColor(im, im1, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
  inRange(im1, Scalar(iLowH,iLowS,iLowV), Scalar(iHighH,iHighS,iHighV), im2); //Threshold the image

  morphologyEx(im2, dst, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, abc) );
  morphologyEx(dst, dst, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, abc) );

  vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;

  findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

             vector<vector<Point> > contours_poly(contours.size());
             vector<Rect> boundRect(contours.size());

             for(int i=0; i<contours.size();i++)
             {
          	   approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
          	   boundRect[i]= boundingRect(Mat(contours_poly[i]));
             }
             Mat temp=Mat::zeros(dst.size(),CV_8UC3);
             for(int i = 0; i < contours.size(); i++)
             {
          	   rectangle(temp, boundRect[i].tl(), boundRect[i].br(), Scalar(0,255,255),2,8,0);
             }

             imshow("try", temp);

             switch(waitKey(30)) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
                    {
                     case 27:
                             cout << "esc key is pressed by user" << endl;
                             flag=1;
                             break;
                     case 98:
                     		iLowH = 75;
                     	    iHighH = 130;
                     	    col = Scalar(255,0,0);
                     	    cout<<"\nTracking blue!!\n";
                     	    break;
                     case 103:
                     		iLowH = 38;
                     		iHighH = 75;
                     		col = Scalar(0,255,0);
                     		cout<<"\nTracking Green!!\n";
                     		break;
                     case 114:
                    	 iLowH = 170;
                    	 iHighH = 190;
                    	 col = Scalar(0,255,0);
                    	 cout<<"\nTracking Red!!\n";
                    	 break;
                    }
                     if(flag)
                     {
                     	break;
                     }

  }

  waitKey(0);
  return 0;
 }



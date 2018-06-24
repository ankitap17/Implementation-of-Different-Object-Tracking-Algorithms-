#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv/cv.h"
#include "opencv2/photo/photo.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

#define abc Size(2,2)
int main(int argc, char* argv[])
{
    VideoCapture cap(0/*"C:/Jonathan.mp4"*//*"C:/Users/Public/Videos/Sample Videos/Wildlife.wmv"*/); // open the video file for reading

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

     cout << "Frame per seconds : " << fps << endl;

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE);
    namedWindow("Video",CV_WINDOW_AUTOSIZE);//create a window called "MyVideo"
    namedWindow("Trackbars",CV_WINDOW_AUTOSIZE);

    int iLowH = 170;
    int iHighH = 179;

     int iLowS = 150;
    int iHighS = 255;

     int iLowV = 60;
    int iHighV = 255;

     //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Trackbars", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Trackbars", &iHighH, 179);

     cvCreateTrackbar("LowS", "Trackbars", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Trackbars", &iHighS, 255);

     cvCreateTrackbar("LowV", "Trackbars", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Trackbars", &iHighV, 255);
    Mat imgHSV;
    Mat imgThresholded;
    Mat dst;
    Mat frame;
    Mat nframe;
    Mat tmp;
    cap.read(tmp);
    Mat imgLines=Mat::zeros(tmp.size(),CV_8UC3);
    int iX=-1;
    int iY=-1;
    Scalar col = Scalar(0,0,255);

    while(1)
    {
    	bool bSuccess = cap.read(frame); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
                        cout << "Cannot read the frame from video file" << endl;
                       break;
        }

        //fastNlMeansDenoisingColored(frame,nframe,3,7,5);
           cvtColor(frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
           //imshow("HSVVideo", imgHSV);


           inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
           //imshow("thresholded",imgThresholded);

           //morphological opening (remove small objects from the foreground)
          // erode(imgThresholded, imgThresholded, Mat()); //getStructuringElement(MORPH_ELLIPSE, size) );
           //dilate( imgThresholded, imgThresholded, Mat()); //getStructuringElement(MORPH_ELLIPSE, size) );

            //morphological closing (fill small holes in the foreground)
           //dilate( imgThresholded, imgThresholded, Mat()); //getStructuringElement(MORPH_ELLIPSE, size) );
           //erode(imgThresholded, imgThresholded, Mat()); //getStructuringElement(MORPH_ELLIPSE, size) );

           morphologyEx(imgThresholded, dst, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, abc) );
           morphologyEx(dst, dst, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, abc) );
           Moments m = moments(imgThresholded);
           double dM01=m.m01;
           double dM10=m.m10;
           double area=m.m00;
          // cout<<area<<endl;
           if(area>2000)
           {
        	   int X=dM10/area;
        	   int Y=dM01/area;
        	   if(iX>=0 && iY>=0 && X>=0 && Y>=0)
        	   {
        		   line(imgLines,Point(X,Y),Point(iX,iY),col,2);
        		   circle(frame,Point(X,Y),25,Scalar(0,255,0),2);
        	   }
        	   iX=X;
        	   iY=Y;
           }
           frame=frame+imgLines;
           imshow("Video", frame); //show the frame in "MyVideo" window
           imshow("MyVideo", dst);
           int flag=0;

        switch(waitKey(30)) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
        case 27:
                cout << "esc key is pressed by user" << endl;
                flag=1;
                break;
        case 98:
        		iLowH = 75;
        	    iHighH = 130;
        	    iX=-1;
        	    iY=-1;
        	    col = Scalar(255,0,0);
        	    cout<<"\nTracking blue!!\n";

        	    break;
        case 103:
        		iLowH = 38;
        		iHighH = 75;
        		iX=-1;
        		iY=-1;
        		col = Scalar(0,255,0);
        		cout<<"\nTracking Green!!\n";
        		break;

       }
        if(flag)
        {
        	break;
        }
        //waitKey(0);

    }

    return 0;

}

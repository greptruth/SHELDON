#include <sstream>
#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/contrib/contrib.hpp"

//#include <pstream>


#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

using namespace cv;
using namespace std;


int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}


void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN %d", H_MIN);
	sprintf( TrackbarName, "H_MAX %d", H_MAX);
	sprintf( TrackbarName, "S_MIN %d", S_MIN);
	sprintf( TrackbarName, "S_MAX %d", S_MAX);
	sprintf( TrackbarName, "V_MIN %d", V_MIN);
	sprintf( TrackbarName, "V_MAX %d", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}


void drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
	


}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

void blobDetection(Mat cameraFeed,Mat src)
{
	cv::SimpleBlobDetector::Params params;
 
	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;
	 
	// Filter by Area.
	params.filterByArea = false;
	params.minArea = 15;
	 
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.6;
	params.maxCircularity = 0.9;
	 
	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;
	 
	// Filter by Inertia
	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;

	params.filterByColor = true;
	params.blobColor = 0;

	const char* lang = "eng";
    tesseract::TessBaseAPI tess;
    //tesseract::TessBaseAPI::SetVariable("tessedit_char_whitelist", "0123456789");
    //tess.SetVariable("tessedit_char_whitelist", "+-X012345789");
    tess.Init(NULL, lang, tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

	/*#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2
 
	  // Set up detector with params
	  SimpleBlobDetector detector(params);
	 
	  // You can use the detector this way
	  // detector.detect( im, keypoints);
	 
	#else
	 
	  // Set up detector with params
	  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	 
	  // SimpleBlobDetector::create creates a smart pointer. 
	  // So you need to use arrow ( ->) instead of dot ( . )
	  // detector->detect( im, keypoints);
	 
	#endif*/
	  //SimpleBlobDetector detector(params);
	cv::Ptr<cv::SimpleBlobDetector> blob_detector = cv::SimpleBlobDetector::create();

	///
	/*vector<MSERParams> pMSER;
	vector<MSERParams>::iterator itMSER;
	itMSER = pMSER.begin();
*/
	// Detect blobs.
	//std::vector<std::vector<cv::KeyPoint> > keypoints;
	std::vector<KeyPoint> keypoints;
	cv::Ptr<cv::BRISK> ptrBrisk = cv::BRISK::create();
	//ptrBrisk->detect(imTmp, objectKeypoints);
	blob_detector->detect( cameraFeed, keypoints);
	std::cout<<"length="<<keypoints.size()<<"\n";

	Mat im_with_keypoints;

	Mat dst = Mat::zeros(cameraFeed.rows, cameraFeed.cols, CV_8UC3);
	vector< vector<Point> > contours,Contours;
	vector<Vec4i> hierarchy,Hierarchy;
	findContours( cameraFeed, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	Mat ROI1,ROI2,ROI3,ROI4;
	int idx = 0;
    for( int i=1; idx >= 0; idx = hierarchy[idx][0],i++ )
    {
        Scalar color( 255,255,255);
        drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );

        Rect R=boundingRect(contours[idx]);
        std::cout<<"here moherfucker \n";
        //R = Rect(R.x+5,R.y+5,R.width,R.height);
        cv::Rect myROI(100,100,100,100);
        Mat temp1,temp2,temp3,temp4,temp5;



        temp1 = src(R);
        //imshow("temp1",temp1);
        resize(temp1, temp2, Size(300,300), 0, 0, INTER_CUBIC);
        temp1=temp2.clone();
        temp2 = temp1(myROI);

       
    	src.at<Vec3b>(R.x + R.width/2,R.y + R.height/2)[2] = 255; 
    	temp2.at<Vec3b>(temp2.size().width/2,temp2.size().height/2)[2] = 255;
    	std::cout<<"this shit"<<R.x<<"    "<<R.y<<"\n";



        imshow("temp2",temp2);
        tess.SetImage((uchar*)temp2.data, temp2.size().width, temp2.size().height,temp2.channels(), temp2.step1());
	    char* out1 = tess.GetUTF8Text();
	    std::cout << "tess intermediate \n"<<out1 << std::endl;
	    imwrite( "/home/aytas32/temp2.jpg", temp2 );
	    /*redi::ipstream proc("./some_command", redi::pstreams::pstderr);
  		std::string line;
  		while (std::getline(proc.out(), line))
    	std::cout << "result \n" << line;*/
	    /*std::string result = system( "g++ -I/usr/local/include `pkg-config --cflags --libs tesseract` ip.cpp" ) ;
        std::cout << "result \n"<<result << std::endl;*/
        //temp1(myROI).copyTo(temp2);
        /*for(int k=0;k<100;k++)
        {
        	for(int j=0;j<100;j++)
        		temp2[k][j]=temp1[k][j];
        }*/
        resize(temp2, temp3, Size(100,100), 0, 0, INTER_CUBIC);

        temp4=temp3;
        inRange(temp4,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),temp5);
        findContours( temp5, Contours, Hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
        Mat temp6(temp5.rows, temp5.cols, CV_8UC3,Scalar(255,255,255));
        for( int Idx=0; Idx >= 0; Idx = Hierarchy[Idx][0] )
        	drawContours( temp6, Contours, Idx, Scalar(0,0,0), CV_FILLED, 8, Hierarchy );

        //morphOps(temp2);
        //Mat grHistrogram(301,260,CV_8UC1,Scalar(0,0,0));
        Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
        /*dilate(temp6,temp6,erodeElement);
        dilate(temp6,temp6,erodeElement);
        dilate(temp6,temp6,erodeElement);
        dilate(temp6,temp6,erodeElement);
        */

        /*Mat temp7 = Mat::zeros(temp.rows, temp5.cols, CV_8UC3);
        
        src.copyTo(dst(Rect(left, top, src.cols, src.rows)));
*/
        imshow("temp6",temp6);


        tess.SetImage((uchar*)temp6.data, temp6.size().width, temp6.size().height, temp6.channels(), temp6.step1());
	    char* out = tess.GetUTF8Text();
	    std::cout << "tess \n"<<out << std::endl;

        waitKey(500);
        
        switch(i)
        {
        	case 1: ROI1=temp5;
        			imwrite( "1.jpg", temp6 );
        		break;
        	case 2:ROI2=temp5;
        			imwrite( "2.jpg", temp6 );
        		break;
        	case 3:ROI3=temp5;
        			imwrite( "3.jpg", temp6 );
        		break;
        	case 4:ROI4=temp5;
        			imwrite( "4.jpg", temp6 );
        		break;
        }
    }

    //imshow("cropped1", ROI1 );
    //imshow("cropped2",ROI2);
    //imshow("cropped3",ROI3);
    //imshow("cropped4",ROI4);

    

    /*tess.SetImage((uchar*)dst.data, dst.cols, dst.size().height, 1, dst.cols);
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;*/

    namedWindow( "Components", 1 );
    imshow( "Components", dst );
    resize(src, src, Size(1000,1000), 0, 0, INTER_CUBIC);
    imshow("src",src);	

	//cameraFeed.copyTo(im_with_keypoints);
	//findContours(keypoints,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	CvSeq* seq;  
	CvMemStorage* storage = cvCreateMemStorage( 0 );  
	  
	cvClearMemStorage( storage );  
	  
	// cvFindContours the 12 + 1 extra object  
	// for white backgrounds and black spots, hence  
	// subtract 1  
	/*num_blobs = cvFindContours( img_bw,  
	                storage,  
	                &seq,  
	                sizeof( CvContour ),  
	                CV_RETR_LIST,  
	                CV_CHAIN_APPROX_NONE,  
	                cvPoint( 0, 0 ) ) - 1; */ 

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	
	//drawKeypoints( cameraFeed, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DEFAULT );
	//drawKeypoints(cameraFeed, keypoints, cameraFeed, Scalar(0,0,255), DrawMatchesFlags::DEFAULT ); 
	 
	// Show blobs
	//imshow("keypoints", im_with_keypoints );
	waitKey(5);
}
	
int main(int argc, char* argv[])
{
	//some boolean variables for different functionality within this
	//program
    bool trackObjects = true;
    bool useMorphOps = true;
    bool blobDetect = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//matrix storage for blob marked image
	Mat blobImage;
	//x and y values for the location of the object
	int x=0, y=0;
	//create slider bars for HSV filtering
	createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	///capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	///capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		///capture.read(cameraFeed);
		cameraFeed = imread("/home/aytas32/Desktop/ass.jpg", CV_LOAD_IMAGE_UNCHANGED);;
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV,Scalar(0,0,0),Scalar(0,0,80),threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if(useMorphOps)
			morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if(trackObjects)
			trackFilteredObject(x,y,threshold,cameraFeed);

		//
		//
		if(blobDetect)
			blobDetection(threshold,cameraFeed);

		//show frames 
		imshow(windowName2,threshold);
		imshow(windowName,cameraFeed);
		imshow(windowName1,HSV);
		

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}






	return 0;
}
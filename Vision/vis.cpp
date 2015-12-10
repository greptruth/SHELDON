#include "vis.hpp"
#include <string>

using namespace cv;
using namespace std;

const int MAX_ =256;
const int MIN_ = 0;

int H_MIN_MARK = 0;
int H_MAX_MARK = 256;
int S_MIN_MARK = 0;
int S_MAX_MARK = 256;
int V_MIN_MARK = 0;
int V_MAX_MARK = 60;
int ErsMarker = 3;
int DilMarker = 3;

int H_MIN_START = 35;
int H_MAX_START = 95;
int S_MIN_START = 0;
int S_MAX_START= 256;
int V_MIN_START = 0;
int V_MAX_START = 256;
int ErsStart = 5;
int DilStart = 4;

int H_MIN_DIGITS = 0;
int H_MAX_DIGITS = 256;
int S_MIN_DIGITS = 0;
int S_MAX_DIGITS = 256;
int V_MIN_DIGITS = 0;
int V_MAX_DIGITS = 256;


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
const string trackbarWindowName = "Trackbars";

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}

void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN_DIGITS %d", H_MIN_DIGITS);
	sprintf( TrackbarName, "H_MAX_DIGITS %d", H_MAX_DIGITS);
	sprintf( TrackbarName, "S_MIN_DIGITS %d", S_MIN_DIGITS);
	sprintf( TrackbarName, "S_MAX_DIGITS %d", S_MAX_DIGITS);
	sprintf( TrackbarName, "V_MIN_DIGITS %d", V_MIN_DIGITS);
	sprintf( TrackbarName, "V_MAX_DIGITS %d", V_MAX_DIGITS);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN_DIGITS", trackbarWindowName, &H_MIN_DIGITS, MAX_, on_trackbar );
    createTrackbar( "H_MAX_DIGITS", trackbarWindowName, &H_MAX_DIGITS, MAX_, on_trackbar );
    createTrackbar( "S_MIN_DIGITS", trackbarWindowName, &S_MIN_DIGITS, MAX_, on_trackbar );
    createTrackbar( "S_MAX_DIGITS", trackbarWindowName, &S_MAX_DIGITS, MAX_, on_trackbar );
    createTrackbar( "V_MIN_DIGITS", trackbarWindowName, &V_MIN_DIGITS, MAX_, on_trackbar );
    createTrackbar( "V_MAX_DIGITS", trackbarWindowName, &V_MAX_DIGITS, MAX_, on_trackbar );

    //char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN_MARK %d", H_MIN_MARK);
	sprintf( TrackbarName, "H_MAX_MARK %d", H_MAX_MARK);
	sprintf( TrackbarName, "S_MIN_MARK %d", S_MIN_MARK);
	sprintf( TrackbarName, "S_MAX_MARK %d", S_MAX_MARK);
	sprintf( TrackbarName, "V_MIN_MARK %d", V_MIN_MARK);
	sprintf( TrackbarName, "V_MAX_MARK %d", V_MAX_MARK);
	sprintf(TrackbarName,"Marker Erosion No %d",ErsMarker);
	sprintf(TrackbarName,"Marker Dilation No %d",DilMarker);	

    createTrackbar( "H_MIN_MARK", trackbarWindowName, &H_MIN_MARK, MAX_, on_trackbar );
    createTrackbar( "H_MAX_MARK", trackbarWindowName, &H_MAX_MARK, MAX_, on_trackbar );
    createTrackbar( "S_MIN_MARK", trackbarWindowName, &S_MIN_MARK, MAX_, on_trackbar );
    createTrackbar( "S_MAX_MARK", trackbarWindowName, &S_MAX_MARK, MAX_, on_trackbar );
    createTrackbar( "V_MIN_MARK", trackbarWindowName, &V_MIN_MARK, MAX_, on_trackbar );
    createTrackbar( "V_MAX_MARK", trackbarWindowName, &V_MAX_MARK, MAX_, on_trackbar );
    createTrackbar( "Marker Erosion", trackbarWindowName, &ErsMarker, 5, on_trackbar );
    createTrackbar( "Marker Dilation", trackbarWindowName, &DilMarker, 5, on_trackbar );

    sprintf( TrackbarName, "H_MIN_START %d", H_MIN_START);
	sprintf( TrackbarName, "H_MAX_START %d", H_MAX_START);
	sprintf( TrackbarName, "S_MIN_START %d", S_MIN_START);
	sprintf( TrackbarName, "S_MAX_START %d", S_MAX_START);
	sprintf( TrackbarName, "V_MIN_START %d", V_MIN_START);
	sprintf( TrackbarName, "V_MAX_START %d", V_MAX_START);
	sprintf(TrackbarName,"Starter Erosion No %d",ErsStart);
	sprintf(TrackbarName,"Starter Dilation No %d",DilStart);	


	createTrackbar( "H_MIN_START", trackbarWindowName, &H_MIN_START, MAX_, on_trackbar );
    createTrackbar( "H_MAX_START", trackbarWindowName, &H_MAX_START, MAX_, on_trackbar );
    createTrackbar( "S_MIN_START", trackbarWindowName, &S_MIN_START, MAX_, on_trackbar );
    createTrackbar( "S_MAX_START", trackbarWindowName, &S_MAX_START, MAX_, on_trackbar );
    createTrackbar( "V_MIN_START", trackbarWindowName, &V_MIN_START, MAX_, on_trackbar );
    createTrackbar( "V_MAX_START", trackbarWindowName, &V_MAX_START, MAX_, on_trackbar );
    createTrackbar( "Starter Erosion", trackbarWindowName, &ErsStart, 5, on_trackbar );
    createTrackbar( "Starter Dilation", trackbarWindowName, &DilStart, 5, on_trackbar );




}

string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

void drawObject(int x, int y,Mat &frame)
{

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

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{

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

void morphOps(Mat &thresh,int erd,int dil)
{

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	for(int i=0;i<erd;i++)
		erode(thresh,thresh,erodeElement);


	for(int j=0;j<dil;j++)
		dilate(thresh,thresh,dilateElement);
	
}

void markerDetection(Mat HSV,Mat cameraFeed)
{
	std::cout<<"I'm a dick 1 \n";
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

	//Setting tesseract parameters
	const char* lang = "eng";
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, lang, tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    std::cout<<"I'm a dick 2 \n";

    //thresholding on HSV for markers
    Mat MarkerPlates;
    inRange(HSV,Scalar(H_MIN_MARK,S_MIN_MARK,V_MIN_MARK),Scalar(H_MAX_MARK,S_MAX_MARK,V_MAX_MARK),MarkerPlates);
    morphOps(MarkerPlates,ErsMarker,DilMarker);
    imshow("MarkerPlates",MarkerPlates);
    std::cout<<"I'm a dick 3 \n";
        



	Mat dst = Mat::zeros(HSV.rows, HSV.cols, CV_8UC3);//dst stores markerplates after threshold ansd countour detect
	vector< vector<Point> > contours,Contours;
	vector<Vec4i> hierarchy,Hierarchy;
	findContours( MarkerPlates, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	std::cout<<"I'm a dick 4\n";

	if(!MarkerPlates.empty())
    for( int i=1,idx=0; idx >= 0; idx = hierarchy[idx][0],i++ )
    {
    	/*if(MarkerPlates.empty())
    		break;*/
        Scalar color( 255,255,255);
        std::cout<<"I'm a dick 4a\n";
        drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
        std::cout<<"I'm a dick 4b\n";
        /*if(boundingRect(contours[idx]))
        	break;*/
		Rect R=boundingRect(contours[idx]);
		std::cout<<"I'm a dick 4c\n";
        cv::Rect myROI(100,100,100,100);
        Mat temp1,temp2,temp3,temp4,temp5;///this is too much Mat
        std::cout<<"I'm a dick 5 \n";


        //getting the markers from feild
        temp1 = cameraFeed(R);
        ///need to see temp1 too in real situation
        resize(temp1, temp2, Size(300,300), 0, 0, INTER_CUBIC);
        temp1=temp2.clone();
        temp2 = temp1(myROI);
        if(!temp2.empty())
			imshow("temp2",temp2);
        
        //tesseract output from directly bounding rectangle
        tess.SetImage((uchar*)temp2.data, temp2.size().width, temp2.size().height,temp2.channels(), temp2.step1());
	    char* out1 = tess.GetUTF8Text();
	    std::cout << "tess intermediate \n"<<out1 << std::endl;
	    

	    //further processing for real situations
        resize(temp2, temp3, Size(100,100), 0, 0, INTER_CUBIC);
		temp4=temp3;
        inRange(temp4,Scalar(H_MIN_DIGITS,S_MIN_DIGITS,V_MIN_DIGITS),Scalar(H_MAX_DIGITS,S_MAX_DIGITS,V_MAX_DIGITS),temp5);
        findContours( temp5, Contours, Hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
        Mat temp6(temp5.rows, temp5.cols, CV_8UC3,Scalar(255,255,255));
        for( int Idx=0; Idx >= 0; Idx = Hierarchy[Idx][0] )
        	drawContours( temp6, Contours, Idx, Scalar(0,0,0), CV_FILLED, 8, Hierarchy );

       	//final processed markers being displayed
       	if(!temp6.empty())
        	imshow("temp6",temp6);

        //tesseract data from processed image
        tess.SetImage((uchar*)temp6.data, temp6.size().width, temp6.size().height, temp6.channels(), temp6.step1());
	    char* out = tess.GetUTF8Text();
	    std::cout << "tess \n"<<out << std::endl;

        waitKey(1);
        
        
    }


    //marker contours are drawn on dst
    namedWindow( "Components", 1 );
    if(!dst.empty())
    	imshow( "Components", dst );	

	
	waitKey(5);
}

void startDetection(Mat HSV,Mat cameraFeed)
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


	Mat startPlate;
    inRange(HSV,Scalar(H_MIN_START,S_MIN_START,V_MIN_START),Scalar(H_MAX_START,S_MAX_START,V_MAX_START),startPlate);
    morphOps(startPlate,ErsStart,DilStart);
    imshow("startPlate",startPlate);
    cout<<"**************************************************************************************";
}



int main(int argc, char* argv[])
{
	//some boolean variables for different functionality within this
	//program
    bool markersDetect = true;
    bool botDetect = false;
    bool startDetect = true;
    bool endDetect = false;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
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
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object

		//
		//
		if(markersDetect)
			markerDetection(HSV,cameraFeed);
		if(startDetect)
			startDetection(HSV,cameraFeed);

		//show frames 
		imshow(windowName,cameraFeed);
		imshow(windowName1,HSV);
		

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}






	return 0;
}




void showImages()
{


}
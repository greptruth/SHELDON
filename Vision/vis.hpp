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
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

using namespace cv;
using namespace std;


void on_trackbar( int, void* );
void createTrackbars();

string intToString(int);

void drawObject(int x, int y,Mat &frame);

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);

void morphOps(Mat &thresh,int erd,int dil);

void markerDetection(Mat HSV,Mat cameraFeed);

void startDetection(Mat HSV,Mat cameraFeed);

void endDetection(Mat HSV,Mat cameraFeed);

void botDetection(Mat HSV,Mat cameraFeed);


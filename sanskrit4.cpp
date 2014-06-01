/*
ayoungprogrammer.blogspot.com

Part 1: Extracting contours from text

*/

#include <iostream>

#include <stdio.h>
#include <opencv2/opencv.hpp>

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"

#include <stdlib.h>
#include <ctype.h>
#endif

using namespace std;
using namespace cv;


 
int main( int argc, char** argv )
{


IplImage *img = cvLoadImage("./1.jpg");
 
IplImage *gray = cvCreateImage(cvSize(img->width,img->height),8,1);
 
cvCvtColor(img,gray,CV_RGB2GRAY);
cvNamedWindow("Image:",1);
cvShowImage("Image:", img);
 
cvNamedWindow("Gray:",1);
cvShowImage("Gray:", gray);
 
//create a rectangular area
CvRect rect = cvRect(0,0,500,600);
//CvRect rect = cvRect(x,y,width,height)
 
//apply the rectangular to the image and establish a region of interest
cvSetImageROI(gray,rect);
//gray=pointer to the image(gray pointer)
//rect=nameROI
 
float range[]={0,255};
float *ranges[]={range};
int hist_size = 256;
 
//create an image to hold the histogram
IplImage*histImage = cvCreateImage(cvSize(320,200),8,1);
//create a histogram to the information from the image
CvHistogram *hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
//1=no of dimentions
//&hist_size=size of dimention
//CV_HIST_ARRY=how to store data in histogram(multi dimentional array)
//ranges=ranges of values for dimention(0,255)
//1=uniform flag(sub intervals should be same)
 
//calculate the histogram
cvCalcHist(&gray,hist,0,NULL);
//&gray=source
//hist=pointer to the histogram
//0=accumilation flag(if set,hist is not clear at the begining)
//NULL=operational mask
 
float min_value,max_value=0;
int min_idx,max_idx=0;
//Grab Min/Max Values
cvGetMinMaxHistValue(hist,&min_value,&max_value,&min_idx,&max_idx);
//hist=pointer to the histogram
 
//Scale the bin values to fit to image representation
//remove higher values
cvScale(hist->bins, hist->bins,((double)histImage->height)/max_value,0);
 
//hist->bins=source to be scared
//hist->bins=destination
//use of same vales is manupilate source values and put them in the same location
 
//Set Up Factors For Visualization(set all histogram values to 255)
cvSet(histImage,cvScalarAll(255),0);
//create a factor for scaling along the width
int bin_w=cvRound((double)histImage->width/hist_size);
 
//draw the values
int i;
for(i=0;i<hist_size;i++)
{
//draw the histogram data on to the histogram image
cvRectangle(histImage,cvPoint(i*bin_w,histImage->height),cvPoint((i+1)*bin_w,histImage->height-cvRound(cvGetReal1D(hist->bins,i))),cvScalarAll(0),-1,8,0);
//cvScalarAll(0)=draw in black color
//-1=thickness of the line(filled rectangal)
//8=line type(connected)
//0=no of fractional points
}
 
cvNamedWindow("Histogram:",1);
cvShowImage("Histogram:", histImage);
 
cvWaitKey(0);
cvDestroyWindow("Image:");
cvReleaseImage(&img);
 
cvDestroyWindow("Gray:");
cvReleaseImage(&gray);
 
cvDestroyWindow("Histogram:");
cvReleaseImage(&histImage);
 
return 0;
 
}
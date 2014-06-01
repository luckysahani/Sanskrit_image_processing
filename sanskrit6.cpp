#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include <stdlib.h>
#include <ctype.h>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	Mat src, dst;
    src = imread( argv[1], 1 );
    cv::imshow("image",src);
	if( !src.data )
	{ return -1; }
	int start=0,end=0,prev=255,sum;
	for(int i=0; i<src.rows; i++)
 { 
 	sum=0;
 	for(int j=0; j<src.cols; j++)
    {
      	if(src.at<uchar>(i,j)<120)
      	{
      		sum=sum+0;
      	}
      	else
      	{
      		sum=sum+255;
      	}
     }
     sum=sum/src.cols;
     cout<<i<<" "<<endl;
     if(sum==0)
     	cout<<"no line "<<sum<<endl;
     else
     	cout<< " "<<sum<<endl;

     if(sum<255)
     {
     	if(start==0)
     	start = i;
 	}
 	else
 	{
 		if(prev!=255&&start>0)
 		{
 			end= i-1;
 			if(start!=end)
 			{		
 			cout<<"Hell"<<start<<"  "<<end<<endl; 
 			cv::Mat imageROI;
			imageROI= src(cv::Rect(0,start,src.cols,end-start));
			cv::imshow("src",imageROI);
  			waitKey(0);
  		}
 		}
 		start = 0;
 		end=0;
 	}
 	prev=sum;	
 }

	//Histogrm_l
	vector<Mat> bgr_planes;
	split( src, bgr_planes );
	int histSize = 256;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	Mat b_hist, g_hist, r_hist;
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	for( int i = 1; i < histSize; i++ )
	{
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
	                   Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
	                   Scalar( 255, 0, 0), 2, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
	                   Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
	                   Scalar( 0, 255, 0), 2, 8, 0  );
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
	                   Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
	                   Scalar( 0, 0, 255), 2, 8, 0  );
	}
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );
	waitKey(0);
	return 0;
}

#include <iostream>
#include <opencv2/opencv.hpp>
#include <imgproc\imgproc.hpp>  
#include <highgui\highgui.hpp>  
#include <photo\photo.hpp>  
#include "findContours.cpp";
#include "ImageRepair.cpp";
using namespace cv;

int main(int argc, char const *argv[]) {


	ImageRepair myimageRepair;
	
	
	

	//myimageRepair.
	
	FindContours test;
	
	test.myconvertToHSV("Result.JPG");
	//test.custumizrMask("result222.jpg");

	//test.inpaintingwithmask("result2.jpg","mask.jpg");
	
	//test.find("East.png");
	//test.convertToHSV("North52result2.jpg");
	//Mat imageSource = imread("beauty.JPG");
	//test.myconvertToHSV("AfterRepiar3.jpg");
	//test.findwithMat(imageSource);
	//test.colorTransfer("East.png", "52.png");
	//Mat result = test.equalizeIntensity(imageSource);
	//imshow("Source", imageSource);
	//imshow("Result", result);
	//waitKey();
	
	return 0;

}
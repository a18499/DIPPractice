#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace cv;

int main(){
  Mat inputImg = imread("Inputimage1.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat inputImg2 = imread("selfy5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  
  //OptimalDFT
   Mat padded;
   int m = getOptimalDFTSize(inputImg.rows); 
   int n = getOptimalDFTSize(inputImg.cols);
   copyMakeBorder(inputImg, padded, 0, m-inputImg.rows, 0, n-inputImg.cols, BORDER_CONSTANT, Scalar::all(0)); 
   
   //OptimalDFT
    Mat padded2;
    int m2 = getOptimalDFTSize(inputImg2.rows);  
    int n2= getOptimalDFTSize(inputImg2.cols);
    copyMakeBorder(inputImg2, padded2, 0, m2-inputImg2.rows, 0, n2-inputImg2.cols, BORDER_CONSTANT, Scalar::all(0)); 

    //preallocate
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    //getdft
    dft(complexImg, complexImg);

   
    Mat planes2[] = {Mat_<float>(padded2), Mat::zeros(padded2.size(), CV_32F)};
    Mat complexImg2;
    merge(planes2, 2, complexImg2);
     //getdft2
    dft(complexImg2, complexImg2);

    split(complexImg2,planes2); //分離通道，planes[0]為實數部分，planes[1]為虛數部分
  
    split(complexImg, planes);   //分離通道，planes[0]為實數部分，planes[1]為虛數部分
    
    //Save parameter
    Mat rPart1,iPart1,rPart2,iPart2;
    planes[0].copyTo(rPart1);
    planes[1].copyTo(iPart1);
    planes2[0].copyTo(rPart2);
    planes2[1].copyTo(iPart2);

    //cal magnitude 
  
    magnitude(planes[0], planes[1], planes[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    magnitude(planes2[0], planes2[1], planes2[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    //Save origmagnitude
    Mat origmagnitude1 = planes[0].clone();
    Mat origmagnitude2 = planes2[0].clone();
    Mat magnitude1 = planes[0].clone();//magnitude
    Mat magnitude2 = planes2[0].clone();//magnitude2

    //move to orign
    magnitude1 += Scalar::all(1); //magI = log(1+planes[0])
    magnitude2 += Scalar::all(1);
    log(magnitude1, magnitude1);
    log(magnitude2,magnitude2);

    magnitude1 = magnitude1(Rect(0, 0, magnitude1.cols & -2, magnitude1.rows & -2));  
    magnitude2 = magnitude2(Rect(0, 0, magnitude2.cols & -2, magnitude2.rows & -2)); 

    int cx = magnitude1.cols/2;
    int cy = magnitude1.rows/2;

    int px = magnitude2.cols/2;
    int py = magnitude2.rows/2;

    Mat q0(magnitude1, Rect(0, 0, cx, cy));
    Mat q1(magnitude1, Rect(cx, 0, cx, cy));
    Mat q2(magnitude1, Rect(0, cy, cx, cy));
    Mat q3(magnitude1, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    Mat p0(magnitude2, Rect(0, 0, px, py));
    Mat p1(magnitude2, Rect(px, 0, px, py));
    Mat p2(magnitude2, Rect(0, py, px, py));
    Mat p3(magnitude2, Rect(px, py, px, py));



    Mat tmp2;
    p0.copyTo(tmp2);
    p3.copyTo(p0);
    tmp2.copyTo(p3);
    p1.copyTo(tmp2);
    p2.copyTo(p1);
    tmp2.copyTo(p2);



    // normalize to humanreadable
    normalize(magnitude1, magnitude1, 0, 1, CV_MINMAX);
   // normalize to humanreadable
    normalize(magnitude2, magnitude2, 0, 1, CV_MINMAX);

    //cal phase1
    Mat magn1,angle1,oriangle1,orimagn1;
    cartToPolar(rPart1,iPart1,magn1,angle1,true);
    //Save angle1
    angle1.copyTo(oriangle1);
    //normalize to human readable
    normalize(angle1, angle1, 255,0);

    //cal phase2
    Mat magn2,angle2,oriangle2,orimagn2;
    cartToPolar(rPart2,iPart2,magn2,angle2,true);
    //Save angle2
    angle2.copyTo(oriangle2);
    //normalize to human readable
    normalize(angle2, angle2, 255,0);

    //Show Result
    imshow("InputImage1", inputImg);
    imshow("InputImage2", inputImg2);
    imshow("mangtidue1", magnitude1);
    imshow("mangtidue2", magnitude2);
    imshow("Phase1", angle1);
    imshow("Phase2", angle2);

    //Switch phase
    Mat newplanes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat newplanes2[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};

   //mangtidue1+Phase2;
    polarToCart(magn1,oriangle2,newplanes[0],newplanes[1],true);

    //mangtidue2+Phase1;
    polarToCart(magn2,oriangle1,newplanes2[0],newplanes2[1],true);

    Mat newComplex1,newComplex2;
    merge(newplanes, 2, newComplex1);
      merge(newplanes2, 2, newComplex2);
    //Merge

    //逆向傅立葉轉換
     Mat ifft;
    idft(newComplex1,ifft,DFT_INVERSE|DFT_REAL_OUTPUT);
    
    normalize(ifft,ifft,0,1,CV_MINMAX);
    imshow("mangtidue1+Phase2",ifft);

    Mat ifft2;
    idft(newComplex2,ifft2,DFT_REAL_OUTPUT);
    normalize(ifft2,ifft2,0,1,CV_MINMAX);
    imshow("mangtidue2+Phase1",ifft2);

  waitKey(0);

  return 0;
}
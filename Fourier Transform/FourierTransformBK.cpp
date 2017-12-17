#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace cv;



int main(){
    Mat inputImg = imread("Inputimage1.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat inputImg2 = imread("selfy5.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    Mat padded;
    int m = getOptimalDFTSize(inputImg.rows); 
    int n = getOptimalDFTSize(inputImg.cols);
    copyMakeBorder(inputImg, padded, 0, m-inputImg.rows, 0, n-inputImg.cols, BORDER_CONSTANT, Scalar::all(0)); 

    Mat padded2;
    int m2 = getOptimalDFTSize(inputImg2.rows);  
    int n2= getOptimalDFTSize(inputImg2.cols);
    copyMakeBorder(inputImg2, padded2, 0, m2-inputImg2.rows, 0, n2-inputImg2.cols, BORDER_CONSTANT, Scalar::all(0)); 


    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);

    Mat planes2[] = {Mat_<float>(padded2), Mat::zeros(padded2.size(), CV_32F)};
    Mat complexImg2;
    merge(planes2, 2, complexImg2);
    dft(complexImg2, complexImg2);

    split(complexImg2,planes2);
  
    split(complexImg, planes);   //分離通道，planes[0]為實數部分，planes[1]為虛數部分
    Mat oriPhase1;
    planes[1].copyTo(oriPhase1);
    Mat originPhase;
    phase(planes[0],planes[1],originPhase,true);
    Mat oriPhase2;
    planes2[1].copyTo(oriPhase2);
    Mat originalImage1RealPart;
    planes[0].copyTo(originalImage1RealPart);
    Mat originalImage2RealPart = planes2[0].clone();
    magnitude(planes[0], planes[1], planes[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    magnitude(planes2[0], planes2[1], planes2[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    Mat origmagnitude = planes[0].clone();
    Mat magnitude1 = planes[0].clone();//magnitude
    Mat magnitude2 = planes2[0].clone();//magnitude2
   
  
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




    normalize(magnitude1, magnitude1, 0, 1, CV_MINMAX);

    normalize(magnitude2, magnitude2, 0, 1, CV_MINMAX);

    //cal phase
    Mat magn,angle,oriangle,orimagn;
    cartToPolar(originalImage2RealPart,oriPhase2,magn,angle,true);
    angle.copyTo(oriangle);
   magn.copyTo(orimagn);
    Mat magn1,angle1,oriangle1,orimagn1;
    cartToPolar(originalImage1RealPart,oriPhase1,magn1,angle1,true);
    normalize(angle, angle, 255,0);
    log(magn, magn);
    normalize(magn, magn, 0, 1, CV_MINMAX);
    Mat complexx = magn.mul(angle);
  
    //Switch phase
    Mat newplanes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    //newplanes[0]= originalImage1RealPart.clone();
    //magn.copyTo(newplanes[0]);
    oriangle.copyTo(newplanes[1]);
    Mat newplanes2[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    originalImage2RealPart.copyTo(newplanes2[0]);
    oriPhase1.copyTo(newplanes2[1]);
    Mat newcomplexImg;
    newcomplexImg = magn.mul(oriangle);
    // merge(newplanes,2, newcomplexImg);

     Mat newcomplexImg2;
    merge(newplanes2,2, newcomplexImg2);
    /*Mat tempPhase;
    planes[1].copyTo(tempPhase);
    planes[1].release();
    planes[1]=planes2[1].clone();
    planes2[1].release();
    planes2[1] = tempPhase.clone();
    
    Mat newcomplexImg;
    merge(newplanes,2, newcomplexImg);
  
    merge(planes2, 2, complexImg2);*/
 normalize(originPhase, originPhase, 255,0);
    imshow("InputImage1", inputImg);
    imshow("InputImage2", inputImg2);
    imshow("mangtidue1", magnitude1);
    imshow("mangtidue2", magnitude2);
    imshow("Phase1", originPhase);
    imshow("Phase2", planes2[1]);
    
    Mat afterExp;
    Mat aftermulti;
    aftermulti = originPhase*sqrt(-1);
    exp(aftermulti,afterExp);
    Mat aftermulti2 = origmagnitude.mul(afterExp);
    Mat newreal,newimage;
    Mat newComplexxx;
    polarToCart(orimagn,angle1,newplanes[0],newplanes[1],true);
    merge(newplanes, 2, newComplexxx);
    //逆向傅立葉轉換
    Mat ifft;
    idft(newComplexxx,ifft,DFT_INVERSE|DFT_REAL_OUTPUT);
    
    normalize(ifft,ifft,0,1,CV_MINMAX);
    imshow("mangtidue1+Phase2",ifft);

    Mat ifft2;
    idft(complexImg2,ifft2,DFT_REAL_OUTPUT);
    normalize(ifft2,ifft2,0,1,CV_MINMAX);
    imshow("mangtidue2+Phase1",ifft2);



    waitKey(0);

    // Read image from file
    // Make sure that the image is in grayscale



    return 0;
}

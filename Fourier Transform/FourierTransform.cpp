#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace cv;



int main(){
    Mat inputImg = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
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

    magnitude(planes[0], planes[1], planes[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    magnitude(planes2[0], planes2[1], planes2[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    Mat magI = planes[0];//magnitude
    Mat phaI = planes2[0];//magnitude2
    magI += Scalar::all(1); //magI = log(1+planes[0])
    phaI += Scalar::all(1);
    log(magI, magI);
    log(phaI,phaI);

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));  
    phaI = phaI(Rect(0, 0, phaI.cols & -2, phaI.rows & -2)); 

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    int px = phaI.cols/2;
    int py = phaI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    Mat p0(phaI, Rect(0, 0, px, py));
    Mat p1(phaI, Rect(px, 0, px, py));
    Mat p2(phaI, Rect(0, py, px, py));
    Mat p3(phaI, Rect(px, py, px, py));



    Mat tmp2;
    p0.copyTo(tmp2);
    p3.copyTo(p0);
    tmp2.copyTo(p3);
    p1.copyTo(tmp2);
    p2.copyTo(p1);
    tmp2.copyTo(p2);




    normalize(magI, magI, 0, 1, CV_MINMAX);

    normalize(phaI, phaI, 0, 1, CV_MINMAX);




    Mat tempPhase;
    planes[1].copyTo(tempPhase);
    planes[1].release();
    planes2[1].copyTo(planes[1]);
    planes2[1].release();
    planes2[1] = tempPhase.clone();

    merge(planes, 2, complexImg);
  
    merge(planes2, 2, complexImg2);

    imshow("InputImage1", inputImg);
    imshow("InputImage2", inputImg2);
    imshow("mangtidue1", magI);
    imshow("mangtidue2", phaI);
    imshow("Phase1", planes[1]);
    imshow("Phase2", planes2[1]);
    

    //逆向傅立葉轉換
    Mat ifft;
    idft(complexImg,ifft,DFT_REAL_OUTPUT);
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

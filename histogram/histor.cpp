#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;


void drawHistImg(const Mat &src, Mat &dst){
    int histSize = 256;
    float histMaxValue = 0;
    for(int i=0; i<histSize; i++){
        float tempValue = src.at<float>(i);
        if(histMaxValue < tempValue){
            histMaxValue = tempValue;
        }
    }

    float scale = (0.9*256)/histMaxValue;
    for(int i=0; i<histSize; i++){
        int intensity = static_cast<int>(src.at<float>(i)*scale);
        line(dst,Point(i,255),Point(i,255-intensity),Scalar(0));
    }
}



void historgram_specification(Mat image1 ,Mat image2 ,Mat histogram1,Mat histogram2){
  int grayLevel = 256;
  float* prImg1 =new float[grayLevel];
  float* mapImg1 =new float[grayLevel];
  float* prImg2 =new float[grayLevel];
  float mn = image1.rows * image1.cols;
  float mn2 = image2.rows*image2.cols;


//calculate pr(r)
  for(int i = 0 ; i < grayLevel ; i++)
  {
   float nk = histogram1.at<float>(i);
   prImg1[i] = nk / mn;//Pr(rk) of image1
   nk = histogram2.at<float>(i);
   prImg2[i] = nk / mn2; //Pr(rk) of image2
   mapImg1[i] = 0;//fill 0 to target map
  }
  printf("Continue\n");
  for(int i = 1 ; i < grayLevel ; i++)
  {
    prImg1[i] = prImg1[i - 1] + prImg1[i];
    prImg2[i] = prImg2[i - 1] + prImg2[i];
  }
  prImg1[grayLevel - 1 ] = prImg2[grayLevel - 1 ] = 1;
  printf("Mapping\n");
  Mat img_new = image1.clone();
  int currentID = 0;
  for(int i = 1 ; i < grayLevel ; i++)
  {
   bool mapped =false;
   while(mapped ==false)
   {
     if( prImg1[i] <= prImg2[ currentID ] )
     {
        mapImg1[i] = currentID;
        mapped =true;
      }else {
        currentID++;
      }
     }
   }
   printf("New image");
   // New Image
   for(int i = 0 ; i < img_new.rows ; i++)
   {
     unsigned char* data = img_new.ptr<unsigned char>(i);
     for(int j = 0 ; j < img_new.cols ; j++){
      int grayValue = (int)data[j * img_new.channels() + 0];
      float val = mapImg1[ grayValue ];
      data[j * img_new.channels() + 0 ] = val;
    }
  }
  Mat histimagenew;
  int histSize = 256;
  float range[] = {0, 255};
  const float* histRange = {range};
  Mat showHistImg3(256,256,CV_8UC1,Scalar(255));
  calcHist(&img_new, 1, 0, Mat(), histimagenew, 1, &histSize, &histRange);
  drawHistImg(histimagenew, showHistImg3);
  // HISTOGRAM AFTER
  //namedWindow("Histogram IMAGE NEW");
  //imshow("Histogram IMAGE NEW",h.getHistogramImage(img_new));
  //---Image After
  namedWindow("IMAGE NEW");
  imshow("IMAGE NEW",img_new);
  namedWindow("IMAGE NEW Hist");
  imshow("IMAGE NEW Hist" ,showHistImg3);
  cv::imwrite("newimage.jpg", img_new);

}

//Testing image is 825*430
int main(int argc, char** argv )
{

  if ( argc !=  3)
  {
      printf("usage: hist <Image_Path1> <Image_Path2>\n");
      return -1;
  }

  int histSize = 256;
  Mat image ,histImg1 , image2, histImg2;
  float range[] = {0, 255} ;
  const float* histRange = {range};
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  image2 = imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);
  calcHist(&image, 1, 0, Mat(), histImg1, 1, &histSize, &histRange);
  calcHist(&image2, 1, 0, Mat(), histImg2, 1, &histSize, &histRange);
  Mat showHistImg(256,256,CV_8UC1,Scalar(255));  //draw a blank images size = 256X256
  Mat showHistImg2(256,256,CV_8UC1,Scalar(255));

  printf("Begin historgram \n");

  historgram_specification(image ,image2,histImg1,histImg2);

  drawHistImg(histImg1, showHistImg);
  drawHistImg(histImg2, showHistImg2);
  namedWindow("image1", WINDOW_AUTOSIZE);
  namedWindow("image2", WINDOW_AUTOSIZE);
  imshow("image1", image);
  imshow("image2", image2);
  imshow("image1 his", showHistImg);
  imshow("image2 his", showHistImg2);
  waitKey(0);
  return 0;


/*

  float range[] = {0, 255} ;
  const float* histRange = {range};
  Mat histImg;
  calcHist(&src, 1, 0, Mat(), histImg, 1, &histSize, &histRange);

  Mat showHistImg(256,256,CV_8UC1,Scalar(255));  //把直方圖秀在一個256*256大的影像上
  drawHistImg(histImg, showHistImg);

*/
}
/*

/ SPECIFICATION
float* prImg1 =new float[grayLevel];
float* mapImg1 =new float[grayLevel];


 float* prImg2 =new float[grayLevel];
 floatn1 = img1.rows * img1.cols;
 floatn2 = img2.rows * img2.cols;
 cout <<"Xac Suat"<< endl;
 // TINH XAC SUAT
 for(inti = 0 ; i < grayLevel ; i++)
 {
  float nk = histogram1.at<float>(i);
  prImg1[i] = nk / n1;
  nk = histogram2.at<float>(i);
  prImg2[i] = nk / n2;mapImg1[i] = 0;
 }
 cout <<"Cong don"<< endl;
 // CONG DON
  for(inti = 1 ; i < grayLevel ; i++)
  {
    prImg1[i] = prImg1[i - 1] + prImg1[i];
    prImg2[i] = prImg2[i - 1] + prImg2[i];
  }
  prImg1[grayLevel - 1 ] = prImg2[grayLevel - 1 ] = 1;
  cout <<"Mapping"<< endl;// MAPPING intcurrent
  ID = 0;
  for(inti = 1 ; i < grayLevel ; i++)
  {
   bool mapped =false;
   while(mapped ==false)
   {
     if( prImg1[i] <= prImg2[ currentID ] )
     {
        mapImg1[i] = currentID;
        mapped =true;
      }else {
        currentID++;
      }
     }
   }
   cout <<"New image"<< endl;
   // New Image
   for(inti = 0 ; i < img_new.rows ; i++)
   {
     uchar* data = img_new.ptr<uchar>(i);
     for(intj = 0 ; j < img_new.cols ; j++){
      int grayValue = (int)data[j * img_new.channels() + 0];
      float val = mapImg1[ grayValue ];
      data[j * img_new.channels() + 0 ] = val;
    }
  }
*/

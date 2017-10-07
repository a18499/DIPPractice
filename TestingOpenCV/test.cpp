#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;



int Binear_interprolation(Mat srcimage,float scale){
  //Src image properties
  uchar* dataSrc = srcimage.data;
  int stepSrc = srcimage.step;
  int iWidthSrc = srcimage.cols;
  int iHiehgtSrc = srcimage.rows;

  //Create Dst image
  Mat matDst1,matDst2;

  matDst1 = cv::Mat(cv::Size(iWidthSrc*scale, iHiehgtSrc*scale), srcimage.type(), cv::Scalar::all(0));
  matDst2 = cv::Mat(matDst1.size(), srcimage.type(), cv::Scalar::all(0));
  double scale_x = (double)srcimage.cols / matDst1.cols;
  double scale_y = (double)srcimage.rows / matDst1.rows;
  printf("in test\n");
  uchar* dataDst = matDst1.data;
  int stepDst = matDst1.step;
  //step[i]:每一维元素的大小，单位字节

  printf("stepSrc: %d\n", stepSrc);
  printf("matDst1 height %d\n", matDst1.rows);
    printf("srcimage height %d\n", iHiehgtSrc);
  for (int each_Rowindex_in_matDst1 = 0; each_Rowindex_in_matDst1 < matDst1.rows; ++each_Rowindex_in_matDst1)
      {
         //printf("numbers %d\n",each_Rowindex_in_matDst1 );
          float unknownfloat_y = (float)((each_Rowindex_in_matDst1 + 0.5) * scale_y - 0.5);
          //get integr
          int srcimage_y = cvFloor(unknownfloat_y);
          unknownfloat_y -= srcimage_y;
          //Select the min value
          srcimage_y = std::min(srcimage_y, iHiehgtSrc - 2);
          //check srcimage_y is a >0 num
          srcimage_y = std::max(0, srcimage_y);

          short cbuunknownfloat_y[2];

          cbuunknownfloat_y[0] = cv::saturate_cast<short>((1.f - unknownfloat_y) * 2048);
          cbuunknownfloat_y[1] = 2048 - cbuunknownfloat_y[0];

          for (int i = 0; i < matDst1.cols; ++i)
          {
              float unknownfloat_x = (float)((i + 0.5) * scale_x - 0.5);
              int srcimage_x = cvFloor(unknownfloat_x);
              unknownfloat_x -= srcimage_x;

              if (srcimage_x < 0) {
                  unknownfloat_x = 0, srcimage_x = 0;
              }
              if (srcimage_x >= iWidthSrc - 1) {
                  unknownfloat_x = 0, srcimage_x = iWidthSrc - 2;
              }

              short cbufx[2];
              cbufx[0] = cv::saturate_cast<short>((1.f - unknownfloat_x) * 2048);
              cbufx[1] = 2048 - cbufx[0];

              for (int k = 0; k < srcimage.channels(); ++k)
              {
                  *(dataDst+ each_Rowindex_in_matDst1*stepDst + 3*i + k) = (*(dataSrc + srcimage_y*stepSrc + 3*srcimage_x + k) * cbufx[0] * cbuunknownfloat_y[0] +
                      *(dataSrc + (srcimage_y+1)*stepSrc + 3*srcimage_x + k) * cbufx[0] * cbuunknownfloat_y[1] +
                      *(dataSrc + srcimage_y*stepSrc + 3*(srcimage_x+1) + k) * cbufx[1] * cbuunknownfloat_y[0] +
                      *(dataSrc + (srcimage_y+1)*stepSrc + 3*(srcimage_x+1) + k) * cbufx[1] * cbuunknownfloat_y[1]) >> 22;
              }
          }
      }
      namedWindow("Display New Image", WINDOW_AUTOSIZE );
      imshow("Display New Image", matDst1);
      cv::imwrite("linear_1.jpg", matDst1);

      cv::resize(srcimage, matDst2, matDst1.size(), 0, 0, 1);
      cv::imwrite("linear_2.jpg", matDst2);

  return 1;
}

int Bicubic_interprolation(Mat srcimage,float scale){


  printf("in bicubic \n");
  //Src image properties
  uchar* dataSrc = srcimage.data;
  int stepSrc = srcimage.step;
  int iWidthSrc = srcimage.cols;
  int iHiehgtSrc = srcimage.rows;
  //Create Dst image
  Mat matDst1,matDst2;

  matDst1 = cv::Mat(cv::Size(iWidthSrc*scale, iHiehgtSrc*scale), srcimage.type(), cv::Scalar::all(0));
  matDst2 = cv::Mat(matDst1.size(), srcimage.type(), cv::Scalar::all(0));

  double scale_x = (double)srcimage.cols / matDst1.cols;
  double scale_y = (double)srcimage.rows / matDst1.rows;


  for (int each_Rowindex_in_matDst1 = 0; each_Rowindex_in_matDst1 < matDst1.rows; ++each_Rowindex_in_matDst1)
  {
    float unknownfloat_y = (float)((each_Rowindex_in_matDst1 + 0.5) * scale_y - 0.5);
    int srcimage_y = cvFloor(unknownfloat_y);
    unknownfloat_y -= srcimage_y;
    //Select the min value
    srcimage_y = std::min(srcimage_y, iHiehgtSrc - 2);
    //check srcimage_y is a >0 num
    srcimage_y = std::max(0, srcimage_y);
    const float A = -0.75f;
    float coeffsrcimage_y[4];
    coeffsrcimage_y[0] = ((A*(unknownfloat_y + 1) - 5*A)*(unknownfloat_y + 1) + 8*A)*(unknownfloat_y + 1) - 4*A;
    coeffsrcimage_y[1] = ((A + 2)*unknownfloat_y - (A + 3))*unknownfloat_y*unknownfloat_y + 1;
    coeffsrcimage_y[2] = ((A + 2)*(1 - unknownfloat_y) - (A + 3))*(1 - unknownfloat_y)*(1 - unknownfloat_y) + 1;
    coeffsrcimage_y[3] = 1.f - coeffsrcimage_y[0] - coeffsrcimage_y[1] - coeffsrcimage_y[2];

    short cbuunknownfloat_y[4];
    cbuunknownfloat_y[0] = cv::saturate_cast<short>(coeffsrcimage_y[0] * 2048);
    cbuunknownfloat_y[1] = cv::saturate_cast<short>(coeffsrcimage_y[1] * 2048);
    cbuunknownfloat_y[2] = cv::saturate_cast<short>(coeffsrcimage_y[2] * 2048);
    cbuunknownfloat_y[3] = cv::saturate_cast<short>(coeffsrcimage_y[3] * 2048);
    for (int i = 0; i < matDst1.cols; ++i)
    {
        float fx = (float)((i + 0.5) * scale_x - 0.5);
        int sx = cvFloor(fx);
        fx -= sx;

        if (sx < 1) {
            fx = 0, sx = 1;
        }
        if (sx >= srcimage.cols - 3) {
            fx = 0, sx = srcimage.cols - 3;
        }

        float coeffsX[4];
        coeffsX[0] = ((A*(fx + 1) - 5*A)*(fx + 1) + 8*A)*(fx + 1) - 4*A;
        coeffsX[1] = ((A + 2)*fx - (A + 3))*fx*fx + 1;
        coeffsX[2] = ((A + 2)*(1 - fx) - (A + 3))*(1 - fx)*(1 - fx) + 1;
        coeffsX[3] = 1.f - coeffsX[0] - coeffsX[1] - coeffsX[2];

        short cbufX[4];
        cbufX[0] = cv::saturate_cast<short>(coeffsX[0] * 2048);
        cbufX[1] = cv::saturate_cast<short>(coeffsX[1] * 2048);
        cbufX[2] = cv::saturate_cast<short>(coeffsX[2] * 2048);
        cbufX[3] = cv::saturate_cast<short>(coeffsX[3] * 2048);

        for (int k = 0; k < srcimage.channels(); ++k)
        {
            matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, i)[k] = abs((srcimage.at<cv::Vec3b>(srcimage_y-1, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[1] +
                srcimage.at<cv::Vec3b>(srcimage_y+1, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[3] +
                srcimage.at<cv::Vec3b>(srcimage_y-1, sx)[k] * cbufX[1] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx)[k] * cbufX[1] * cbuunknownfloat_y[1] +
                srcimage.at<cv::Vec3b>(srcimage_y+1, sx)[k] * cbufX[1] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx)[k] * cbufX[1] * cbuunknownfloat_y[3] +
                srcimage.at<cv::Vec3b>(srcimage_y-1, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[1] +
                srcimage.at<cv::Vec3b>(srcimage_y+1, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[3] +
                srcimage.at<cv::Vec3b>(srcimage_y-1, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[1] +
                srcimage.at<cv::Vec3b>(srcimage_y+1, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[3] ) >> 22);
        }
     }
   }

    namedWindow("Display New Image", WINDOW_AUTOSIZE );
    imshow("Display New Image", matDst1);
    cv::imwrite("cubic_1.jpg", matDst1);

    cv::resize(srcimage, matDst2, matDst1.size(), 0, 0, 2);
    cv::imwrite("cubic_2.jpg", matDst2);

  /*
  int iscale_x = cv::saturate_cast<int>(scale_x);
  int iscale_y = cv::saturate_cast<int>(scale_y);

  for (int each_Rowindex_in_matDst1 = 0; each_Rowindex_in_matDst1 < matDst1.rows; ++each_Rowindex_in_matDst1)
  {
      float unknownfloat_y = (float)((each_Rowindex_in_matDst1 + 0.5) * scale_y - 0.5);
      int srcimage_y = cvFloor(unknownfloat_y);
      unknownfloat_y -= srcimage_y;
      srcimage_y = std::min(srcimage_y, srcimage.rows - 3);
      srcimage_y = std::max(1, srcimage_y);

      const float A = -0.75f;

      float coeffsrcimage_y[4];
      coeffsrcimage_y[0] = ((A*(unknownfloat_y + 1) - 5*A)*(unknownfloat_y + 1) + 8*A)*(unknownfloat_y + 1) - 4*A;
      coeffsrcimage_y[1] = ((A + 2)*unknownfloat_y - (A + 3))*unknownfloat_y*unknownfloat_y + 1;
      coeffsrcimage_y[2] = ((A + 2)*(1 - unknownfloat_y) - (A + 3))*(1 - unknownfloat_y)*(1 - unknownfloat_y) + 1;
      coeffsrcimage_y[3] = 1.f - coeffsrcimage_y[0] - coeffsrcimage_y[1] - coeffsrcimage_y[2];

      short cbuunknownfloat_y[4];
      cbuunknownfloat_y[0] = cv::saturate_cast<short>(coeffsrcimage_y[0] * 2048);
      cbuunknownfloat_y[1] = cv::saturate_cast<short>(coeffsrcimage_y[1] * 2048);
      cbuunknownfloat_y[2] = cv::saturate_cast<short>(coeffsrcimage_y[2] * 2048);
      cbuunknownfloat_y[3] = cv::saturate_cast<short>(coeffsrcimage_y[3] * 2048);

      for (int i = 0; i < matDst1.cols; ++i)
      {
          float fx = (float)((i + 0.5) * scale_x - 0.5);
          int sx = cvFloor(fx);
          fx -= sx;

          if (sx < 1) {
              fx = 0, sx = 1;
          }
          if (sx >= srcimage.cols - 3) {
              fx = 0, sx = srcimage.cols - 3;
          }

          float coeffsX[4];
          coeffsX[0] = ((A*(fx + 1) - 5*A)*(fx + 1) + 8*A)*(fx + 1) - 4*A;
          coeffsX[1] = ((A + 2)*fx - (A + 3))*fx*fx + 1;
          coeffsX[2] = ((A + 2)*(1 - fx) - (A + 3))*(1 - fx)*(1 - fx) + 1;
          coeffsX[3] = 1.f - coeffsX[0] - coeffsX[1] - coeffsX[2];

          short cbufX[4];
          cbufX[0] = cv::saturate_cast<short>(coeffsX[0] * 2048);
          cbufX[1] = cv::saturate_cast<short>(coeffsX[1] * 2048);
          cbufX[2] = cv::saturate_cast<short>(coeffsX[2] * 2048);
          cbufX[3] = cv::saturate_cast<short>(coeffsX[3] * 2048);

          for (int k = 0; k < srcimage.channels(); ++k)  int testreturn  =  Binear_interprolation(image,scale);
          {
              matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, i)[k] = abs((srcimage.at<cv::Vec3b>(srcimage_y-1, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[1] +
                  srcimage.at<cv::Vec3b>(srcimage_y+1, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx-1)[k] * cbufX[0] * cbuunknownfloat_y[3] +
                  srcimage.at<cv::Vec3b>(srcimage_y-1, sx)[k] * cbufX[1] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx)[k] * cbufX[1] * cbuunknownfloat_y[1] +
                  srcimage.at<cv::Vec3b>(srcimage_y+1, sx)[k] * cbufX[1] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx)[k] * cbufX[1] * cbuunknownfloat_y[3] +
                  srcimage.at<cv::Vec3b>(srcimage_y-1, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[1] +
                  srcimage.at<cv::Vec3b>(srcimage_y+1, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+1)[k] * cbufX[2] * cbuunknownfloat_y[3] +
                  srcimage.at<cv::Vec3b>(srcimage_y-1, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[1] +
                  srcimage.at<cv::Vec3b>(srcimage_y+1, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+2)[k] * cbufX[3] * cbuunknownfloat_y[3] ) >> 22);
          }
      }
  }
  cv::imwrite("cubic_1.each_Rowindex_in_matDst1pg", matDst1);

  cv::resize(srcimage, matDst2, matDst1.size(), 0, 0, 2);
  cv::imwrite("cubic_2.each_Rowindex_in_matDst1pg", matDst2);

  */
  return 1;
}

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: DisplayImage.out <Image_Path> <ScaleSize>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], -1);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }


    float scale;
    sscanf(argv[2],"%f",&scale);
    int testreturn  =  Binear_interprolation(image,scale);
      int testreturn2  =  Bicubic_interprolation(image,scale);
    printf("heigh %d\n", image.cols );
    printf("width %d\n", image.rows );

    namedWindow("Display Image", WINDOW_AUTOSIZE );

    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

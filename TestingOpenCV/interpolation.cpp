#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;


int bilinear_interprolation(Mat srcimage,float scale){
  printf("in old bilinear\n");
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
  for (int each_Rowindex_in_matDst1 = 0; each_Rowindex_in_matDst1 < matDst1.rows; ++each_Rowindex_in_matDst1)
    {
      float dstY_mapping_to_srcY = (float)(each_Rowindex_in_matDst1*scale_y);
      int srcimage_y = cvFloor(dstY_mapping_to_srcY);

      dstY_mapping_to_srcY -= srcimage_y;
      //Select the min value
      srcimage_y = std::min(srcimage_y, iHiehgtSrc - 2);
      //check srcimage_y is a >0 num
      srcimage_y = std::max(0, srcimage_y);

      double knownfloat_y[2];
      //(1-V) //short and double will cause diffirent effect
      knownfloat_y[0] = cv::saturate_cast<short>(1.f - dstY_mapping_to_srcY);
      //V
      knownfloat_y[1] = 1 - knownfloat_y[0];


      for (int  each_Colindex_in_matDst1= 0; each_Colindex_in_matDst1 < matDst1.cols; ++each_Colindex_in_matDst1)
      {
        float dstX_mapping_to_srcX = (float)(each_Colindex_in_matDst1*scale_x);
        int srcimage_x = cvFloor(dstX_mapping_to_srcX);

        dstX_mapping_to_srcX -= srcimage_x;

        if (srcimage_x < 0) {
            dstX_mapping_to_srcX = 0, srcimage_x = 0;
        }
        if (srcimage_x >= iWidthSrc - 1) {
            dstX_mapping_to_srcX = 0, srcimage_x = iWidthSrc - 2;
        }

        double knownfloat_x[2];
        //(1-U)   //short and double will cause diffirent effect
        knownfloat_x[0] = cv::saturate_cast<short>((1.f - dstX_mapping_to_srcX));
        //U
        knownfloat_x[1] = 1 - knownfloat_x[0];
        //f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)
        for (int k = 0; k < srcimage.channels(); ++k)
        {
            *(dataDst+ each_Rowindex_in_matDst1*stepDst + 3*each_Colindex_in_matDst1 + k) = (*(dataSrc + srcimage_y*stepSrc + 3*srcimage_x + k) * knownfloat_y[0] * knownfloat_x[0] +
                *(dataSrc + (srcimage_y+1)*stepSrc + 3*srcimage_x + k) * knownfloat_x[0] * knownfloat_y[1] +
                *(dataSrc + srcimage_y*stepSrc + 3*(srcimage_x+1) + k) *knownfloat_x[1] *   knownfloat_y[0] +
                *(dataSrc + (srcimage_y+1)*stepSrc + 3*(srcimage_x+1) + k) * knownfloat_y[1] * knownfloat_x[1]);
        }

      }

    }
    namedWindow("Display old bilinear Image", WINDOW_AUTOSIZE );
    imshow("Display old bilinear Image", matDst1);
    cv::imwrite("old_linear_1.jpg", matDst1);
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
        if (sx >= srcimage.cols - 2) {
            fx = 0, sx = srcimage.cols - 2;
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

    namedWindow("Display Bicubic Image", WINDOW_AUTOSIZE );
    imshow("Display Bicubic Image", matDst1);
    cv::imwrite("cubic_1.jpg", matDst1);

    cv::resize(srcimage, matDst2, matDst1.size(), 0, 0, 2);
    cv::imwrite("cubic_2.jpg", matDst2);


  return 1;
}
long double cubicInterpolate (long double p[4], long double x) {
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

long double bicubicInterpolate (long double p[4][4], long double x, long double y) {
	long double arr[4];
	arr[0] = cubicInterpolate(p[0], y);
	arr[1] = cubicInterpolate(p[1], y);
	arr[2] = cubicInterpolate(p[2], y);
	arr[3] = cubicInterpolate(p[3], y);
	return cubicInterpolate(arr, x);

}

int old_bicubic_interprolation(Mat srcimage,float scale){
  printf("in bicubic \n");
  //Src image properties
  uchar* dataSrc = srcimage.data;
  int stepSrc = srcimage.step;
  int iWidthSrc = srcimage.cols;
  int iHiehgtSrc = srcimage.rows;
  //Create Dst image
  Mat matDst1,matDst2;

  matDst1 = cv::Mat(cv::Size(iWidthSrc*scale, iHiehgtSrc*scale), srcimage.type(), cv::Scalar::all(0));
  int dstHeight = matDst1.rows;
  int dstWidth = matDst1.cols;
  double scale_x = ( double)iWidthSrc / dstWidth;
  double scale_y = ( double)iHiehgtSrc / dstHeight;
  for (int each_Rowindex_in_matDst1 = 0; each_Rowindex_in_matDst1 < dstHeight; ++each_Rowindex_in_matDst1)
  {
      double unknownfloat_y = (double)((each_Rowindex_in_matDst1 + 0.5) * scale_y - 0.5);
      int srcimage_y = cvFloor(unknownfloat_y+0.5f);
      unknownfloat_y -= srcimage_y;
      //Select the min value
      srcimage_y = std::min(srcimage_y, iHiehgtSrc - 2);
      //check srcimage_y is a >0 num
      srcimage_y = std::max(0, srcimage_y);

      for (int each_Colindex_in_matDst1 = 0; each_Colindex_in_matDst1 < dstWidth; ++each_Colindex_in_matDst1)
      {
          double fx = (double)((each_Colindex_in_matDst1+0.5) * scale_x-0.5);
          int sx = cvFloor(fx+0.5f);
          fx -= sx;

          if (sx < 1) {
              fx = 0, sx = 1;
          }
          if (sx > srcimage.cols - 2) {
              fx = 0, sx = srcimage.cols - 2;
          }
          for (int k = 0; k < srcimage.channels(); ++k)
           {

                long double  nearpoint[4][4];
				            for(int l=0; l<4; l++){
                      float z = srcimage_y -1 +l;

                     for(int col=0; col<4; col++){
                       nearpoint[l][col] = srcimage.at<cv::Vec3b>((srcimage_y)+(l-1), (sx)+(col-1))[k];//img(i/f +l, j/f +k, 0, c);

                     }
                    }

              const long double x = 1;
              const long double y = 1;
                 matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, each_Colindex_in_matDst1)[k] = bicubicInterpolate(nearpoint,x,y);

          }
        }


  }
  namedWindow("Display Old Bicubic Image", WINDOW_AUTOSIZE );
  imshow("Display Old Bicubic Image", matDst1);
  cv::imwrite("old_Bicubic_1.jpg", matDst1);
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
    image = imread( argv[1], 1);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }


    float scale;
    sscanf(argv[2],"%f",&scale);

    int testreturn2  =  Bicubic_interprolation(image,scale);
    int testreturn3 = bilinear_interprolation(image,scale);
    int testreturn4 = old_bicubic_interprolation(image,scale);
    printf("heigh %d\n", image.cols );
    printf("width %d\n", image.rows );

    namedWindow("Display Image", WINDOW_AUTOSIZE );

    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

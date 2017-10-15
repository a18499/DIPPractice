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
  //scale_y = v
  //scale_x = u
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
          //
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
int old_bilinear_interprolation(Mat srcimage,float scale){
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
      //(1-V)
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
        //(1-U)
        knownfloat_x[0] = cv::saturate_cast<short>((1.f - dstX_mapping_to_srcX));
        //U
        knownfloat_x[1] = 1 - knownfloat_x[0];
        //f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)
        for (int k = 0; k < srcimage.channels(); ++k)
        {

          /*
          *(dataDst+ each_Rowindex_in_matDst1*stepDst + 3*i + k) = (*(dataSrc + srcimage_y*stepSrc + 3*srcimage_x + k) * cbufx[0] * cbuunknownfloat_y[0] +
              *(dataSrc + (srcimage_y+1)*stepSrc + 3*srcimage_x + k) * cbufx[0] * cbuunknownfloat_y[1] +
              *(dataSrc + srcimage_y*stepSrc + 3*(srcimage_x+1) + k) * cbufx[1] * cbuunknownfloat_y[0] +
              *(dataSrc + (srcimage_y+1)*stepSrc + 3*(srcimage_x+1) + k) * cbufx[1] * cbuunknownfloat_y[1]) >> 22;
          */

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
  /*arr[0] = CubicHermite(p[0][0],p[0][1],p[0][2],p[0][3], y);
	arr[1] = CubicHermite(p[1][0],p[1][1],p[1][2],p[1][3], y);
	arr[2] = CubicHermite(p[2][0],p[2][1],p[2][2],p[2][3], y);
	arr[3] = CubicHermite(p[3][0],p[3][1],p[3][2],p[3][3], y);
	return CubicHermite(arr[0],arr[1],arr[2],arr[3], x);*/
}
unsigned char cubicInterpolate1 (unsigned char p[4], unsigned char x) {
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

unsigned char bicubicInterpolate1 (unsigned char p[4][4], unsigned char x, unsigned char y) {
	unsigned char arr[4];
	arr[0] = cubicInterpolate1(p[0], y);
	arr[1] = cubicInterpolate1(p[1], y);
	arr[2] = cubicInterpolate1(p[2], y);
	arr[3] = cubicInterpolate1(p[3], y);
	return cubicInterpolate1(arr, x);
}
/*
void* calculatePixels(void* ptr) {
	thread_data *data = (thread_data*) ptr;
	int channels = 3;
	int row_stride = data->dest_width * channels;
    const float tx = float(data->src_width) / data->dest_width;
    const float ty = float(data->src_height) / data->dest_height;
	unsigned char P[4][4] = { 0 };
    const int x = int(tx * data->j);
    const int y = int(ty * data->i);
    for (int jj = 0; jj < 4; ++jj)
      {
		 const int z = y - 1 + jj;

         P[jj][0] = getpixel(in_buffer, data->src_width, data->src_height, z, x, data->channel);
         P[jj][1] = getpixel(in_buffer, data->src_width, data->src_height, z, x - 1, data->channel);
         P[jj][2] = getpixel(in_buffer, data->src_width, data->src_height, z, x + 1, data->channel);
         P[jj][3] = getpixel(in_buffer, data->src_width, data->src_height, z, x + 2, data->channel);
      }
	unsigned char val = bicubicInterpolate1(P,1,1);

    out_buffer[data->i * row_stride + data->j * channels + data->channel] = val;
	return NULL;
	//pthread_exit(0);
}
*/
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
          //Select the min value
        //  sx = std::min(sx, iWidthSrc - 2);
          //check srcimage_y is a >0 num
        //  sx = std::max(0, sx);
          if (sx < 1) {
              fx = 0, sx = 1;
          }
          if (sx > srcimage.cols - 2) {
              fx = 0, sx = srcimage.cols - 2;
          }


          for (int k = 0; k < srcimage.channels(); ++k)
           {
              /*matDst1.at<cv::Vec3b>(each_Rowindex_in            }
_matDst1, i)[k] = abs((srcimage.at<cv::Vec3b>(srcimage_y-1, sx-1)[k] * coeffsX[0] * coeffsrcimage_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx-1)[k] * coeffsX[0] * coeffsrcimage_y[1] +
              srcimage.at<cv::Vec3b>(srcimage_y+1, sx-1)[k] * coeffsX[0] * coeffsrcimage_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx-1)[k] * coeffsX[0] * coeffsrcimage_y[3] +
              srcimage.at<cv::Vec3b>(srcimage_y-1, sx)[k] * coeffsX[1] * coeffsrcimage_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx)[k] * coeffsX[1] * coeffsrcimage_y[1] +
              srcimage.at<cv::Vec3b>(srcimage_y+1, sx)[k] * coeffsX[1] * coeffsrcimage_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx)[k] * coeffsX[1] * coeffsrcimage_y[3] +
              srcimage.at<cv::Vec3b>(srcimage_y-1, sx+1)[k] * coeffsX[2] * coeffsrcimage_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+1)[k] * coeffsX[2] * coeffsrcimage_y[1] +
              srcimage.at<cv::Vec3b>(srcimage_y+1, sx+1)[k] * coeffsX[2] * coeffsrcimage_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+1)[k] * coeffsX[2] * coeffsrcimage_y[3] +
              srcimage.at<cv::Vec3b>(srcimage_y-1, sx+2)[k] * coeffsX[3] * coeffsrcimage_y[0] + srcimage.at<cv::Vec3b>(srcimage_y, sx+2)[k] * coeffsX[3] * coeffsrcimage_y[1] +
              srcimage.at<cv::Vec3b>(srcimage_y+1, sx+2)[k] * coeffsX[3] * coeffsrcimage_y[2] + srcimage.at<cv::Vec3b>(srcimage_y+2, sx+2)[k] * coeffsX[3] * coeffsrcimage_y[3] ));
              double pixel =  matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, i)[k];*/
            /*  double P00= srcimage.at<cv::Vec3b>(srcimage_y-1, sx-1)[k]*(1+unknownfloat_y)*(1+fx);
              double row1[4] = {P00,srcimage.at<cv::Vec3b>(srcimage_y-1, sx)[k]*(1+unknownfloat_y)*(fx),srcimage.at<cv::Vec3b>(srcimage_y-1, sx+1)[k],srcimage.at<cv::Vec3b>(srcimage_y-1, sx+2)[k]};
              double row2[4] = {srcimage.at<cv::Vec3b>(srcimage_y, sx-1)[k]*(unknownfloat_y)*(1+fx),srcimage.at<cv::Vec3b>(srcimage_y, sx)[k],srcimage.at<cv::Vec3b>(srcimage_y, sx+1)[k],srcimage.at<cv::Vec3b>(srcimage_y, sx+2)[k]};
              double row3[4] = {srcimage.at<cv::Vec3b>(srcimage_y+1, sx-1)[k]*(1-unknownfloat_y)*(1+fx),srcimage.at<cv::Vec3b>(srcimage_y+1, sx)[k],srcimage.at<cv::Vec3b>(srcimage_y+1, sx+1)[k],srcimage.at<cv::Vec3b>(srcimage_y+1, sx+2)[k]};
              double row4[4] = {srcimage.at<cv::Vec3b>(srcimage_y+2, sx-1)[k]*(2-unknownfloat_y)*(1+fx),srcimage.at<cv::Vec3b>(srcimage_y+2, sx+1)[k],srcimage.at<cv::Vec3b>(srcimage_y+2, sx+1)[k],srcimage.at<cv::Vec3b>(srcimage_y+2, sx+2)[k]};

              double nearpoint[4][4] = {{row1[0],row1[1],row1[2],row1[3]},{row2[0],row2[1],row2[2],row2[3]},{row3[0],row3[1],row3[2],row3[3]},{row4[0],row4[1],row4[2],row4[3]}};
              */

                long double  nearpoint[4][4];
				            for(int l=0; l<4; l++){
                      float z = srcimage_y -1 +l;

                      //nearpoint[l][0]=srcimage.at<cv::Vec3b>(z, (sx))[k];
                      //nearpoint[l][1]=srcimage.at<cv::Vec3b>(z, (sx)-1)[k];
                      //nearpoint[l][2]=srcimage.at<cv::Vec3b>(z, (sx)+1)[k];
                      //nearpoint[l][3]=srcimage.at<cv::Vec3b>(z, (sx)+2)[k];
                     for(int col=0; col<4; col++)
                            nearpoint[l][col] = srcimage.at<cv::Vec3b>((srcimage_y)+(l-1), (sx)+(col-1))[k];//img(i/f +l, j/f +k, 0, c);
                          //  nearpoint[l][col] = srcimage.at<cv::Vec3b>((unknownfloat_y)+(l-1), (fx)+(col-1))[k];
                    }

				                          // color[c] = bicubicInterpolate(nearpoint, fx, unknownfloat_y);

              //printf("unknownfloat_y %f\n", unknownfloat_y);
              //printf("fx %f\n", fx);
              const long double x = 1;
              const long double y = 1;
                 matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, each_Colindex_in_matDst1)[k] = bicubicInterpolate(nearpoint,x,y);
                // double pixel =  matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, i)[k];
                // printf("%lf\n", pixel);
              /*if(pixel>255){
                printf("%lf\n", pixel);
                printf("Over 255");
                matDst1.at<cv::Vec3b>(each_Rowindex_in_matDst1, i)[k] = 255;
              }*/

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
    //int testreturn  =  Binear_interprolation(image,scale);
    //int testreturn2  =  Bicubic_interprolation(image,scale);
    int testreturn3 = old_bilinear_interprolation(image,scale);
    //int testreturn4 = old_bicubic_interprolation(image,scale);
    printf("heigh %d\n", image.cols );
    printf("width %d\n", image.rows );

    namedWindow("Display Image", WINDOW_AUTOSIZE );

    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

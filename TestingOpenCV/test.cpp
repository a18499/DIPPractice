#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;



int test(Mat srcimage,float scale){
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
  for (int j = 0; j < matDst1.rows; ++j)
      {
         //printf("numbers %d\n",j );
          float fy = (float)((j + 0.5) * scale_y - 0.5);
          //get integr
          int sy = cvFloor(fy);
          fy -= sy;
          //Select the min value
          sy = std::min(sy, iHiehgtSrc - 2);
          //check sy is a >0 num
          sy = std::max(0, sy);

          short cbufy[2];
          cbufy[0] = cv::saturate_cast<short>((1.f - fy) * 2048);
          cbufy[1] = 2048 - cbufy[0];

          for (int i = 0; i < matDst1.cols; ++i)
          {
              float fx = (float)((i + 0.5) * scale_x - 0.5);
              int sx = cvFloor(fx);
              fx -= sx;

              if (sx < 0) {
                  fx = 0, sx = 0;
              }
              if (sx >= iWidthSrc - 1) {
                  fx = 0, sx = iWidthSrc - 2;
              }

              short cbufx[2];
              cbufx[0] = cv::saturate_cast<short>((1.f - fx) * 2048);
              cbufx[1] = 2048 - cbufx[0];

              for (int k = 0; k < srcimage.channels(); ++k)
              {
                  *(dataDst+ j*stepDst + 3*i + k) = (*(dataSrc + sy*stepSrc + 3*sx + k) * cbufx[0] * cbufy[0] +
                      *(dataSrc + (sy+1)*stepSrc + 3*sx + k) * cbufx[0] * cbufy[1] +
                      *(dataSrc + sy*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[0] +
                      *(dataSrc + (sy+1)*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[1]) >> 22;
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
    int testreturn  =  test(image,scale);
    printf("heigh %d\n", image.cols );
    printf("width %d\n", image.rows );

    namedWindow("Display Image", WINDOW_AUTOSIZE );

    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

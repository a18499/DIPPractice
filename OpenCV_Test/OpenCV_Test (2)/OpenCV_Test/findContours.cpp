#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <highgui\highgui.hpp>  
using namespace cv;
using namespace std;
class FindContours {

public: 
	//edge dection
	int find(String imagesrc) {
		
		Mat src = imread(imagesrc, CV_LOAD_IMAGE_GRAYSCALE);
		GaussianBlur(src, src, Size(3, 3), 0, 0);
		Mat dst1, dst2;
		Canny(src, dst1, 50, 150, 3);
		threshold(dst1, dst2, 128, 255, THRESH_BINARY_INV);  
		imshow("origin", src);
		imshow("Canny_1", dst1);
		imshow("Canny_2", dst2);
		waitKey(0);

		return 0;

	}
	//edge dection
	int findwithMat(Mat src) {
		GaussianBlur(src, src, Size(3, 3), 0, 0);
		Mat dst1, dst2;
		Canny(src, dst1, 50, 150, 3);
		threshold(dst1, dst2, 128, 255, THRESH_BINARY_INV);  
		imshow("origin", src);
		imshow("Canny_1", dst1);
		imshow("Canny_2", dst2);
		
		return 0;
	}
	int myconvertToHSV(String imagesrc) {
		
		
		Mat matsrc = imread(imagesrc, CV_LOAD_IMAGE_COLOR);
		Mat result;
		cvtColor(matsrc, result, CV_BGR2HSV);
		vector<Mat> mv;
		

		split(result, mv);

		/*int widthLimit = result.channels() * result.cols;
		for (int i = 0; i < (result.rows);i++)
		{
			for (int j = 0; j < widthLimit; j++) {
				Vec3b hsv = result.at<Vec3b>(i, j);
				int H = hsv.val[0]; //hue
				int S = hsv.val[1]; //saturation
				int V = hsv.val[2]; //value
				printf("H: %d",H);
				printf("S: %d", S);
				printf("V: %d", V);
				 
			}
		}*/
		/*vector<Mat> hsv_planes;
		split(result, hsv_planes);*/
		Mat h = mv[0]; // H channel
		Mat s = mv[1]; // S channel
		Mat v = mv[2]; // V channel

		
		imshow("hue", h);
		
		imshow("saturation", s);
	
		imshow("value", v);


		Mat showHistImg3S = Mat::zeros(256, 256, CV_8UC1); //把直方圖秀在一個256*256大的影像上
		drawHistImg2(mv[1], showHistImg3S, "TestS");
		imshow("Before  S Historgram", showHistImg3S);

		equalizeHist(mv[1], mv[1]);
		Mat showHistImg3AfterS = Mat::zeros(256, 256, CV_8UC1); //把直方圖秀在一個256*256大的影像上
		drawHistImg2(mv[1], showHistImg3AfterS, "TestS");
		imshow("AFTER  S Historgram", showHistImg3AfterS);

		//equalizeHist(mv[2], mv[2]);
		
		/*cv::Mat srcHistImageS = cv::Mat::zeros(256, 256, CV_8UC1);
		Mat showHistImg3S = Mat::zeros(256, 256, CV_8UC1);  //把直方圖秀在一個256*256大的影像上
		drawHistImg2(s, showHistImg3S, "TestS");
		imshow("Before s Historgram", showHistImg3S);
		equalizeHist(mv[1], mv[1]);

		Mat showHistImg3AfterS = Mat::zeros(256, 256, CV_8UC1); //把直方圖秀在一個256*256大的影像上
		drawHistImg2(mv[1], showHistImg3AfterS, "TestS");
		imshow("AFTER  S Historgram", showHistImg3AfterS);

		//cvCreateMat(src_Img->height, src_Img->width);
		//Mat showHistImg3 = cvCreateMat(mv[2]->height, mv[2]->width);
		cv::Mat srcHistImage = cv::Mat::zeros(256, 256, CV_8UC1);
		Mat showHistImg3 = Mat::zeros(256, 256, CV_8UC1);  //把直方圖秀在一個256*256大的影像上
		drawHistImg2(mv[2], showHistImg3,"Test");
		imshow("Before Historgram", showHistImg3);
		//equalizeHist(mv[2], mv[2]);
		imshow("After value", mv[2]);
		Mat showHistImg3After = Mat::zeros(256, 256, CV_8UC1); //把直方圖秀在一個256*256大的影像上
		drawHistImg2(mv[2], showHistImg3After, "Test");
		imshow("AFTER Historgram", showHistImg3After);
		*/
		Mat eqResult;
		merge( mv, eqResult);
		Mat afterEaq;
		cvtColor(eqResult, afterEaq, CV_HSV2BGR);
		imshow("origin", matsrc);
		imshow("HSV", result);
		imshow("After", eqResult);
		imshow("AfterRGB", afterEaq);

		imwrite("AfterRGB.jpg", afterEaq);
		
		waitKey();
		return 0;
	}
	
	

	//equalizeIntensity in ycrcb color space
	Mat equalizeIntensity(const Mat& inputImage)
	{
		if (inputImage.channels() >= 3)
		{
			Mat ycrcb;
			cvtColor(inputImage, ycrcb, CV_BGR2YCrCb);

			vector<Mat> channels;
			split(ycrcb, channels);

			equalizeHist(channels[0], channels[0]);
			equalizeHist(channels[1], channels[1]);
			equalizeHist(channels[2], channels[2]);

			Mat result;
			merge(channels, ycrcb);
			cvtColor(ycrcb, result, CV_YCrCb2BGR);

			return result;
		}

		return Mat();
	}

	int colorTransfer(String imagesrc, String imageRefer) {
		/*
		Mat matgray = imread(imagesrc, CV_LOAD_IMAGE_GRAYSCALE);
		Mat matsrc = imread(imagesrc, CV_LOAD_IMAGE_COLOR);
		imshow("origin", matsrc);
		IplImage temp = IplImage(matsrc);
		IplImage* src = &temp;
		*/
		Mat matimagesrc = imread(imagesrc, CV_LOAD_IMAGE_COLOR);	
		IplImage temp = IplImage(matimagesrc);	
		IplImage *srcimg1 = &temp;
		IplImage *srcimg2 = cvCreateImage(cvGetSize(srcimg1), srcimg1->depth, srcimg1->nChannels);
		cvCvtColor(srcimg1, srcimg2, CV_BGR2Lab);
		CvMat *out = cvCreateMat(srcimg2->height, srcimg2->width, CV_8UC3);

		Mat matimageRefer = imread(imageRefer, CV_LOAD_IMAGE_COLOR);
		IplImage tempRefer = IplImage(matimageRefer);
		IplImage *dstimg1 = &tempRefer;
		IplImage *dstimg2 = cvCreateImage(cvGetSize(dstimg1), dstimg1->depth, dstimg1->nChannels);
		cvCvtColor(dstimg1, dstimg2, CV_BGR2Lab);

		int i, j;
		double mean1[3], mean2[3];
		double var1[3], var2[3];

		for (i = 0; i<3; i++)
		{
			mean1[i] = 0;
			mean2[i] = 0;
			var1[i] = 0;
			var2[i] = 0;
		}

		CvScalar s;

		for (i = 0; i<srcimg2->height; i++)
		{
			for (j = 0; j<srcimg2->width; j++)
			{
				s = cvGet2D(srcimg2, i, j);
				mean1[0] = mean1[0] + s.val[0];
				mean1[1] = mean1[1] + s.val[1];
				mean1[2] = mean1[2] + s.val[2];
			}
		}

		for (i = 0; i<3; i++)
		{
			mean1[i] = mean1[i] / ((srcimg2->height)*(srcimg2->width));
		}

		for (i = 0; i<dstimg2->height; i++)
		{
			for (j = 0; j<dstimg2->width; j++)
			{
				s = cvGet2D(dstimg2, i, j);
				mean2[0] = mean2[0] + s.val[0];
				mean2[1] = mean2[1] + s.val[1];
				mean2[2] = mean2[2] + s.val[2];
			}
		}

		for (i = 0; i<3; i++)
		{
			mean2[i] = mean2[i] / ((dstimg2->height)*(dstimg2->width));
		}

		for (i = 0; i<srcimg2->height; i++)
		{
			for (j = 0; j<srcimg2->width; j++)
			{
				s = cvGet2D(srcimg2, i, j);
				var1[0] = var1[0] + (s.val[0] - mean1[0])*(s.val[0] - mean1[0]);
				var1[1] = var1[1] + (s.val[1] - mean1[1])*(s.val[1] - mean1[1]);
				var1[2] = var1[2] + (s.val[2] - mean1[2])*(s.val[2] - mean1[2]);
			}
		}

		for (i = 0; i<3; i++)
		{
			var1[i] = sqrt(var1[i] / ((srcimg2->width)*(srcimg2->height)));
			//cout<<var1[i]<<endl;  
		}

		for (i = 0; i<dstimg2->height; i++)
		{
			for (j = 0; j<dstimg2->width; j++)
			{
				s = cvGet2D(dstimg2, i, j);
				var2[0] = var2[0] + (s.val[0] - mean2[0])*(s.val[0] - mean2[0]);
				var2[1] = var2[1] + (s.val[1] - mean2[1])*(s.val[1] - mean2[1]);
				var2[2] = var2[2] + (s.val[2] - mean2[2])*(s.val[2] - mean2[2]);
			}
		}

		for (i = 0; i<3; i++)
		{
			var2[i] = sqrt(var2[i] / ((dstimg2->width)*(dstimg2->height)));
			//cout<<var2[i]<<endl;  
		}

		for (i = 0; i<srcimg2->height; i++)
		{
			for (j = 0; j<srcimg2->width; j++)
			{
				s = cvGet2D(srcimg2, i, j);
				s.val[0] = (s.val[0] - mean1[0])*(var2[0] / var1[0]) + mean2[0];
				s.val[1] = (s.val[1] - mean1[1])*(var2[1] / var1[1]) + mean2[1];
				s.val[2] = (s.val[2] - mean1[2])*(var2[2] / var1[2]) + mean2[2];
				cvSet2D(srcimg2, i, j, s);
			}
		}

		IplImage *srcimg3 = cvCreateImage(cvGetSize(srcimg1), srcimg1->depth, srcimg1->nChannels);
		cvCvtColor(srcimg2, srcimg3, CV_Lab2BGR);
		cvNamedWindow("result", 1);
		cvNamedWindow("input", 1);
		cvNamedWindow("refer", 1);
		cvShowImage("input", srcimg1);
		cvShowImage("refer", dstimg1);
		cvShowImage("result", srcimg3);
		Mat result = cvarrToMat(srcimg3);
		imwrite("result2.jpg ", result);
		cvWaitKey(0);
		cvReleaseImage(&srcimg1);
		cvReleaseImage(&srcimg3);
		cvReleaseImage(&dstimg1);
		cvDestroyWindow("input");
		cvDestroyWindow("refer");
		cvDestroyWindow("result");

		return 0;
	}

	void drawHistImg(const Mat &src, Mat &dst) {
		int histSize = 256;
		float histMaxValue = 0;
		for (int i = 0; i<histSize; i++) {
			float tempValue = src.at<float>(i);
			if (histMaxValue < tempValue) {
				histMaxValue = tempValue;
			}
		}

		float scale = (0.9 * 256) / histMaxValue;
		for (int i = 0; i<histSize; i++) {
			int intensity = static_cast<int>(src.at<float>(i)*scale);
			line(dst, Point(i, 255), Point(i, 255 - intensity), Scalar(0));
		}
	}

	//drow historgram
	void drawHistImg2(cv::Mat &src, cv::Mat &histImage, std::string name)
	{
		const int bins = 256;
		int hist_size[] = { bins };
		float range[] = { 0, 256 };
		const float* ranges[] = { range };
		cv::MatND hist;
		int channels[] = { 0 };

		cv::calcHist(&src, 1, channels, cv::Mat(), hist, 1, hist_size, ranges, true, false);

		double maxValue;
		cv::minMaxLoc(hist, 0, &maxValue, 0, 0);
		int scale = 1;
		int histHeight = 256;

		for (int i = 0; i < bins; i++)
		{
			float binValue = hist.at<float>(i);
			int height = cvRound(binValue*histHeight / maxValue);
			cv::rectangle(histImage, cv::Point(i*scale, histHeight), cv::Point((i + 1)*scale, histHeight - height), cv::Scalar(255));

			cv::imshow(name, histImage);
		}

		waitKey();

	}

	int custumizrMask(String imagesrc) {
	

			int hmax = 0, hmin = 0, vmin = 0, vmax = 0, smin = 0, smax = 0;
			printf("此程序将输入的图片由一般的RGB色系转化为HSV色系，然后对图片取掩膜板, 可以对掩膜板的H、S、V三个元素取不同的范围进行测试并查看， 效果，拖动滑动条即可选择范围！按 q 键退出测试！\n\n");

			Mat matimagesrc = imread(imagesrc, CV_LOAD_IMAGE_COLOR);
			IplImage temp = IplImage(matimagesrc);
			IplImage *image, *hsv, *mask;
			//创建窗口  
			cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("hsv", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("Track", CV_WINDOW_AUTOSIZE);

			cvCreateTrackbar("Hmin", "Track", &hmin, 256, 0);
			cvCreateTrackbar("Hmax", "Track", &hmax, 256, 0);
			cvCreateTrackbar("Smin", "Track", &smin, 256, 0);
			cvCreateTrackbar("Smax", "Track", &smax, 256, 0);
			cvCreateTrackbar("Vmin", "Track", &vmin, 256, 0);
			cvCreateTrackbar("Vmax", "Track", &vmax, 256, 0);


			//分配图像空间  
			image = &temp;
			hsv = cvCreateImage(cvGetSize(image), 8, 3);
			mask = cvCreateImage(cvGetSize(image), 8, 1);
			//将RGB转化为HSV色系  
			cvCvtColor(image, hsv, CV_RGB2BGR);
			cvShowImage("image", image);
			cvShowImage("hsv", hsv);
			int _hmax = 0, _hmin = 0, _vmin = 0, _vmax = 0, _smin = 0, _smax = 0, flag = 0;
			while (flag != 'q')
			{
				_hmax = hmax, _hmin = hmin, _vmin = vmin, _vmax = vmax, _smin = smin, _smax = smax;
				//produce mask
				cvInRangeS(hsv, cvScalar(MIN(_hmax, _hmin), MIN(_smax, _smin), MIN(_vmax, _vmin), 0),
					cvScalar(MAX(_hmax, _hmin), MAX(_smax, _smin), MAX(_vmax, _vmin), 0), mask);

				//show image  
				cvShowImage("mask", mask);
				flag = cvWaitKey(40);
			}

			Mat result = cvarrToMat(mask);
			imwrite("mask.jpg", result);
			cvDestroyAllWindows();
			//cvReleaseImage(&image);
			//cvReleaseImage(&hsv);
			//cvReleaseImage(&mask);
		
		
			system("pause");
			return 0;
		}
	
	int inpaintingwithmask(String srcimage,String mask) {

		Mat imageSource = imread(srcimage);
		Mat imageMask = imread(mask);
		if (!imageSource.data)
		{
			return -1;
		}
		imshow("SrcImage", imageSource);
		Mat imageGray;
		Mat maskGray;
		//Turn into 
		cvtColor(imageSource, imageGray, CV_RGB2GRAY, 0);
		cvtColor(imageMask, maskGray, CV_RGB2GRAY, 0);
		//Mat imageMask = Mat(imageSource.size(), CV_8UC1, Scalar::all(0));

		//Produce mask through threshold  
		//threshold(imageGray, imageMask, 10, 255, CV_THRESH_BINARY);
		//adaptiveThreshold(imageGray, imageMask, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 67, 0);

		Mat Kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
		//do dilation
		dilate(maskGray, maskGray, Kernel);
		dilate(maskGray, maskGray, Kernel);

		//inpainting
		inpaint(imageSource, maskGray, imageSource, 1, INPAINT_TELEA);
		imshow("Mask", maskGray);
		imshow("After Repair", imageSource);
		//write image
		imwrite("AfterRepiar3.jpg", imageSource);
		waitKey();
		return 0;
	}
	
};
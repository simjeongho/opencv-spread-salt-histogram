#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
void SpreadSalts_blue(Mat img, int num);
void SpreadSalts_green(Mat img, int num);
void SpreadSalts_red(Mat img, int num);
void Go_bright(Mat img);
void Go_dark(Mat img);
Mat GetHistogram(Mat& src);
void Countsalt(Mat src);
//void SpreadSalt(Mat img, int num);
int main()
{
	Mat src_img = imread("img1.jpg" , 1);
	Mat src_img2 = imread("img2.jpg", 0);
	Mat src_img3 = imread("img2.jpg", 0);
	Mat src_img4 = imread("img3.jpg", 1);//화성
	Mat src_img5 = imread("img4.jpg", 1);//비네팅
	resize(src_img5, src_img5, Size(src_img4.cols, src_img4.rows));//이미지 사이즈 재설정
	Mat src_img6 = imread("img5.jpg", 1);//로고
	resize(src_img6, src_img6, Size(600, 200));
	//int flags = IMREAD_COLOR 또는 int flags =1 ->컬러 영상으로 읽음
	//int flags = IMREAD_GRAYSCALE또는 int flags = 0-> 흑백 영상으로 읽음
	//int flags = IMREAD_UNCHANGED 또는 int flags = -1 ->원본 영상의 형식대로 읽음
	
	SpreadSalts_blue(src_img, 50);
	SpreadSalts_green(src_img, 50);
	SpreadSalts_red(src_img,50);
	
	Go_bright(src_img2);
	Go_dark(src_img3);
	//Go_dark(src_img2);
	
	Mat histo_bright = GetHistogram(src_img2);
	Mat histo_dark = GetHistogram(src_img3);
	
	imshow("Test window1", src_img); // 이미지 출력
	imshow("Test window2", src_img2);//두 번째 이미지
	imshow("Test window3", src_img3);
	Countsalt(src_img);
	//
	imshow("histogram", histo_bright);
	imshow("histogram2", histo_dark);
	Mat dst1;
	
	Mat graymask = imread("img5.jpg", IMREAD_GRAYSCALE);
	resize(graymask, graymask, Size(600, 200));
	Mat blackmask; // 글자색이 검정인 마스크
	Mat whitemask; // 글자색이 하얀색인 마스크
	Mat blackmaskc3; // flag를 color로 바꾼 blackmask
	Mat whitemaskc3; // flag를 color로 바꾼 whitemask
	

	threshold(graymask, blackmask, 0, 255, THRESH_OTSU); // 그레이 스케일 이미지 이진화
	//imshow("graymask", graymask);
	//imshow("blackmask", blackmask);
	bitwise_not(blackmask, whitemask); // black마스크를 반전시키면 whitemask
	//imshow("whitemask", whitemask);
	cvtColor(blackmask, blackmaskc3, COLOR_GRAY2BGR); // 컬러 스케일로 전환
	cvtColor(whitemask, whitemaskc3, COLOR_GRAY2BGR); // 컬러 스케일로 전환
	//circle(mask, Point(500, 250), 200, Scalar(255), -1, -1);
	
	Mat roiblack; // 영역과 black마스크를 합침
	Mat roiwhite; // roiblack에다 whitemaskc3을 합침

	Mat roi; // 관심영역
	roi = src_img4(Rect(Point(400, 400), Point(1000 ,600))); // roi를 설정할 때는 이미지의 크기를 고려해야한다.
	//resize(roiblack, roiblack, Size(whitemask.rows, whitemask.cols));
	cout << roi.size << " " << blackmask.size << endl;
	roi.copyTo(roiblack, blackmask);//roi와 blackmask는 크기가 같음
	roiwhite = whitemaskc3 + roiblack; // 0+하얀색은 하얀색임을 이용
	//imshow("roiwhite", roiwhite);
	//imshow("roiblack", roiblack);
	Mat roiclone; // roi영역에 로고를 넣은 모습
	roiclone = roiwhite & src_img6; // 하얀색 & 어떤색 = 어떤색
	imshow("roiclone", roiclone);
	roiclone.copyTo(roi);
	imshow("completelogo", src_img4);
	subtract(src_img4, src_img5, dst1);
	imshow("subtract", dst1);

	cout << src_img5.rows << " " << src_img4.cols;//675 //1200

	waitKey(0); // 키 입력 대기(0: 키가 입력될 때까지 프로그램 멈춤)


	//
	//
	destroyWindow("Test window"); // 이미지 출력창 종료
	return 0;
	
}


void Go_bright(Mat img)
{
	int x = 0;
	int y = 0; 
	int num = 0;
	cout << img.rows << " " << img.cols << endl;
	for (int i = 0; i < img.rows; i++)
	{
		for (int n = 0; n < img.cols; n++)
		{
			if (img.channels() == 1)
			{
				if (img.at<uchar>(i, n) - num > 0) {
					img.at<uchar>(i, n) -= num;
				}
				else
					img.at<uchar>(i, n) = 0;
			}
		}
		if (num < 200)
		{
			num++;
		}
	}
}
void Go_dark(Mat img)
{
	int x = 0;
	int y = 0;
	int num = 0;
	for (int i = img.rows-1; i > 0; i--)
	{
		for (int n = img.cols-1; n > 0; n--)
		{
			if (img.channels() == 1)
			{
				if (img.at<uchar>(i, n) - num >0) {
					img.at<uchar>(i, n) -= num;
				}
				else
					img.at<uchar>(i, n) = 0;
			}
		}
		if (num < 200)
		{
			num++;
		}
	}
}

void SpreadSalts_blue(Mat img, int num)

{
	//num: 점을 찍을 개수
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols는 이미지의 폭 정보를 저장
		int y = rand() % img.rows; // img.rows는 이미지의 높이 정보를 저장
		//나머지는 나누는 수를 넘을 수 없으므로 무작위 위치가 
		//이미지의 크기를 벗어나지 않도록 제한하는 역할을 하여줌
		
		if (img.channels() == 1)
		{
			//img.channels()는 이미지의 채널 수를 반환
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 255; // Blue 채널 접근
			img.at<Vec3b>(y, x)[1] = 0; // Green 채널 접근
			img.at<Vec3b>(y, x)[2] = 0; // Red 채널 접근
		}
	}
}
void SpreadSalts_green(Mat img, int num)

{
	//num: 점을 찍을 개수
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols는 이미지의 폭 정보를 저장
		int y = rand() % img.rows; // img.rows는 이미지의 높이 정보를 저장
		//나머지는 나누는 수를 넘을 수 없으므로 무작위 위치가 
		//이미지의 크기를 벗어나지 않도록 제한하는 역할을 하여줌

		if (img.channels() == 1)
		{
			//img.channels()는 이미지의 채널 수를 반환
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 0; // Blue 채널 접근
			img.at<Vec3b>(y, x)[1] = 255; // Green 채널 접근
			img.at<Vec3b>(y, x)[2] = 0; // Red 채널 접근
		}
	}
}
void SpreadSalts_red(Mat img, int num)

{
	//num: 점을 찍을 개수
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols는 이미지의 폭 정보를 저장
		int y = rand() % img.rows; // img.rows는 이미지의 높이 정보를 저장
		//나머지는 나누는 수를 넘을 수 없으므로 무작위 위치가 
		//이미지의 크기를 벗어나지 않도록 제한하는 역할을 하여줌

		if (img.channels() == 1)
		{
			//img.channels()는 이미지의 채널 수를 반환
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 0; // Blue 채널 접근
			img.at<Vec3b>(y, x)[1] = 0; // Green 채널 접근
			img.at<Vec3b>(y, x)[2] = 255; // Red 채널 접근
		}
	}
}

void Countsalt(Mat img)
{
	
	int count_blue = 0;
	int count_green = 0;
	int count_red = 0;
	for (int n = 0; n <img.cols; n++)
	{
		for (int u = 0; u < img.rows; u++)
		{
			if (img.at<Vec3b>(u,n)[0] == 255 && img.at<Vec3b>(u, n)[1] == 0 && img.at<Vec3b>(u, n)[2] == 0)
			{
				count_blue++;
			}
			if (img.at<Vec3b>(u,n)[0] == 0 && img.at<Vec3b>(u,n)[1] == 255 && img.at<Vec3b>(u,n)[2] == 0)
			{
				count_green++;
			}
			if (img.at<Vec3b>(u,n)[0] == 0 && img.at<Vec3b>(u,n)[1] == 0 && img.at<Vec3b>(u,n)[2] == 255)
			{
				count_red++;
			}
		}
	}
	cout << "파란점은 " << count_blue << "개"  << endl;
	cout << "초록점은 " << count_green << "개" << endl;
	cout << "붉은점은 " << count_red << "개" << endl;
}

Mat GetHistogram(Mat& src)
{
	Mat histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	//히스토그램 계산
	calcHist(&src, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	//히스토그램 plot
	int hist_w = 500;
	int hist_h = 1000;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());//정규화 

	for (int i = 1; i < number_bins; i++)
	{
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1))),
			Point(bin_w*(i-1), hist_h-cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0); // 값과 값을 잇는 선을 그리는 방식으로 plot
	}
	return histImage;
}
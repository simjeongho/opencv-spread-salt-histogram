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
	Mat src_img4 = imread("img3.jpg", 1);//ȭ��
	Mat src_img5 = imread("img4.jpg", 1);//�����
	resize(src_img5, src_img5, Size(src_img4.cols, src_img4.rows));//�̹��� ������ �缳��
	Mat src_img6 = imread("img5.jpg", 1);//�ΰ�
	resize(src_img6, src_img6, Size(600, 200));
	//int flags = IMREAD_COLOR �Ǵ� int flags =1 ->�÷� �������� ����
	//int flags = IMREAD_GRAYSCALE�Ǵ� int flags = 0-> ��� �������� ����
	//int flags = IMREAD_UNCHANGED �Ǵ� int flags = -1 ->���� ������ ���Ĵ�� ����
	
	SpreadSalts_blue(src_img, 50);
	SpreadSalts_green(src_img, 50);
	SpreadSalts_red(src_img,50);
	
	Go_bright(src_img2);
	Go_dark(src_img3);
	//Go_dark(src_img2);
	
	Mat histo_bright = GetHistogram(src_img2);
	Mat histo_dark = GetHistogram(src_img3);
	
	imshow("Test window1", src_img); // �̹��� ���
	imshow("Test window2", src_img2);//�� ��° �̹���
	imshow("Test window3", src_img3);
	Countsalt(src_img);
	//
	imshow("histogram", histo_bright);
	imshow("histogram2", histo_dark);
	Mat dst1;
	
	Mat graymask = imread("img5.jpg", IMREAD_GRAYSCALE);
	resize(graymask, graymask, Size(600, 200));
	Mat blackmask; // ���ڻ��� ������ ����ũ
	Mat whitemask; // ���ڻ��� �Ͼ���� ����ũ
	Mat blackmaskc3; // flag�� color�� �ٲ� blackmask
	Mat whitemaskc3; // flag�� color�� �ٲ� whitemask
	

	threshold(graymask, blackmask, 0, 255, THRESH_OTSU); // �׷��� ������ �̹��� ����ȭ
	//imshow("graymask", graymask);
	//imshow("blackmask", blackmask);
	bitwise_not(blackmask, whitemask); // black����ũ�� ������Ű�� whitemask
	//imshow("whitemask", whitemask);
	cvtColor(blackmask, blackmaskc3, COLOR_GRAY2BGR); // �÷� �����Ϸ� ��ȯ
	cvtColor(whitemask, whitemaskc3, COLOR_GRAY2BGR); // �÷� �����Ϸ� ��ȯ
	//circle(mask, Point(500, 250), 200, Scalar(255), -1, -1);
	
	Mat roiblack; // ������ black����ũ�� ��ħ
	Mat roiwhite; // roiblack���� whitemaskc3�� ��ħ

	Mat roi; // ���ɿ���
	roi = src_img4(Rect(Point(400, 400), Point(1000 ,600))); // roi�� ������ ���� �̹����� ũ�⸦ ����ؾ��Ѵ�.
	//resize(roiblack, roiblack, Size(whitemask.rows, whitemask.cols));
	cout << roi.size << " " << blackmask.size << endl;
	roi.copyTo(roiblack, blackmask);//roi�� blackmask�� ũ�Ⱑ ����
	roiwhite = whitemaskc3 + roiblack; // 0+�Ͼ���� �Ͼ������ �̿�
	//imshow("roiwhite", roiwhite);
	//imshow("roiblack", roiblack);
	Mat roiclone; // roi������ �ΰ� ���� ���
	roiclone = roiwhite & src_img6; // �Ͼ�� & ��� = ���
	imshow("roiclone", roiclone);
	roiclone.copyTo(roi);
	imshow("completelogo", src_img4);
	subtract(src_img4, src_img5, dst1);
	imshow("subtract", dst1);

	cout << src_img5.rows << " " << src_img4.cols;//675 //1200

	waitKey(0); // Ű �Է� ���(0: Ű�� �Էµ� ������ ���α׷� ����)


	//
	//
	destroyWindow("Test window"); // �̹��� ���â ����
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
	//num: ���� ���� ����
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols�� �̹����� �� ������ ����
		int y = rand() % img.rows; // img.rows�� �̹����� ���� ������ ����
		//�������� ������ ���� ���� �� �����Ƿ� ������ ��ġ�� 
		//�̹����� ũ�⸦ ����� �ʵ��� �����ϴ� ������ �Ͽ���
		
		if (img.channels() == 1)
		{
			//img.channels()�� �̹����� ä�� ���� ��ȯ
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 255; // Blue ä�� ����
			img.at<Vec3b>(y, x)[1] = 0; // Green ä�� ����
			img.at<Vec3b>(y, x)[2] = 0; // Red ä�� ����
		}
	}
}
void SpreadSalts_green(Mat img, int num)

{
	//num: ���� ���� ����
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols�� �̹����� �� ������ ����
		int y = rand() % img.rows; // img.rows�� �̹����� ���� ������ ����
		//�������� ������ ���� ���� �� �����Ƿ� ������ ��ġ�� 
		//�̹����� ũ�⸦ ����� �ʵ��� �����ϴ� ������ �Ͽ���

		if (img.channels() == 1)
		{
			//img.channels()�� �̹����� ä�� ���� ��ȯ
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 0; // Blue ä�� ����
			img.at<Vec3b>(y, x)[1] = 255; // Green ä�� ����
			img.at<Vec3b>(y, x)[2] = 0; // Red ä�� ����
		}
	}
}
void SpreadSalts_red(Mat img, int num)

{
	//num: ���� ���� ����
	for (int n = 0; n < num; n++)
	{
		int x = rand() % img.cols;//img.cols�� �̹����� �� ������ ����
		int y = rand() % img.rows; // img.rows�� �̹����� ���� ������ ����
		//�������� ������ ���� ���� �� �����Ƿ� ������ ��ġ�� 
		//�̹����� ũ�⸦ ����� �ʵ��� �����ϴ� ������ �Ͽ���

		if (img.channels() == 1)
		{
			//img.channels()�� �̹����� ä�� ���� ��ȯ
			img.at<uchar>(y, x) = 255;

		}
		else
		{
			img.at<Vec3b>(y, x)[0] = 0; // Blue ä�� ����
			img.at<Vec3b>(y, x)[1] = 0; // Green ä�� ����
			img.at<Vec3b>(y, x)[2] = 255; // Red ä�� ����
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
	cout << "�Ķ����� " << count_blue << "��"  << endl;
	cout << "�ʷ����� " << count_green << "��" << endl;
	cout << "�������� " << count_red << "��" << endl;
}

Mat GetHistogram(Mat& src)
{
	Mat histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	//������׷� ���
	calcHist(&src, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	//������׷� plot
	int hist_w = 500;
	int hist_h = 1000;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());//����ȭ 

	for (int i = 1; i < number_bins; i++)
	{
		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1))),
			Point(bin_w*(i-1), hist_h-cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0); // ���� ���� �մ� ���� �׸��� ������� plot
	}
	return histImage;
}
//参考论文原作者（Multi-scale C++）：github.com/joaofaro/KCFcpp
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "kcftracker.hpp"
#include "BoxExtractor.h"

using namespace std;
using namespace cv;

int main() {

	bool HOG = true; //使用hog（方向梯度直方图）特征
	bool FIXEDWINDOW = false; //使用固定窗口大小
	bool MULTISCALE = true; //使用多尺度
	bool SILENT = true;
	bool LAB = false; //不使用lab色空间特征
	// 创建KCF跟踪器
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// ROI selector
	BoxExtractor box;

	//需要跟踪的视频
	string video = "C://Users//tcsns//Desktop//空中签名项目//video//init//IMG_0080.MOV";
	VideoCapture cap(video);
	//保存跟踪视频
	string video_save_path = "C:\\Users\\tcsns\\Desktop\\空中签名项目\\video\\finish\\";
	string videoName = "IMG_0080_KCF.avi";
	string path = video_save_path + videoName;
	VideoWriter demo(path, CV_FOURCC('M', 'J', 'P', 'G'), 119, Size(1280, 720));
	//VideoWriter demo(path, CV_FOURCC('M', 'J', 'P', 'G'), 29, Size(960, 544));//手机拍摄
	vector<Point> points;
	//double width = cap.get(CAP_PROP_FRAME_WIDTH); //视频帧的宽度
	//double hight = cap.get(CAP_PROP_FRAME_HEIGHT); //视频帧的高度
	//cout << "\n视频帧的宽度为：" << width << "视频帧的高度为：" << hight;

	Mat frame;
	// 给定初始跟踪框
	cap >> frame;
	Rect2d roi = box.extract("tracker", frame, true);

	//不给框直接撤
	if (roi.width == 0 || roi.height == 0) return 0;

	tracker.init(roi, frame);
	rectangle(frame, roi, Scalar(0, 255, 255), 1, 8); //画出初始跟踪框,跟踪框颜色RGB(0,255,255)
	Rect result;


	//开始跟踪
	printf("Start the tracking process, press ESC to quit.\n");
	while (1) 
	{
		cap >> frame;
		//视频结束
		if (frame.rows == 0 || frame.cols == 0) break;
		result = tracker.update(frame);

		//画出空中签名轨迹
		Point centerPoint((result.x + result.width/2), (result.y + result.height/2));
		points.push_back(centerPoint);
		if (points.size() > 1) {
			for (int i = 0; i < points.size() - 1; i++) {
				line(frame, points[i], points[i + 1], Scalar(0, 0, 0)); //points.size()可能会超过int的范围
			}
		}
		//画出跟踪框，帧，左上角点，右下角，颜色
		rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 0, 0), 1, 8);
		
		imshow("跟踪画面", frame);
		demo << frame;
		//按esc退出
		if (waitKey(1) == 27)break;
	}

	return 0;
}

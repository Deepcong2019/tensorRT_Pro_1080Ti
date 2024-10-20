#include <iostream>
#include "YoloSDK.hpp"
#include "common/ilogger.hpp"
#include <opencv2/opencv.hpp>
#include "trt_builder.hpp"

using namespace std;
using namespace YoloSDK;
bool requires(const char* name);

int main() {
	Params params;
	DetectResult result;
	YoloObj* yolov7 = new YoloObj();
	params.mode = TRT::Mode::FP32;
	params.type = Yolo::Type::V5;
	params.deviceId = 0;
	params.test_batch_size = 16;



	yolov7->init_yolo(params);

	string video_path = "../workspace/exp/face_tracker.mp4";
	// ���� VideoCapture ����
	cv::VideoCapture cap(video_path);
	// �����Ƶ�Ƿ�ɹ���
	if (!cap.isOpened()) {
		std::cerr << "Error: Could not open video file." << std::endl;
		return -1;
	}

	// ��ȡ����ʾ��Ƶ֡
	cv::Mat frame;
	while (true) {

		cap >> frame; // ��ȡһ֡
		if (frame.empty()) {
			break; // ��Ƶ����
		}
		std::cout<<"frame size: " << frame.size() << std::endl;
		yolov7->infer_yolo(frame, result, true);

		if (cv::waitKey(1) == 27) { // ���� ESC ���˳�
			break;
		}
	}

}

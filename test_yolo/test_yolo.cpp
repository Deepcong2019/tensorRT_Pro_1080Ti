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
	// 创建 VideoCapture 对象
	cv::VideoCapture cap(video_path);
	// 检查视频是否成功打开
	if (!cap.isOpened()) {
		std::cerr << "Error: Could not open video file." << std::endl;
		return -1;
	}

	// 读取并显示视频帧
	cv::Mat frame;
	while (true) {

		cap >> frame; // 读取一帧
		if (frame.empty()) {
			break; // 视频结束
		}
		std::cout<<"frame size: " << frame.size() << std::endl;
		yolov7->infer_yolo(frame, result, true);

		if (cv::waitKey(1) == 27) { // 按下 ESC 键退出
			break;
		}
	}

}

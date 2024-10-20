#pragma once
#ifndef YOLO_SDK_HPP
#define YOLO_SDK_HPP
#include "export_global.hpp"
#include <vector>
#include <iostream>
#include "builder/trt_builder.hpp"
#include <app_yolo/yolo.hpp>
#endif
using namespace std;
namespace YoloSDK {
	struct TRT_EXPORT Params {
		TRT::Mode mode;
		Yolo::Type type;
		int deviceId;
		int test_batch_size;
		bool show;
		string onnx_file;
		string engine_file;
		float confidence_threshold;
		float nms_threshold;
		//Params() : mode(TRT::Mode::FP32), type(Yolo::Type::V5), deviceId(0) {}
		Params() {
			 mode = TRT::Mode::FP32;
			 type = Yolo::Type::V5;
			 deviceId = 0;
			 show = false;
			 test_batch_size = 1;
			 confidence_threshold = 0.25;
			 nms_threshold = 0.5;
			 onnx_file = "";
			 engine_file = "";
		}
	};


	struct TRT_EXPORT DetectResult {
		Yolo::BoxArray boxes_array;

	};

	class TRT_EXPORT YoloObj {
		private:
		 shared_ptr<Yolo::Infer> engine_yolo;

	public:
		static bool compile_yolo(TRT::Mode mode,int test_batch_size, string onnx_file, string engine_file);
		bool init_yolo(Params params);
		void infer_yolo(cv::Mat& image, DetectResult& result, bool show);
	};
}


#include "YoloSDK.hpp"
#include <ilogger.hpp>
#include "builder/trt_builder.hpp"
using namespace std;
using namespace YoloSDK;

bool requires(const char* name);

bool YoloObj::compile_yolo(TRT::Mode mode, int test_batch_size, string onnx_file, string engine_file)
{
    bool end_flag = TRT::compile(
                    mode,                       // FP32¡¢FP16¡¢INT8
                    test_batch_size,            // max batch size
                    onnx_file,                  // source 
                    engine_file                 // save to
    );
    if (end_flag) {
        cout << "Compile done!" << endl;
        return true;
    }
    return false;
}





bool YoloObj::init_yolo(Params params) {
    TRT::set_device(params.deviceId);
    string model = "yolov7";
    auto mode_name = TRT::mode_string(params.mode);
    const char* name = model.c_str();
    if (not requires(name))
        return 0;
    params.onnx_file = iLogger::format("%s.onnx", name);
    params.engine_file = iLogger::format("%s.%s.trtmodel", name, mode_name);
    if (not iLogger::exists(params.engine_file)) {
        if (!compile_yolo(params.mode, params.test_batch_size, params.onnx_file, params.engine_file))
            return 0;
    }
    
    engine_yolo = Yolo::create_infer(
            params.engine_file,             // engine file
            params.type,                    // yolo type, Yolo::Type::V5 / Yolo::Type::X
            params.deviceId,                // gpu id
            params.confidence_threshold,    // confidence threshold
            params.nms_threshold,           // nms threshold
            Yolo::NMSMethod::FastGPU,   // NMS method, fast GPU / CPU
            1024,                       // max objects
            false                       // preprocess use multi stream
            );
    if (engine_yolo == nullptr) {
        INFOE("infer create failed.");
        return 0;
    }
    return 1;
}



void YoloObj::infer_yolo(cv::Mat& image, DetectResult& result, bool show) {
    result.boxes_array = engine_yolo->commit(image).get();
    if (show) {
        for (auto& obj : result.boxes_array) {
            uint8_t b, g, r;
            tie(b, g, r) = iLogger::random_color(obj.class_label);
            cv::rectangle(image, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(b, g, r), 5);

            cv::rectangle(image, cv::Point(obj.left - 3, obj.top - 33), cv::Point(obj.left, obj.top), cv::Scalar(b, g, r), -1);

            cv::imshow("Video", image);

        }
    }
}
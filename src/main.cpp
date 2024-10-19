#include "app_yolo/yolo.hpp"
#include "common/ilogger.hpp"
#include "builder/trt_builder.hpp"
#include "infer/trt_infer.hpp"

using namespace std;
bool requires(const char* name);

static const char* cocolabels[] = {
    "person", "bicycle", "car", "motorcycle", "airplane",
    "bus", "train", "truck", "boat", "traffic light", "fire hydrant",
    "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse",
    "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
    "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis",
    "snowboard", "sports ball", "kite", "baseball bat", "baseball glove",
    "skateboard", "surfboard", "tennis racket", "bottle", "wine glass",
    "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich",
    "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
    "chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv",
    "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave",
    "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
    "scissors", "teddy bear", "hair drier", "toothbrush"
};

int main() {
    TRT::Mode mode = TRT::Mode::FP32;
    Yolo::Type type = Yolo::Type::V7;
    string model = "yolov7";
    int deviceid = 0;
    auto mode_name = TRT::mode_string(mode);
    TRT::set_device(deviceid);



    const char* name = model.c_str();
    INFO("===================== test %s %s %s ==================================", Yolo::type_name(type), mode_name, name);

    if (not requires(name))
        return 0;

    string onnx_file = iLogger::format("%s.onnx", name);
    string model_file = iLogger::format("%s.%s.trtmodel", name, mode_name);
    int test_batch_size = 16;


    // 编译模型
    if (not iLogger::exists(model_file)) {
        TRT::compile(
            mode,                       // FP32、FP16、INT8
            test_batch_size,            // max batch size
            onnx_file,                  // source 
            model_file                 // save to
         );
    }

    auto engine = Yolo::create_infer(
        model_file,                // engine file
        type,                       // yolo type, Yolo::Type::V5 / Yolo::Type::X
        deviceid,                   // gpu id
        0.25f,                      // confidence threshold
        0.45f,                      // nms threshold
        Yolo::NMSMethod::FastGPU,   // NMS method, fast GPU / CPU
        1024,                       // max objects
        false                       // preprocess use multi stream
    );
    if (engine == nullptr) {
        INFOE("Engine is nullptr");
        return 0;
    }

    string video_path = "./exp/face_tracker.mp4";
    Yolo::BoxArray boxes_array;

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
        auto begin_timer = iLogger::timestamp_now_float();
        cap >> frame; // 读取一帧
        if (frame.empty()) {
            break; // 视频结束
        }
        boxes_array = engine->commit(frame).get();
        float fps = 1000 / (iLogger::timestamp_now_float() - begin_timer);
       // 将 fps 转换为字符串（使用任一方法）
        std::string fps_str = std::to_string(fps);  // 或者使用字符串流方法

        // 在视频帧上显示 FPS
        cv::putText(frame, "FPS: " + fps_str, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

        for (auto& obj : boxes_array) {
            uint8_t b, g, r;
            tie(b, g, r) = iLogger::random_color(obj.class_label);
            cv::rectangle(frame, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(b, g, r), 5);

            auto name = cocolabels[obj.class_label];
            auto caption = iLogger::format("%s %.2f %.2f", name, obj.confidence);
            int width = cv::getTextSize(caption, 0, 1, 2, nullptr).width + 10;
            cv::rectangle(frame, cv::Point(obj.left - 3, obj.top - 33), cv::Point(obj.left + width, obj.top), cv::Scalar(b, g, r), -1);

            cv::putText(frame, caption, cv::Point(obj.left, obj.top - 5), 0, 1, cv::Scalar::all(0), 2, 16);
            cv::imshow("Video", frame);
            
        }

      
        if (cv::waitKey(1) == 27) { // 按下 ESC 键退出
            break;
        }
    }
    engine.reset();

    // 释放资源
    cap.release();
    cv::destroyAllWindows();


    return 0;
}



#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

int main() {
    // 读取输入图像并上传到 GPU
    cv::Mat src = cv::imread("input.jpg", cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cerr << "无法读取图像" << std::endl;
        return -1;
    }
    cv::cuda::GpuMat d_src, d_dst;
    d_src.upload(src);

    // 创建高斯滤波器
    int ksize = 15; // 高斯核大小
    double sigma = 1.5; // 标准差
    cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createGaussianFilter(d_src.type(), d_src.type(), cv::Size(ksize, ksize), sigma);

    // 应用高斯滤波器
    filter->apply(d_src, d_dst);

    // 将结果从 GPU 下载到 CPU
    cv::Mat dst;
    d_dst.download(dst);

    // 显示结果
    cv::imshow("Original Image", src);
    cv::imshow("Gaussian Filtered Image", dst);
    cv::waitKey(0);

    return 0;
}
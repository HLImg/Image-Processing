#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

cv::Mat channel_swap(cv::Mat image){
    size_t width = image.cols ;
    size_t height = image.rows ;

    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3) ;

    for (int y = 0 ; y < height ; y++){
        for (int x = 0 ; x < width ; x++){
            // BGR -> RGB
            out.at<cv::Vec3b>(y, x)[0] = image.at<cv::Vec2b>(y, x)[2] ;
            out.at<cv::Vec3b>(y, x)[1] = image.at<cv::Vec2b>(y, x)[1] ;
            out.at<cv::Vec3b>(y, x)[2] = image.at<cv::Vec2b>(y, x)[0] ;
        }
    }

    return out ;
}

int main(int argc, const char* argv[]){
    std::cout << "begin" << std::endl ;
    cv::Mat image_bgr = cv::imread("./kodim08.png", cv::IMREAD_COLOR);
    cv::Mat image_rgb = channel_swap(image_bgr);
    cv::imwrite("./output/answer.png", image_rgb) ;
    std::cout << "end" << std::endl ;
    return 0 ;
}
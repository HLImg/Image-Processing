#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

cv::Mat BGR2GRAY(cv::Mat image){
    int height = image.rows ;
    int width = image.cols ;

    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1) ;

    for (int y = 0 ; y < height ; y++){
        for (int x = 0 ; x < width ; x++){
            out.at<uchar>(y, x) = 0.2126 * image.at<cv::Vec3b>(y, x)[2] \ 
            + 0.7152 * image.at<cv::Vec3b>(y, x)[1] \
            + 0.0722 * image.at<cv::Vec3b>(y, x)[0] ;
        }
    }

    return out ;
}

int main(int argc, const char* argv[]){
    cv::Mat image = cv::imread("../q_1/kodim08.png", cv::IMREAD_COLOR) ;
    cv::Mat out = BGR2GRAY(image) ;
    cv::imwrite("./output/answer.png", out) ;
    return 0 ;
}
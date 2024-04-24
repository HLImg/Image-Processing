#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

cv::Mat decrease_color(cv::Mat image){
    int height = image.rows ;
    int width = image.cols ;
    int channel = image.channels() ;

    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3) ;

    for (int y = 0 ; y < height ; y++){
        for (int x = 0 ; x < width ; x++){
            for (int c = 0 ; c < channel; c++){
                out.at<cv::Vec3b>(y, x)[c] = (uchar)(floor((double)image.at<cv::Vec3b>(y, x)[c] / 64) * 64 + 32);
            }
        }
    }

    return out ;
}

int main(int argc, const char* argv[]){
    std::string file_name = "../q_1/kodim08.png" ;
    cv::Mat image = cv::imread(file_name) ;
    cv::Mat out = decrease_color(image) ;
    cv::imwrite("./output/answer.png", out) ;
    return 0 ; 
}
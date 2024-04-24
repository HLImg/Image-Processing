#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

cv::Mat avg_pooling(cv::Mat image, int win){
    int height = image.rows ;
    int width = image.cols ;
    int channel = image.channels() ;

    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3) ;

    double v ;

    for (int y = 0 ; y < height ; y+=win){
        for (int x = 0 ; x < width ; x+=win){
            for (int c = 0 ; c < channel ; c++){
                v = 0 ;
                for (int dy = 0 ; dy < win && dy < height - y ;  dy++)
                    for (int dx = 0 ; dx < win && dx < width - x ; dx++)
                        v += (double)image.at<cv::Vec3b>(y + dy, x + dx)[c] ;
                
                v /= (win * win) ;

                for (int dy = 0 ; dy < win && dy < height - y ; dy++)
                    for (int dx = 0 ; dx < win && dx < width - x ; dx++)
                        out.at<cv::Vec3b>(y + dy, x + dx)[c] = (uchar)v;

            }
        }
    }
    
    return out ;
}


int main(int argc, const char* argv[]){
    std::string file_name = "../q_1/kodim08.png" ;
    cv::Mat image = cv::imread(file_name) ;
    cv::Mat out = avg_pooling(image, 8) ;
    cv::imwrite("./output/answer.png", out) ;
    std::cout << "end" << std::endl ;
    return 0 ; 
}
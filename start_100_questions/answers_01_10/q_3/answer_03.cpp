#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <string>

class Binarize {
    private:
        std::string filename ; 
        cv::Mat image ;

        int height = 0 ;
        int width = 0 ;

    public:
        Binarize(const std::string& name) : filename(name){
            image = cv::imread(filename, cv::IMREAD_COLOR) ;
            height = image.rows ;
            width = image.cols ;
        }

        cv::Mat bgr2gray(){
            cv::Mat out = cv::Mat::zeros(this->height, this->width, CV_8UC1) ;
            for (int y = 0 ; y < this->height ; y++){
                for (int x = 0 ; x < this->width ; x++){
                    out.at<uchar>(y, x) = 0.2126 * this->image.at<cv::Vec3b>(y, x)[2] \
                                        + 0.7152 * this->image.at<cv::Vec3b>(y, x)[1] \
                                        + 0.0722 * this->image.at<cv::Vec3b>(y, x)[0] ;
                }
            }
            return out ;
        }

        cv::Mat binarize(int threshold){
            cv::Mat out = cv::Mat::zeros(this->height, this->width, CV_8UC1) ;
            cv::Mat gray = this->bgr2gray() ;

            for (int y = 0 ; y < this->height ; y++){
                for (int x = 0 ; x < this->width ; x++){
                    if (gray.at<uchar>(y, x) < threshold ){
                        out.at<uchar>(y, x) = 0 ;
                    }else{
                        out.at<uchar>(y, x) = 255 ;
                    }
                }
            }

            return out ;
        }
};


int main(int argc, const char* argv[]){
    int threshold = 128 ;
    std::string file_name = "../q_1/kodim08.png" ;
    Binarize binarize(file_name) ;
    cv::Mat out = binarize.binarize(threshold) ;
    cv::imwrite("./output/answer.png", out) ;
    return 0; 
}
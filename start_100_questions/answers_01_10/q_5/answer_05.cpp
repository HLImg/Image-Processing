#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <string>
#include <math.h>

class HSV {
    private:
        std::string filename ;
        cv::Mat image ;

        int height = 0 ;
        int width = 0 ;
    
    public:
        HSV(const std::string& name) : filename(name){
            image = cv::imread(filename, cv::IMREAD_COLOR) ;
            height = image.rows ;
            width = image.cols ;
        }

        cv::Mat bgr2hsv(){
            float r, g, b ;
            float h, s, v ;
            float _max, _min ;

            cv::Mat hsv = cv::Mat::zeros(this->height, this->width, CV_32FC3) ;

            for (int y = 0 ; y < this->height ; y++){
                for (int x = 0 ; x < this->width ; x++){
                    r = (float)image.at<cv::Vec3b>(y, x)[2] / 255 ;
                    g = (float)image.at<cv::Vec3b>(y, x)[1] / 255 ;
                    b = (float)image.at<cv::Vec3b>(y, x)[0] / 255 ;

                    _max = fmax(r, fmax(g, b)) ;
                    _min = fmin(r, fmin(g, b)) ;

                    // get hue
                    if (_max == _min){
                        h = 0 ;
                    } else if (_min == b){
                        h = 60 * (g - r) / (_max - _min) + 60 ;
                    } else if (_min == r){
                        h = 60 * (b - g) / (_max - _min) + 180 ;
                    } else if (_min == g){
                        h = 60 * (r - b) / (_max - _min) + 300 ;
                    }

                    s = _max - _min ;
                    v = _max ;

                    hsv.at<cv::Vec3f>(y, x)[0] = h ;
                    hsv.at<cv::Vec3f>(y, x)[1] = s ;
                    hsv.at<cv::Vec3f>(y, x)[2] = v ;
                }
            }

            return hsv ;

        }

        cv::Mat hsv2bgr(cv::Mat hsv){
            float h, s, v ;
            double c, _h, _x ;
            double r, g, b ;

            cv::Mat out = cv::Mat::zeros(this->height, this->width, CV_8UC3) ;

            for (int y = 0 ; y < this->height ; y++){
                for (int x = 0 ; x < this->width ; x++){
                    h = hsv.at<cv::Vec3f>(y, x)[0];
                    s = hsv.at<cv::Vec3f>(y, x)[1];
                    v = hsv.at<cv::Vec3f>(y, x)[2];

                    c = s ;
                    _h = h / 60 ;
                    _x = c * (1 - abs(fmod(_h, 2) - 1)) ;

                    r = g = b = v - c ;

                    if (_h < 1){
                        r += c ;
                        g += _x;
                    } else if (_h < 2){
                        r += _x;
                        g += c;
                    } else if (_h < 3){
                        g += c;
	                    b += _x;
                    } else if (_h < 4){
                        g += _x;
	                    b += c;
                    } else if (_h < 5){
                        r += _x;
	                    b += c;
                    } else if (_h < 5){
                        r += c;
	                    b += _x;
                    }

                    out.at<cv::Vec3b>(y, x)[0] = (uchar)(b * 255);
                    out.at<cv::Vec3b>(y, x)[1] = (uchar)(g * 255);
                    out.at<cv::Vec3b>(y, x)[2] = (uchar)(r * 255);
                }
            }
            return out ;
        }

        cv::Mat inverse_hue(cv::Mat hsv){
            for (int y = 0 ; y < this->height ; y++){
                for (int x = 0 ; x < this->width ; x++){
                    hsv.at<cv::Vec3f>(y, x)[0] = fmod(hsv.at<cv::Vec3f>(y, x)[0] + 180, 360);
                }
            }
            return hsv ;
        }
} ;

int main(int argc, const char* argv[]){
    std::string file_name = "../q_1/kodim08.png" ;
    HSV hsv(file_name) ;

    cv::Mat im_hsv = hsv.bgr2hsv() ;
    im_hsv = hsv.inverse_hue(im_hsv) ;
    cv::Mat out = hsv.hsv2bgr(im_hsv) ;
    cv::imwrite("./output/answer.png", out) ;
    return 0 ;
}
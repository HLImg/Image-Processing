#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>

class MediumFilter{
    private:
        int height = 0 ;
    public:
        MediumFilter(int h) : height(h) {}

        cv::Mat zero_pad(cv::Mat image, int ksize, int padding, int stride){
            int height = image.rows ;
            int width = image.cols ;
            int new_h = (height - ksize + 2 * padding) / stride + 1 ;
            int new_w = (width - ksize + 2 * padding) / stride + 1 ;

            std::cout << "the size of padded image is (" << new_h << ", " << new_w << ")" << std::endl ;

            cv::Mat out = cv::Mat::zeros(new_h, new_w, CV_8UC3) ;

            for (int y = padding ;  y < new_h - padding ; y++){
                for (int x = padding ; x < new_w - padding ; x++){
                    out.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(y - padding, x - padding) ;
                }
            }

            return out ;
        }

        cv::Mat filtering(cv::Mat image, int stride, int padding, int ksize){
            int height = image.rows ;
            int width = image.cols ;
            int channels = image.channels() ;

            double v = 0 ;

            int count ;
            int radius = ksize / 2 ;
            int vs[ksize * ksize];

            cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3) ;


            for (int y = padding ; y < height - padding ; y+=stride){
                for (int x = padding ; x < width - padding ; x+=stride){
                    for (int c = 0 ; c < channels ; c++){
                        v = 0 ;

                        for (int i = 0 ; i < ksize * ksize ; i++)
                            vs[i] = 999 ;

                        count = 0 ;
                        for (int dy = -radius ; dy < radius + 1 ; dy++){
                            for (int dx = -radius ; dx < radius + 1 ; dx++){
                                vs[count++] = (int)image.at<cv::Vec3b>(std::max(0, std::min(height - 1, y + dy)), 
                                                          std::max(0, std::min(width - 1, x + dx)))[c] ;
                            }
                        }

                        std::sort(vs, vs + (ksize * ksize));
                        out.at<cv::Vec3b>(y, x)[c] = (uchar)vs[int(floor(count / 2)) + 1] ;
                    }
                }
            }

            return out ;
        }
};


int main(int argc, const char* argv[]){
    int ksize = 7 ;
    int stride = 1 ;
    int padding = 1 ;

    MediumFilter filter(2) ;

    cv::Mat image = cv::imread("../q_1/kodim08.png") ;
    cv::Mat zero_padded = filter.zero_pad(image, ksize, padding, stride) ;
    cv::Mat out = filter.filtering(image, stride, padding, ksize) ;

    cv::imwrite("./output/answer.png", out) ;
    return 0 ;
}
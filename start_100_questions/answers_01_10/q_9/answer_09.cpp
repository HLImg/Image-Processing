#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>

class GaussianFilter{
    private:
        int height = 0 ;
    public:
        GaussianFilter(int h) : height(h) {}

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

        cv::Mat gen_kernel(int ksize, float sigma){
            int center = ksize / 2 ;
            cv::Mat kernel = cv::Mat::zeros(ksize, ksize, CV_32FC1) ;
            double g ;
            double sum = 0. ;

            for (int y = 0 ; y < ksize ; y++){
                for (int x = 0 ; x < ksize ; x++){
                    g = exp(
                        -1 * ((pow(x - center, 2) + pow(y - center, 2))) / (2 * sigma * sigma)
                    ) / (2 * CV_PI * sigma * sigma) ;

                    sum += g;
                    kernel.at<float>(y, x) = (float)g ;
                }
            }
            // normalization
            for (int y = 0 ; y < ksize ; y++){
                for (int x = 0 ; x < ksize ; x++){
                    kernel.at<float>(y, x) = kernel.at<float>(y, x) / sum ;
                }
            }

            return kernel ;
        }

        cv::Mat filtering(cv::Mat image, int stride, int padding, int ksize, float sigma){
            int height = image.rows ;
            int width = image.cols ;
            int channels = image.channels() ;

            double v = 0 ;

            int radius = ksize / 2 ;
            cv::Mat kernel = this->gen_kernel(ksize, sigma) ;

            cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3) ;

            for (int y = padding ; y < height - padding ; y+=stride){
                for (int x = padding ; x < width - padding ; x+=stride){
                    for (int c = 0 ; c < channels ; c++){
                        v = 0 ;
                        for (int dy = -radius ; dy < radius + 1 ; dy++){
                            for (int dx = -radius ; dx < radius + 1 ; dx++){
                                v += image.at<cv::Vec3b>(std::max(0, std::min(height - 1, y + dy)), 
                                                          std::max(0, std::min(width - 1, x + dx)))[c] * 
                                     kernel.at<float>(dy + radius, dx + radius) ;
                            }
                        }

                        out.at<cv::Vec3b>(y, x)[c] = (uchar)v ;
                    }
                }
            }

            return out ;
        }
};

int main(int argc, const char* argv[]){
    int ksize = 3 ;
    int padding = 1 ;
    int stride = 1 ;
    float sigma = 1.3 ;
    std::cout << "sigma = " << sigma << std::endl ;
    GaussianFilter filter(2) ;
    cv::Mat image = cv::imread("./input.png") ;
    cv::Mat zero_padded = filter.zero_pad(image, ksize, padding, stride) ;
    cv::Mat out = filter.filtering(zero_padded, stride, padding, ksize, sigma) ;

    cv::imwrite("./output/answer.png", out) ;
    return 0; 
}

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

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

        /**
         * @brief 普通的图像二值化
         * 
         * @param threshold ： 二值化的阈值
         * @return cv::Mat 
         */
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


        int cal_threshold_otcu(){
            int val ;
            int threshold = 0 ;
            double w_0 = 0, w_1 = 0 ;
            double m_0 = 0, m_1 = 0 ;
            double max_sb = 0, sb = 0 ;

            for (int t = 0 ; t < 255 ; t++){
                w_0 = 0 ;
                w_1 = 0 ;
                m_0 = 0 ;
                m_1 = 0 ;

                for (int y = 0 ; y < this->height ; y++){
                    for (int x = 0 ; x < this->width ; x++){
                        val = (int)(this->image.at<uchar>(y, x)) ;
                        if (val < t){
                            w_0++ ;
                            m_0 += val ;
                        }else{
                            w_1++;
                            m_1 += val ;
                        }
                    }
                }

                m_0 = m_0 / w_0 ;
                m_1 = m_1 / w_1 ;
                w_0 = w_0 / (this->height * this->width) ;
                w_1 = w_1 / (this->height * this->width) ;
                sb = w_0 * w_1 * pow((m_0 - m_1), 2) ;

                if (sb > max_sb){
                    max_sb = sb ;
                    threshold = t ;
                }
            }

            return threshold ;
        }
};


int main(int argc, const char* argv[]){
    std::string file_name = "../q_1/kodim08.png" ;
    Binarize binarize(file_name) ;
    int threshold = binarize.cal_threshold_otcu();
    std::cout << "Otus's threshold is " << threshold << std::endl ;
    cv::Mat out = binarize.binarize(threshold) ;
    cv::imwrite("./output/answer.png", out) ;
    return 0; 
}
#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat preprocess(cv::Mat eyeFrame, int threshold)
{   
    int erosionSize = 1;
    //cv::Mat kernel = cv::Mat::ones(3, 3, CV_8UC1);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, 
                                               cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), 
                                               cv::Point(erosionSize, erosionSize));
    cv::Mat result;
    cv::bilateralFilter(eyeFrame, result, 10, 15, 15);
    cv::erode(result, result, kernel, cv::Point(-1, -1), 3); 
    cv::threshold(result, result, threshold, 255, cv::THRESH_BINARY);

    return result;
}

#endif

#ifndef PUPIL_HPP
#define PUPIL_HPP

#include <vector>
#include <algorithm>
#include <exception>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "preprocessing.hpp"

class Pupil
{
public:
    int y, x;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    Pupil(int threshold) 
    {
        this->threshold = threshold;
    }

    cv::Mat preprocess(cv::Mat eyeFrame, int threshold)
    {
        cv::Mat result;
        cv::bilateralFilter(eyeFrame, result, 10, 15, 15);
        cv::erode(result, result, this->kernel, cv::Point(-1, -1), 3);
        cv::threshold(result, result, threshold, 255, cv::THRESH_BINARY);

        return result;
    }

    void findPupil(cv::Mat eyeFrame)
    {
        //cv::Mat pupilFrame = this->preprocess(eyeFrame, threshold);
        try
        {
        cv::Mat pupilFrame = preprocess(eyeFrame, threshold);

        cv::findContours(pupilFrame, this->contours, this->hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        std::cout << contours.size() << " ";
        std::sort(contours.begin(), contours.end(),
                  [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
                  { return cv::contourArea(a) < cv::contourArea(b); }); // <-- (-2)

        //try
        //{
            cv::Moments moments = cv::moments(contours[contours.size() - 2]);
            this->x = moments.m10 / moments.m00;
            this->y = moments.m01 / moments.m00;
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    bool contourAreaComp(const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
    {
        return cv::contourArea(a) < cv::contourArea(b);
    }
    
private:
    int threshold = 50;
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_8UC1);
};

#endif

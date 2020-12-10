#ifndef PUPIL_HPP
#define PUPIL_HPP

#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "preprocessing.hpp"
#include "calibration.hpp"
#include <iostream>
using std::cout;
using std::endl;

class Pupil
{
public:
    int y = -1;
    int x = -1;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    Pupil(int threshold) 
    {
        this->threshold = threshold;
    }

    bool pupilIsLocated()
    {
        return x >= 0 && y >= 0;
    }

    void findPupil(cv::Mat &eyeFrame)
    {
        cv::Mat pupilFrame = preprocess(eyeFrame, this->threshold);

        cv::findContours(pupilFrame, this->contours, this->hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        std::sort(contours.begin(), contours.end(),
                  [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
                  { return cv::contourArea(a) < cv::contourArea(b); });

        try
        {
            std::vector<cv::Point> contour = contours.at(contours.size() - 2);
            cv::Moments moments = cv::moments(contour);
            this->x = (int) (moments.m10 / moments.m00);
            this->y = (int) (moments.m01 / moments.m00);
        }
        catch(std::out_of_range &oor)
        {
        }
        catch(std::overflow_error &oe)
        {
        } 
        catch(std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    bool contourAreaComp(const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
    {
        return cv::contourArea(a) < cv::contourArea(b);
    }
    
private:
    int threshold;
    int erosionSize = 1;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, 
                                               cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1),
                                               cv::Point(erosionSize, erosionSize));
};

#endif

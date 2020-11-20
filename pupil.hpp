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

    void findPupil(cv::Mat eyeFrame)
    {
        //cv::Mat pupilFrame = this->preprocess(eyeFrame, threshold);
        cv::Mat pupilFrame = preprocess(eyeFrame, this->threshold);

        cv::findContours(pupilFrame, this->contours, this->hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        //std::cout << contours.size() << " ";
        std::sort(contours.begin(), contours.end(),
                  [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
                  { return cv::contourArea(a, false) < cv::contourArea(b, false); });

        //for(auto &contour : contours) cout << cv::contourArea(contour) << " ";

        try
        {
            //cout << contours.size() - 2 << " ";
            std::vector<cv::Point> contour = contours.at(contours.size() - 2);
            cv::Moments moments = cv::moments(contour);
            this->x = moments.m10 / moments.m00;
            this->y = moments.m01 / moments.m00;
        }
        catch(std::out_of_range &oor)
        {
            //std::cerr << "out of range" << std::endl;
        }
        catch(std::overflow_error &oe)
        {
            //std::cerr << "divided by zero" << std::endl;
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

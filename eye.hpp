#ifndef eye_HPP
#define eye_HPP

#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/full_object_detection.h>

#include "calibration.hpp"

class Eye
{
public:
    int xmin, xmax, ymin, ymax;
    int xcenter, ycenter;

    Eye() {}

    cv::Mat analyze(cv::Mat &frame, std::vector<dlib::full_object_detection> &shapes, int side)
    {
        std::vector<cv::Point> eyeRegion;
        std::vector<std::vector<cv::Point>> temp;

        if(side == 0) // left eye
        {
            for(const int &i : leftEyePoints)
            {
                eyeRegion.push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }
        }
        else if(side == 1) // right eye
        {
            for(const int &i : rightEyePoints)
            {
                eyeRegion.push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }
        }
        temp.push_back(eyeRegion);

        cv::Mat blackFrame = cv::Mat(frame.size(), CV_8UC1, cv::Scalar(0));
        cv::Mat mask = cv::Mat(frame.size(), CV_8UC1, cv::Scalar(255));
        cv::fillPoly(mask, temp, cv::Scalar(0, 0, 0));
        cv::Mat eyeFrame = frame.clone();
        cv::bitwise_not(blackFrame, eyeFrame, mask=mask);

        // Crop the eye region
        xmin = (*min_element(eyeRegion.begin(), eyeRegion.end(), [](cv::Point i, cv::Point j) { return i.x < j.x; })).x - margin;
        xmax = (*max_element(eyeRegion.begin(), eyeRegion.end(), [](cv::Point i, cv::Point j) { return i.x < j.x; })).x + margin;
        ymin = (*min_element(eyeRegion.begin(), eyeRegion.end(), [](cv::Point i, cv::Point j) { return i.y < j.y; })).y - margin;
        ymax = (*max_element(eyeRegion.begin(), eyeRegion.end(), [](cv::Point i, cv::Point j) { return i.y < j.y; })).y + margin;

        xcenter = xmin + (xmax - xmin) / 2;
        ycenter = ymin + (ymax - ymin) / 2;

        return eyeFrame;
    }

private:
    const int leftEyePoints[6] = {36, 37, 38, 39, 40, 41};
    const int rightEyePoints[6] = {42, 43, 44, 45, 46, 47};
    const int margin = 5;
};

#endif

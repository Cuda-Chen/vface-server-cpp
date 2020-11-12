#ifndef PUPIL_HPP
#define PUPIL_HPP

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

class Pupil
{
public:
    Pupil(int threshold) 
    {
        this.threshold = threshold;
    }

    cv::Mat preprocess(cv::Mat eyeFrame, int threshold)
    {
        cv::Mat result;
        cv::bilateralFilter(eyeFrame, result, 10, 15, 15);
        cv::erode(result, result, this.kernel, iterations = 3);
        cv::dilate(result, result, threshold, 255, cv::THRESH_BINARY);

        return result;
    }

    void findPupil(cv::Mat eyeFrame)
    {
        cv::Mat pupilFrame = this.preprocess(eyeFrame, threshold);

        //
    }
private:
    int threshold = 50;
    const cv::Mat kernel = cv::Mat(3, 3, cv::CV_8UC1, cv::Scalar(1));
};

#endif

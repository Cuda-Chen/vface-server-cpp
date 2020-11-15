#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include <cmath>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

#include "preprocessing.hpp"

class Calibration
{
public:
    Calibration() {}

    inline bool isComplete()
    {
        return leftThreshold.size() >= numFrames && rightThreshold.size() >= numFrames;
    }

    int getThreshold(int side)
    {
        if(side == 0) // left eye
        {
            return std::accumulate(leftThreshold.begin(), leftThreshold.end(), 0) / leftThreshold.size();
        }
        else if(side == 1) // right
        {
            return std::accumulate(rightThreshold.begin(), rightThreshold.end(), 0) / rightThreshold.size();
        }
    }

    int getIrisSize(cv::Mat frame)
    {
        cv::Rect roi(5, 5, frame.rows - 5, frame.cols - 5);
        cv::Mat eyeFrame = cv::Mat(frame, roi);
        int numPixels = eyeFrame.total();
        int numBlackPixels = cv::countNonZero(eyeFrame);
        return numBlackPixels / numPixels;
    }

    int getBestThreshold(cv::Mat eyeFrame)
    {
        std::unordered_map<int, double> thresholdCandidates;

        for(int i = this->minThreshold; i <= this->maxThresnold; i += this->searchInterval)
        {
            cv::Mat irisFrame = preprocess(eyeFrame, i);
            thresholdCandidates[i] = this->getIrisSize(irisFrame);
        }

        auto it = min_element(thresholdCandidates.begin(), thresholdCandidates.end(),
                              [this](const auto &a, const auto &b)
                              { return std::abs(a.second - averageIrisSize) < std::abs(b.second - averageIrisSize); }
                             );
        int bestThreshold = it->first;
        return bestThreshold;
    }
private:
    const int numFrames = 20;
    std::vector<int> leftThreshold;
    std::vector<int> rightThreshold;
    const double averageIrisSize = 0.48;
    const int minThreshold = 5;
    const int maxThresnold = 100;
    const int searchInterval = 5;
};

#endif

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <cmath>
#include <vector>
#include <utility>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <numeric>

using std::vector;
using std::unordered_map;
using std::string;
using std::make_pair;
using std::accumulate;

typedef std::pair<int, int> point;

class Calculator
{
public:
    Calculator(double width) {
        this->width = width;
    }

    void updatePoints(vector<point> points)
    {
        this->points = points;
    }

    double distance(point &p1, point &p2)
    {
        return hypot(p1.first - p2.first, p1.second - p2.second);
    }

    point middle(point &p1, point &p2)
    {
        return make_pair((p1.first + p2.first) / 2, (p1.second + p2.second) / 2);
    }

    double avg(vector<double> &v)
    {
        return accumulate(v.begin(), v.end(), 0.0) / v.size();
    }

    double calcAngleX()
    {
        double t = this->distance(this->points[2], this->points[33]) - this->distance(this->points[33], this->points[14]);
        double d = this->distance(this->points[2], this->points[14]);
        return (t / d) * 50;
    }

    double getAngleXAvg()
    {
        this->angleX.push_back(this->calcAngleX());
        this->angleX.erase(this->angleX.begin());
        return avg(this->angleX);
    }

    double calcAngleY()
    {
        double t = this->distance(this->points[30], this->points[51]) - this->distance(this->points[28], this->points[30]);
        double d = this->distance(this->points[28], this->points[51]);
        return (t / d - 0.2) * 90;
    }

    double getAngleYAvg()
    {
        this->angleY.push_back(this->calcAngleY());
        this->angleY.erase(this->angleY.begin());
        return avg(this->angleY);
    }

    double calcAngleZ()
    {
        return atan2(this->points[27].first - this->points[33].first, this->points[33].second - this->points[27].second) * 100; 
    }

    double getAngleZAvg()
    {
        this->angleZ.push_back(this->calcAngleZ());
        this->angleZ.erase(this->angleZ.begin());
        return avg(this->angleZ);
    }

    double calcLeftEyeOpen()
    {
        double t = this->distance(this->points[43], this->points[47]) + this->distance(this->points[44], this->points[46]);
        double d = this->distance(this->points[23], this->points[43]) + this->distance(this->points[24], this->points[44]);
        return (t / d - 0.15) * 7;
    }

    double getLeftEyeOpen()
    {
        this->leftEyeOpen.push_back(this->calcLeftEyeOpen());
        this->leftEyeOpen.erase(this->leftEyeOpen.begin());
        return avg(this->leftEyeOpen);
    }

    double calcRightEyeOpen()
    {
        double t = this->distance(this->points[37], this->points[41]) + this->distance(this->points[38], this->points[40]);
        double d = this->distance(this->points[19], this->points[37]) + this->distance(this->points[20], this->points[38]);
        return (t / d - 0.15) * 7;
    }

    double getRightEyeOpen()
    {
        this->rightEyeOpen.push_back(this->calcRightEyeOpen());
        this->rightEyeOpen.erase(this->rightEyeOpen.begin());
        return avg(this->rightEyeOpen);
    }

    double calcEyeBallX()
    {
        try
        {
            double ln = this->distance(this->points[36], this->points[68]);
            double rn = this->distance(this->points[42], this->points[69]);
            return (-1 + ln / (ln + this->distance(this->points[39], this->points[68])) + rn / (rn + this->distance(this->points[45], this->points[69]))) * 3;
        }
        catch(std::out_of_range &oor)
        {
            return this->params["eyeBallX"].back();
        }
    }

    double getEyeBallX()
    {
        this->eyeBallX.push_back(this->calcEyeBallX());
        this->eyeBallX.erase(this->eyeBallX.begin());
        return avg(this->eyeBallX);
    }

    double calcEyeBallY()
    {
        try
        {
            point lt = this->middle(this->points[37], this->points[38]);
            point ld = this->middle(this->points[40], this->points[41]);
            point rt = this->middle(this->points[43], this->points[44]);
            point rd = this->middle(this->points[46], this->points[47]);
            double ln = this->distance(ld, this->points[68]);
            double rn = this->distance(rd, this->points[69]);
            return (-1.3 + ln / (ln + this->distance(lt, this->points[68])) + rn / (rn + this->distance(rt, this->points[69]))) * 3;
        }
        catch(std::out_of_range &oor)
        {
            return this->params["eyeBallY"].back();
        }
    }

    double getEyeBallY()
    {
        this->eyeBallY.push_back(this->calcEyeBallY());
        this->eyeBallY.erase(this->eyeBallY.begin());
        return avg(this->eyeBallY);
    }

    double calcMouthOpenY()
    {
        return (this->distance(this->points[62], this->points[66]) / this->distance(this->points[33], this->points[66])) * 2;
    }

    double getMouthOpenY()
    {
        this->mouthOpenY.push_back(this->calcMouthOpenY());
        this->mouthOpenY.erase(this->mouthOpenY.begin());
        return avg(this->mouthOpenY);
    }

    double calcBodyAngleZ()
    {
        double t = this->points[2].first + this->points[14].first - this->points[33].first - this->width / 2;
        return (t / this->width) * 100;
    }

    double getBodyAngleZ()
    {
        this->bodyAngleZ.push_back(this->calcBodyAngleZ());
        this->bodyAngleZ.erase(this->bodyAngleZ.begin());
        return avg(this->bodyAngleZ);
    }

private:
    vector<point> points;
    const int queueSize = 5;

    double width;
    vector<double> angleX = vector<double>(this->queueSize, 0);
    vector<double> angleY = vector<double>(this->queueSize, 0);
    vector<double> angleZ = vector<double>(this->queueSize, 0);
    vector<double> leftEyeOpen = vector<double>(this->queueSize, 1);
    vector<double> rightEyeOpen = vector<double>(this->queueSize, 1);
    vector<double> eyeBallX = vector<double>(this->queueSize, 0);
    vector<double> eyeBallY = vector<double>(this->queueSize, 0);
    vector<double> mouthOpenY = vector<double>(this->queueSize, 0);
    vector<double> bodyAngleZ = vector<double>(this->queueSize, 0);
    unordered_map<string, vector<double>> params = {
        {"angleX", this->angleX},
        {"angleY", this->angleY},
        {"angleZ", this->angleZ},
        {"leftEyeOpen", this->leftEyeOpen},
        {"rightEyeOpen", this->rightEyeOpen},
        {"eyeBallX", this->eyeBallX},
        {"eyeBallY", this->eyeBallY},
        {"mouthOpenY", this->mouthOpenY},
        {"bodyAngleZ", this->bodyAngleZ}
    };
};

#endif

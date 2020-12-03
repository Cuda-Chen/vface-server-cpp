#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <cmath>
#include <vector>
#include <utility>

using std::vector;

typedef std::pair<int, int> point;

class Calculator
{
public:
    Calculator(double width) {
        this->width = width;
    }

    void update(vector<point> points)
    {
        this->points = points;
    }

    double distance(point &p1, point &p2)
    {
        return hypot(p1.first - p2.first, p1.second - p2.second);
    }

    point moddle(point &p1, point &p2)
    {
        return std::make_pair((p1.first + p2.first) / 2, (p1.second + p2.second) / 2);
    }

    double calcAngleX()
    {
        double t = this->distance(points[2], points[33]) - this->distance(points[33], points[14]);
        double d = this->distance(points[2], points[14]);
        return (t / d) * 50;
    }

    double calcAngleY()
    {
        double t = this->distance(points[30], points[51]) - this->distance(points[28], points[30]);
        double d = this->distance(points[28], points[51]);
        return (t / d - 0.2) * 90;
    }

    double calcAngleZ()
    {
        return atan2(points[27].first - points[33].first, points[33].second - points[27].second) * 100; 
    }

    double calcLeftEyeOpen()
    {
        double t = this->distance(points[43], points[47]) + this->distance(points[44], points[46]);
        double d = this->distance(points[23], points[43]) + this->distance(points[24], points[44]);
        return (t / d - 0.15) * 7;
    }

    double calcRightEyeOpen()
    {
        double t = this->distance(points[37], points[41]) + this->distance(points[38], points[40]);
        double d = this->distance(points[19], points[37]) + this->distance(points[20], points[38]);
        return (t / d - 0.15) * 7;
    }

    double calcEyeBallX()
    {//
    }

    double calcEyeBallY()
    {//
    }

    double calcMouthOpenY()
    {
        return (this->distance(points[62], points[66]) / this->distance(points[33], points[66])) * 2;
    }

    double calcBodyAngleZ()
    {
        double t = points[2].first + points[14].first - points[33].first - this->width / 2;
        return (t / width) * 100;
    }
private:
    vector<point> points;
    double width;
};

#endif

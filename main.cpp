#include <iostream>
#include <chrono>
#include <dlib/opencv.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <vector>
#include <utility>
#include <thread>

#include "eye.hpp"
#include "pupil.hpp"
#include "calculator.hpp"

using namespace dlib;
using namespace std;
using namespace std::chrono;

milliseconds delay(1); // 1 ms

int main()
{
    cout.setf(std::ios::unitbuf);

    try
    {
        cv::Mat frame;
        //cv::VideoCapture cap("test.mp4"); // use test video for testing
        cv::VideoCapture cap(0);
        if(!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        // Resize to appropriate size to speed up
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        Calculator calc = Calculator(cv::CAP_PROP_FRAME_WIDTH);

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("./trained_models/shape_predictor_68_face_landmarks.dat") >> pose_model;

        while(true)
        {
            cap >> frame;
            if(frame.empty()) break;

            //auto start = high_resolution_clock::now();
            // Detect face as well as landmarks
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
            cv_image<unsigned char> cimg(frame);
            std::vector<rectangle> faces = detector(cimg);
            std::vector<full_object_detection> shapes;
            if(!faces.empty())
            {
                shapes.push_back(pose_model(cimg, faces[0]));

                // eye
                Eye l = Eye();
                Eye r = Eye();
                cv::Mat leftTemp = l.analyze(frame, shapes, 0);
                cv::Mat rightTemp = r.analyze(frame, shapes, 1);
                cv::Rect leftRect(l.xmin, l.ymin, l.xmax - l.xmin, l.ymax - l.ymin);
                cv::Rect rightRect(r.xmin, r.ymin, r.xmax - r.xmin, r.ymax - r.ymin);
                cv::Mat leftEye = cv::Mat(leftTemp, leftRect);
                cv::Mat rightEye = cv::Mat(rightTemp, rightRect);

                // calibration
                Calibration calibration = Calibration();
                if(!calibration.isComplete())
                {
                    calibration.evaluate(leftEye, 0);
                    calibration.evaluate(rightEye, 1);
                }

                // pupil
                Pupil lPupil = Pupil(calibration.getThreshold(0));
                Pupil rPupil = Pupil(calibration.getThreshold(1));
                lPupil.findPupil(leftEye);
                rPupil.findPupil(rightEye);

                // detection test
                /*cv::Mat output = frame.clone(); 
                if(lPupil.pupilIsLocated()) cv::circle(output, cv::Point(l.xmin + lPupil.x, l.ymin + lPupil.y), 1, cv::Scalar(255));
                if(rPupil.pupilIsLocated()) cv::circle(output, cv::Point(r.xmin + rPupil.x, r.ymin + rPupil.y), 1, cv::Scalar(255));
                cv::imshow("detections", output);*/

                std::vector<std::pair<int, int>> points;
                for(int i = 0; i < 68; i++)
                {
                    std::pair<int, int> p = std::make_pair(shapes[0].part(i).x(), shapes[0].part(i).y());
                    points.push_back(p);
                }
                points.push_back({l.xmin + lPupil.x, l.ymin + lPupil.y});
                points.push_back({r.xmin + rPupil.x, r.ymin + rPupil.y});
                calc.updatePoints(points);
                //cout << calc.getAngleXAvg() << endl;
                cout << "AngleX " << calc.getAngleXAvg() << endl
                     << "AngleY " << calc.getAngleYAvg() << endl
                     << "AngleZ " << calc.getAngleZAvg() << endl 
                     << "EyeLOpen " << calc.getLeftEyeOpen() << endl
                     << "EyeROpen " << calc.getRightEyeOpen() << endl
                     << "EyeBallX " << calc.getEyeBallX() << endl
                     << "EyeBallY " << calc.getEyeBallY() << endl
                     << "MouthOpenY " << calc.getMouthOpenY() << endl
                     << "BodyAngleZ " << calc.getBodyAngleZ() << endl;
            }
            /*auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << duration.count() << " microseconds" << endl;*/
            std::this_thread::sleep_for(delay);

            if(cv::waitKey(1) == 27)
            {
                break;
            }
        }
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run vface_server_cpp." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

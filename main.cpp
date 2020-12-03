#include <iostream>
#include <chrono>
#include <dlib/opencv.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

#include "eye.hpp"
#include "pupil.hpp"
#include "calculator.hpp"

using namespace dlib;
using namespace std;
using namespace std::chrono;

int main()
{
    cout.setf(std::ios::unitbuf);

    try
    {
        cv::Mat frame;
        cv::VideoCapture cap("test.mp4"); // use test video for testing
        //cv::VideoCapture cap(0);
        if(!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        // Resize to appropriate size to speed up
        //cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        //cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("./trained_models/shape_predictor_68_face_landmarks.dat") >> pose_model;

        while(true)
        {
            cap >> frame;
            if(frame.empty()) break;

            auto start = high_resolution_clock::now();
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

                // calibration
                Calibration calibration = Calibration();
                if(!calibration.isComplete())
                {
                    calibration.evaluate(cv::Mat(leftTemp, leftRect), 0);
                    calibration.evaluate(cv::Mat(rightTemp, rightRect), 1);
                }

                // pupil

                Pupil lPupil = Pupil(calibration.getThreshold(0));
                Pupil rPupil = Pupil(calibration.getThreshold(1));

                lPupil.findPupil(cv::Mat(leftTemp, leftRect));
                rPupil.findPupil(cv::Mat(rightTemp, rightRect));


                cv::Mat leftEye = frame.clone(); 
                if(lPupil.pupilIsLocated()) cv::circle(leftEye, cv::Point(l.xmin + lPupil.x, l.ymin + lPupil.y), 1, cv::Scalar(255));
                if(rPupil.pupilIsLocated()) cv::circle(leftEye, cv::Point(r.xmin + rPupil.x, r.ymin + rPupil.y), 1, cv::Scalar(255));
                cv::imshow("detections", leftEye);
            }
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);

            cout << duration.count() << " microseconds" << endl;

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

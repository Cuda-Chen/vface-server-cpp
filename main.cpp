#include <iostream>
#include <chrono>
#include <dlib/opencv.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

using namespace dlib;
using namespace std;
using namespace std::chrono;

int main()
{
    cout.setf(std::ios::unitbuf);

    try
    {
        cv::Mat frame;
        cv::VideoCapture cap(0);
        if(!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        // Resize to appropriate size to speed up
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("./trained_models/shape_predictor_68_face_landmarks.dat") >> pose_model;

        while(true)
        {
            cap >> frame;

            //auto start = high_resolution_clock::now();
            // Detect face as well as landmarks
            cv_image<bgr_pixel> cimg(frame);
            std::vector<rectangle> faces = detector(cimg);
            std::vector<full_object_detection> shapes;
            if(!faces.empty())
            {
                shapes.push_back(pose_model(cimg, faces[0]));
                for(int i = 0; i < 68; i++)
                {
                    cv::circle(frame, cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(255, 255, 0), -1);
                }

                if(!shapes.empty())
                {
                    cout << shapes[0].part(0).x();
                    cout << " " << shapes[0].part(0).y();
                    cout << " " << shapes[0].part(67).x();
                    cout << " " << shapes[0].part(67).y() << endl;
                }
            }
            //auto stop = high_resolution_clock::now();

            //auto duration = duration_cast<microseconds>(stop - start);

            cv::imshow("vface_server_cpp", frame);
            //cout << duration.count() << " microseconds" << endl;

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

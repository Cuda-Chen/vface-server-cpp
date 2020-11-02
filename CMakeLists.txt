cmake_minimum_required(VERSION 3.14)
project(v_face_server_cpp)
set(EXECUTABLE_OUTPUT_PATH "..")

# force C++11
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch dlib
include(FetchContent)
FetchContent_Declare(dlib
    GIT_REPOSITORY https://github.com/davisking/dlib.git
    GIT_TAG        v19.21
)
FetchContent_MakeAvailable(dlib)

# OpenCV
find_package(OpenCV REQUIRED)

add_executable(v_face_server_cpp webcam_face_pose_ex.cpp)
target_link_libraries(v_face_server_cpp dlib::dlib ${OpenCV_LIBS})

#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat src = imread(argv[1], 1);
    if (!src.data)
    {
        return -1;
    }

    Mat hsv, green;
    cvtColor(src, hsv, CV_BGR2HSV);
    imshow("gfp", hsv);
    inRange(hsv, Scalar(50, 0, 0), Scalar(70, 255, 255), green);

    imshow("green", green);

 
    waitKey(0);

    return 0;
}
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat hsv;

void onMouse(int event, int x, int y, int flag, void* param)
{
    if (event == EVENT_LBUTTONDOWN) {
        /*To access each pixel in the images we are using this syntax: image.at(y,x)[c] where y is the row,
          x is the column and c is H, S or V (0, 1 or 2)*/
        Vec3b p = hsv.at<Vec3b>(y, x);

        std::cout << "x : " << x << " y : " << y << std::endl;
        std::cout << "H=" << (int) p.val[0] << ", S=" << (int) p.val[1] << ", V=" << (int) p.val[2]; //p[0] - H, p[1] - S, p[2] - V
    }
}

int main(int argc, char** argv)
{

    VideoCapture inputVideo(argv[1]);

    if (!inputVideo.isOpened())
    {
        std::cout  << "Could not open the input video " << std::endl;
        return -1;
    }

    Mat src;

    int i;
    while (true) //Show the image captured in the window and repeat
    {
        inputVideo >> src;
        if (src.empty()) break;

        std::cout << "frame " << i << std::endl;

        Mat green;
        cvtColor(src, hsv, CV_BGR2HSV);
        imshow("src", src);

        // get the range in hsv corresponding to the color that we want to track
        inRange(hsv, Scalar(85, 140, 0), Scalar(95, 255, 255), green);

        //morphologyEx(green, green, MORPH_OPEN, Mat());
        //morphologyEx(green, green, MORPH_CLOSE, Mat());

        medianBlur(green, green, 7);

        //imshow("green", green);

        setMouseCallback("src", onMouse, 0);

        cvtColor(green, green, CV_GRAY2BGR);

        Mat onlygreen = src & green;
     
        //imshow("onlygreen", onlygreen);

        std::stringstream ss;


        // write the images
        ss << "images/frame_" << i << ".png";


        // Extract only the chambers that we want to look at
        Mat roi(onlygreen, Rect(1020,254,105,210));

        imwrite(ss.str(), roi);

        i++;

    }

    std::cout << "Finished writing" << std::endl;

    return 0;
}
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    // open the default camera
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        printf("Something went wrong while retrieve the webcam\n");
        return -1;
    }

    Mat light, frame, mask;
    namedWindow("tracking", 1);
    int key;
    while (key != 'q' && key != 'Q')
    {
         // get a new frame from camera
        cap >> frame;

         // Go from BGR space to Y Cr Cb 
        cvtColor(frame, light, CV_BGR2YCrCb);

        // Threshold the image given the light intensity
        inRange(light, Scalar(200, 0, 0), Scalar(255, 255, 255), mask);

        /// Reduce the noise for circle detection
        GaussianBlur(mask, mask, Size(15, 15), 5, 5);

        vector<Vec3f> circles;

        /// Apply the Hough Transform to find the circles
        HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 2, mask.rows/8, 30, 50, 0, 0 );
        

        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle outline
            circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0);
        }

        imshow("tracking", frame);

        key = waitKey(30);
    }

    return 0;
}
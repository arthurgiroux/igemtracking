#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;



// Compute the distance between two 2D points
double distanceBetweenPoints(Point2f p1, Point2f p2) {
    // sqrt((x1 - x2)^2 + (y1 - y2)^2)
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}


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

    Point lastPoint; // latest point tracked
    int i = 0; // number of frames
    while (key != 'q' && key != 'Q')
    {
         // get a new frame from camera
        cap >> frame;

         // Go from BGR space to Y Cr Cb 
        cvtColor(frame, light, CV_BGR2YCrCb);

        // Threshold the image given the light intensity
        inRange(light, Scalar(200, 0, 0), Scalar(255, 255, 255), mask);

        // Reduce the noise for circle detection
        GaussianBlur(mask, mask, Size(15, 15), 5, 5);

        vector<Vec3f> circles;

        // Apply the Hough Transform to find the circles
        HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 2, mask.rows/8, 30, 50, 0, 0 );
        

        // Get the closest circle to the previous one
        Vec3f goodCircle;
        double mindistance = -1;

        for (size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            double distance = distanceBetweenPoints(center, lastPoint);

            if (mindistance == -1 || distance < mindistance) {
                std::cout << "new point" << std::endl;
                lastPoint = center;
                mindistance = distance;
                goodCircle = circles[i];
            }
        }

        if (circles.size() > 0) {

            std::cout << "drawing circle" << std::endl;
            // circle outline
            Point center(cvRound(goodCircle[0]), cvRound(goodCircle[1]));
            int radius = cvRound(goodCircle[2]);

            circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0);
        }


        std::stringstream ss;

        // write the image
        ss << "imagestracking/frame_" << i << ".png";

        imwrite(ss.str(), frame);
        imshow("tracking", frame);

        key = waitKey(30);

        i++;
    }

    return 0;
}
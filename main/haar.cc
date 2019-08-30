#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>


using namespace std;
using namespace cv;


void detectAndDisplay( Mat frame );
String light_cascade_name = "cascade.xml";
CascadeClassifier light_cascade;
String window_name = "detection";


int main()
{
    Mat frame;
    // cv::Rect roi(460, 160, 500, 380);
     VideoCapture capture("/home/melvin/data/data.mp4");

    /********Load the cascades********/

    if( !light_cascade.load( light_cascade_name ) ){ printf("Error loading the cascade\n"); return -1; };

    /*********** Load the video stream***********/
    
    while (1)
    {
        capture >> frame;
        if( frame.empty() )
        {
            printf("No captured frame \n");
            break;
        }

        /*********** Apply the classifier to the frame ************/
        detectAndDisplay( frame );
        if (waitKey(1) >= 0)
         break;
    }
    return 0;
}


void detectAndDisplay( Mat frame )
{
    // frame = frame(roi);
    std::vector<Rect> standalone;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray ); 

    /********** Detect lights *********/
    light_cascade.detectMultiScale( frame_gray, standalone, 1.3, 10, 0|CASCADE_SCALE_IMAGE, Size(20, 20) );     // 1.3 = Scale Factor, 10 = Minimus Distance of neighbour, Size = Minimum pixel size.
        size_t i;
        Point center( standalone[i].x + standalone[i].width/2, standalone[i].y + standalone[i].height/2 );
        ellipse( frame, center, Size( standalone[i].width/2, standalone[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );     // Drawing Ellipse
    imshow( window_name, frame );
}

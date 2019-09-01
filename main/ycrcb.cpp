// #include "opencv2/opencv.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/***************** Load the Cascade Classifier Xml file *******************/
String light_cascade_name = "cascade.xml";
CascadeClassifier type;

int main(void){
  
  if( !type.load( light_cascade_name ) ){ cout << "Error loading the cascade\n"; return -1; };
  
  /*********** Load Video *************/
  VideoCapture cap("/home/melvin/Videos/short.mp4");

  // Check if camera opened successfully
  Mat frame, frameBig, frameGray;
   
  while (1){
    
    /************ Reading each frame **************/
    bool frameRead = cap.read(frameBig);

    /********** If frame not opened successfully ****************/
    if (!frameRead){
            cout << "No captured frame \n";
            break;
        }
        
    /*********** Resizing the image *************/
    resize(frameBig, frameBig, Size(960, 540));
    // imwrite( "ellipse.png", frame );
    frame = frameBig;
    // cv::Rect roi(400, 258, 200, 200);
    cv::Rect roi(280, 190, 200, 200);
    frame = frame(roi);

    /********** Converting to grayscale ***********/
    cvtColor(frame, frameGray, COLOR_BGR2GRAY);

    /**************** Creating vector to store the detected light parameters **************/
    vector<Rect> light;

    /************** Detect light using HAAR like cascade *****************/
    type.detectMultiScale(frameGray,light,1.1,25,0|CASCADE_SCALE_IMAGE, Size(20, 20));
    
    /**************** Loop over each detected light ****************/
          for ( int i = 0; i < light.size(); i++)
              {
    /***************** Dimension parameters for bounding rectangle for light ********************/
              Rect faceRect = light[i];
              Mat res;
              
              res = frame(light[i]); 
              cv::cvtColor( res, res, CV_BGR2YCrCb); 
              Mat red;
              inRange(res, cv::Scalar(0, 142, 0), Scalar(255, 255, 124), red);

              Mat green;
              inRange(res, cv::Scalar(0, 0, 0), Scalar(255, 138, 117), green);
              Mat yellow;
              inRange(res, Scalar(60, 100, 103), Scalar(169, 158, 120), yellow);

              int intred, intgreen, intyellow;
              intred = countNonZero(red);
              intgreen = countNonZero(green);
              intyellow = countNonZero(yellow);
              // cout<<intgreen << " " << intyellow ;
              cvtColor( res, res, CV_YCrCb2BGR); 

                    if (intred > intgreen){
                       Rect eyesRect = Rect(faceRect.x + 0.125*faceRect.width, faceRect.y + 0.25 * faceRect.height, 0.85 * faceRect.width,
                       0.65 * faceRect.height);
                      rectangle(frame, eyesRect,Scalar(0,0,255), 2);
                      cv::putText(frame, "RED", cv::Point(20,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,0,255), 1, CV_AA);
                                          }
                    else if (intgreen > intred){
                          Rect eyesRect = Rect(faceRect.x + 0.125*faceRect.width, faceRect.y + 0.25 * faceRect.height, 0.85 * faceRect.width,
                          0.65 * faceRect.height);
                          rectangle(frame, eyesRect,Scalar(0,255,0), 2);
                          cv::putText(frame, "GREEN", cv::Point(20,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,0), 1, CV_AA);
                                              }
                    else if (intyellow > (intred+intgreen)/2){     // (intred+intgreen)/2
                          Rect eyesRect = Rect(faceRect.x + 0.125*faceRect.width, faceRect.y + 0.25 * faceRect.height, 0.85 * faceRect.width,
                          0.65 * faceRect.height);
                          rectangle(frame, eyesRect,Scalar(0,255,255), 2);
                          cv::putText(frame, "YELLOW", cv::Point(20,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,255), 1, CV_AA);
                                              }
                    else{
                        Rect eyesRect = Rect(faceRect.x + 0.125*faceRect.width, faceRect.y + 0.25 * faceRect.height, 0.85 * faceRect.width,
                        0.65 * faceRect.height);
                        rectangle(frame, eyesRect,Scalar(255,255,255), 2);
                        cv::putText(frame, "No Status", cv::Point(20,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,0,255), 1, CV_AA);
                        
                        }
              }        
    
    /*************** Display the resulting frame ***************/
    // resize(frame, frame, Size(1820, 960));
    rectangle(frameBig, roi,Scalar(255,255,255), 1);
    imshow("result", frameBig);
    int k = waitKey(1);
    if(k == 27)
      break;
  }
  cap.release();
  destroyAllWindows();
  return 0;
}


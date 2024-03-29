#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
using namespace std;
using namespace cv;
bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
int match_method;
int max_Trackbar = 5;
void MatchingMethod( int, void* );


  Mat src, src_gray, grad;
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  int c;


int main( int argc, char** argv )
{
  if (argc < 3)
  {
    cout << "Not enough parameters" << endl;
    cout << "Usage:\n./MatchTemplate_Demo <image_name> <template_name> [<mask_name>]" << endl;
    return -1;
  }
  src = imread( argv[1], IMREAD_COLOR );
  templ = imread( argv[2], IMREAD_COLOR );
  // cv::resize(img, img, cv::Size(640, 480)); 
  // cv::imwrite("save.jpg", img);	
  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
  cvtColor( src, src_gray, CV_BGR2GRAY );
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  img = grad;


  if(argc > 3) {
    use_mask = true;
    mask = imread( argv[3], IMREAD_COLOR );
  }
  if(img.empty() || templ.empty() || (use_mask && mask.empty()))
  {
    cout << "Can't read one of the images" << endl;
    return -1;
  }
  namedWindow( image_window, WINDOW_AUTOSIZE );
  namedWindow( result_window, WINDOW_AUTOSIZE );
  const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
  MatchingMethod( 0, 0 );
  while(char(waitKey(1)) != 'q'){}
  return 0;
}
void MatchingMethod( int, void* )
{
  Mat img_display;
  cv::imwrite("img.png", img);
  // imshow("img", img);
  img.copyTo( img_display );
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;
  result.create( result_rows, result_cols, CV_32FC1 );
  bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
  if (use_mask && method_accepts_mask)
  { matchTemplate( img, templ, result, match_method, mask); }
  else
  { 
      std::cout << "Debugging" << std::endl;
      matchTemplate( img, templ, result, match_method); }
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar(0, 255, 0), 2, 8, 0 );
  // rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  //  cv::resize(result, result, cv::Size(1200, 747));
  
  imshow( image_window, img_display );
  // imshow( result_window, result );
  return;
}
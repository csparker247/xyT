#include <iostream>
#include <opencv2/opencv.hpp>

int main( int argc, char** argv ) {

  cv::VideoCapture Video( argv[1] ); // open the default camera
  if(!Video.isOpened())  // check if we succeeded
    return -1;

  double width = Video.get( CV_CAP_PROP_FRAME_WIDTH);
  double height = Video.get( CV_CAP_PROP_FRAME_HEIGHT );
  double time = Video.get( CV_CAP_PROP_FRAME_COUNT );
  bool needsColorCvt = Video.get(CV_CAP_PROP_CONVERT_RGB);

  cv::Mat output (height, time, CV_8UC3 );

  cv::Mat frame;
  double current_output_index = 0;

  while ( ( Video.read(frame) ) != false ) {

    cv::Mat centerCol;
    frame.col(width/2).copyTo(centerCol);

    centerCol.copyTo(output.col(current_output_index));

    ++current_output_index;
  }

  cv::imwrite("output.png", output);

  return 0;
}
#include <iostream>
#include <opencv2/opencv.hpp>

int main( int argc, char** argv ) {

  cv::VideoCapture Video( argv[1] ); // open the default camera
  if(!Video.isOpened())  // check if we succeeded
    return -1;

  double frameWidth = Video.get( CV_CAP_PROP_FRAME_WIDTH);
  double frameHeight = Video.get( CV_CAP_PROP_FRAME_HEIGHT );
  double frameTime = ceil(Video.get( CV_CAP_PROP_FRAME_COUNT )); // round up
  bool needsColorCvt = Video.get(CV_CAP_PROP_CONVERT_RGB);

  std::vector<cv::Mat> columns;

  cv::Mat frame; // The current decoded frame
  double current_output_column = 0; // The output column index of the current frame

  while ( ( Video.read(frame) ) != false ) {
    if ( frame.empty() ) continue;
    std::cerr << "Unravelling the fabric of space-time for frame: " << current_output_column + 1 << "/" << frameTime;
    if ( current_output_column > frameTime )
      std::cerr << "\?\?\?!?";
    std::cerr << "\r" << std::flush;

    // For each column in the frame, copy its corresponding mat in outputImages
    int input_col_index = frameWidth/2;
    cv::Mat input_col_mat;
    frame.col(input_col_index).copyTo(input_col_mat);
    columns.push_back(input_col_mat);
  
    ++current_output_column; // Next column
  }
  std::cerr << std::endl;

  cv::Mat output(frameHeight, columns.size(), CV_8UC3);
  for ( int col_counter = 0; col_counter < columns.size(); ++col_counter) {
    std::cerr << "Restitching reality: " << col_counter + 1 << "/" << columns.size() << "\r" << std::flush;
    columns[col_counter].copyTo(output.col(col_counter));
  }
  std::cerr << std::endl;

  std::string outputDir = "";
  if ( argc > 2 )
    outputDir = argv[2];

  std::string outputName = outputDir + "output.png";
  cv::imwrite(outputName, output);


  return 0;
}
#include <iostream>
#include <opencv2/opencv.hpp>

int main( int argc, char** argv ) {

  cv::VideoCapture Video( argv[1] ); // open the default camera
  if(!Video.isOpened())  // check if we succeeded
    return -1;

  double frameWidth = Video.get( CV_CAP_PROP_FRAME_WIDTH);
  double frameHeight = Video.get( CV_CAP_PROP_FRAME_HEIGHT );
  double frameTime = ceil(Video.get( CV_CAP_PROP_FRAME_COUNT )) + ceil(Video.get( CV_CAP_PROP_FPS )); // round up and add another second of frames because you can't trust video headers
  bool needsColorCvt = Video.get(CV_CAP_PROP_CONVERT_RGB);

  std::vector<cv::Mat> outputImages;
  for (int i = 0; i < 10; ++i) {
    cv::Mat output = cv::Mat::zeros(frameHeight, frameTime, CV_8UC3);
    outputImages.push_back(output);
  }

  cv::Mat frame; // The current decoded frame
  double current_output_column = 0; // The output column index of the current frame

  while ( ( Video.read(frame) ) != false ) {
    if ( frame.empty() ) continue;
    std::cerr << "Swapping time and space for frame: " << current_output_column + 1 << "\r" << std::flush;

    // For each column in the frame, copy its corresponding mat in outputImages
    for ( int input_col_index = 0; input_col_index < outputImages.size(); ++input_col_index ) {
      cv::Mat input_col_mat;
      frame.col(input_col_index).copyTo(input_col_mat);

      input_col_mat.copyTo( outputImages[input_col_index].col(current_output_column) );
    }

    ++current_output_column; // Next column
  }
  std::cerr << std::endl;

  std::string outputDir = "";
  if ( argc > 2 )
    outputDir = argv[2];

  for ( int outCounter = 0; outCounter < outputImages.size(); ++outCounter){
    std::cerr << "Encoding video frame: " << outCounter + 1 << "\r" << std::flush;
    std::string outputName = outputDir + std::to_string(outCounter) + ".png";
    cv::imwrite(outputName, outputImages[outCounter]);
  }
  std::cerr << std::endl;

  return 0;
}
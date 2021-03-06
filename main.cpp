#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

std::vector< std::vector<cv::Mat> > images;
int max_slices_in_memory = 2000;

void emptyImages(int suffix, int frameHeight, std::string outputDir) {
  std::cerr << std::endl;
  for ( int image = 0 ; image < images.size() ; ++image ) {
    cv::Mat output(frameHeight, images[image].size(), CV_8UC3);
    for ( int col_counter = 0; col_counter < images[image].size(); ++col_counter) {
      std::cerr << "Mixing up the fiddly bits: [" << image << "] " << col_counter + 1 << "/" << images[image].size() << "\r" << std::flush;
      images[image][col_counter].copyTo(output.col(col_counter));
    }

    std::string outputName = outputDir + std::to_string(image) + "_" + std::to_string(suffix) + ".png";
    cv::imwrite(outputName, output);

    images[image].clear();
  }
  std::cerr << std::endl;
}

int main( int argc, char** argv ) {

  cv::VideoCapture Video( argv[1] ); // open the default camera
  if(!Video.isOpened())  // check if we succeeded
    return -1;

  std::string outputFinal = "";
  outputFinal = argv[2];

  bool cleanupTmp = true;

  boost::filesystem::create_directory("tmp");
  std::string outputDir = "tmp/";

  double frameWidth = Video.get(cv::CAP_PROP_FRAME_WIDTH);
  double frameHeight = Video.get(cv::CAP_PROP_FRAME_HEIGHT);
  double frameTime = std::ceil(Video.get(cv::CAP_PROP_FRAME_COUNT));
  bool needsColorCvt = Video.get(cv::CAP_PROP_CONVERT_RGB);

  for ( int i = 0; i < frameWidth; ++i ) {
    std::vector<cv::Mat> columns;
    images.push_back(columns);
  }

  // The current decoded frame
  cv::Mat frame;
  // The output column index of the current frame
  double current_frame = 0.0;

  while ( ( Video.read(frame) ) != false ) {
    if ( frame.empty() ) continue;
    std::cerr << "Unravelling the fabric of space-time: " << current_frame + 1 << "/" << frameTime;
    if ( current_frame > frameTime )
      std::cerr << "\?\?\?!?";
    std::cerr << "\r" << std::flush;

    // For each column in the frame, copy its corresponding col into the vector
    for ( int input_col_index = 0; input_col_index < frameWidth; ++input_col_index ) {
      cv::Mat input_col_mat;
      frame.col(input_col_index).copyTo(input_col_mat);
      images[input_col_index].push_back(input_col_mat);
    }

    if ( ((int) current_frame + 1) % max_slices_in_memory == 0 ) {
      int suffix = (int) current_frame / max_slices_in_memory;
      emptyImages( suffix, frameHeight, outputDir );
    }

    ++current_frame; // Next column
  }

  // Write out any lingering cols
  int suffix = (int) current_frame / max_slices_in_memory;
  emptyImages( suffix, frameHeight, outputDir );

  // Restitch images
  for ( int input_col_index = 0; input_col_index < frameWidth; ++input_col_index ) {
    cv::Mat finalImage( frameHeight, current_frame, CV_8UC3 );
    for ( int partial_img = 0; partial_img <= suffix; ++partial_img ) {
      std::cerr << "Making it just work: [" << input_col_index << "] " << partial_img << "/" << suffix << "\r" << std::flush;
      std::string partialImgPath = outputDir + std::to_string(input_col_index) + "_" + std::to_string(partial_img) + ".png";
      cv::Mat temp = cv::imread( partialImgPath );
      temp.copyTo( finalImage(cv::Rect( partial_img * max_slices_in_memory, 0, temp.cols, temp.rows)) );
      if (cleanupTmp) boost::filesystem::remove( partialImgPath );
    }
    std::string outputName = outputFinal + std::to_string(input_col_index) + ".png";
    // To-Do: Try the VideoWriter again
    cv::imwrite(outputName, finalImage);
  }
  std::cerr << std::endl;

  if (cleanupTmp) boost::filesystem::remove_all("tmp/");

  return 0;
}

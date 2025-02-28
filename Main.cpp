#include "AsciiConversor/ImageProcessor.hpp"
#include "AsciiConversor/VideoProcessor.hpp"
#include <exception>
#include <iostream>
#include <vector>

// using namespace cv;

int main() {
  
  // try {
  //   ImageProcessor processor("zebra.png");
  //
  //   processor.displayImage("Original Image");
  //
  //   cv::Mat pixelizedImg = processor.getPixelizedImage(6);
  //   cv::imshow("Pixelized Image", pixelizedImg);
  //
  //   cv::Mat resizedImg = processor.getResizedImage(150, 1.8);
  //   cv::imshow("Resized Image", resizedImg);
  //
  //   std::string asciiChars = "  ` .'`^,-~:;<>i!lI?+_1|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao23456789*#MW&8%B@";
  //   processor.displayASCIIArt(resizedImg, asciiChars);
  //
  //   cv::waitKey(0);
  // } catch (const std::exception& ex) {
  //   std::cerr << ex.what() << std::endl;
  //   return 1;
  // }
  
  try {
    VideoProcessor processor ("mickey.gif");
    
    std::vector<cv::Mat> video;

    std::string asciiChars = "  ` .'`^,-~:;<>i!lI?+_1|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao23456789*#MW&8%B@";
    video = processor.getResizedVideo(160, 1.8, 6);
    while(1){ 
      processor.displayASCIIArt(video, asciiChars); 
    }

  }
  catch (const std::exception& ex){
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0; 
}

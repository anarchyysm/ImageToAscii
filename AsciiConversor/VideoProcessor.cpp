#include "VideoProcessor.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>
#include <iostream>
#include <vector>
 

VideoProcessor::VideoProcessor(const std::string &videoPath){
  video = cv::VideoCapture(videoPath);
  int delay = static_cast<int>(1000 / video.get(cv::CAP_PROP_FPS));


 //cv::VideoCapture cap("input_video.mp4");
  if(!video.isOpened()){
    throw std::runtime_error("Cold not read the video: " + videoPath);
  }
}

void VideoProcessor::displayVideo(const std::string &windowName) {        
  while(video.read(frame)){
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::imshow(windowName, frame_gray);
    
    if (cv::waitKey(delay) >= 0) break;
  }
}


std::vector<cv::Mat> VideoProcessor::getPixelizedVideo(int blockSize) {

  cv::Mat smallFrame, pixelizedFrame;

  while(video.read(frame)){
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::resize(frame_gray, smallFrame, cv::Size(frame_gray.cols / blockSize, frame_gray.rows / blockSize), 0, 0, cv::INTER_LINEAR);
    cv::resize(smallFrame, pixelizedFrame, cv::Size(frame_gray.cols, frame_gray.rows), 0, 0, cv::INTER_NEAREST);
    
    return pixelizedFrame;
  }  
  return cv::Mat();
 }

void VideoProcessor::displayPixelizedVideo(const std::string &windowName, int blockSize){
  std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);
  
  if (!video.isOpened()) return;

  for(const cv::Mat &frame : pixelizedFrames){
    cv::imshow(windowName, frame);
    if(cv::waitKey(delay) >= 0) break;
  } 
}

// diplayPixelized pega de -> std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);

std::vector<cv::Mat> VideoProcessor::getResizedVideo(int newWidth, double aspectRatio){
  
  std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(6);
  cv::Mat resizedFrame;  

  double proportion = static_cast<int>(frame.rows) / static_cast<double>(frame.cols);
  int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);

  for(const cv::Mat &frame : pixelizedFrames){
    cv::resize(frame, resizedFrame, cv::Size(newWidth, newHeight));
    return resizedFrame;
  }
  return cv::Mat(); 
}

// peguei o pixelizedVideo e o resizedVideo, agora falta o dispayResizedvideo

void VideoProcessor::displayResizedVideo(const std::string &windowName){
  std::vector<cv::Mat> resizedFrames = getResizedVideo(150, 1.8);
  
  if(!video.isOpened()) return;

  for(const cv::Mat &frame : resizedFrames){
    cv::imshow(windowName, frame);
    if(cv::waitKey(delay) >= 0) break;
  }
}

void VideoProcessor::displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length();

  for(const cv::Mat &frame : video){
    for(int y = 0; y < frame.rows; y++){
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        int intensity = frame.at<uchar>(y, x);
        char asciiChar = asciiChars[intensity * numChars / 256];
        asciiLine += asciiChar;
      }
      std::cout << " " << asciiLine << std::endl;
    }
    std::cout << std::endl;
  }
}

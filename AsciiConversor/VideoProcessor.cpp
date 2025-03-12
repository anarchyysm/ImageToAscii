#include "VideoProcessor.hpp"
#include <BinFiles/WriteBin.hpp>
#include <BinFiles/ReadBin.hpp>
#include <cstdlib>
#include <ostream>


VideoProcessor::VideoProcessor(const std::string &videoPath){
  video = cv::VideoCapture(videoPath);
  double fps = video.get(cv::CAP_PROP_FPS);
  delay = (fps > 0) ? static_cast<int>(1000.0 / fps) : 33;

  if(!video.isOpened()){
    throw std::runtime_error("Could not read the video: " + videoPath);
  }
}

void VideoProcessor::displayVideo(const std::string &windowName) {        
  cv::Mat frame_gray;
  while(video.read(frame)){
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::imshow(windowName, frame_gray);
    
    if (cv::waitKey(delay) >= 0) break;
  }
  cv::destroyWindow(windowName);
}

std::vector<cv::Mat> VideoProcessor::getPixelizedVideo(int blockSize) {

  cv::Mat smallFrame, pixelizedFrame;
  std::vector<cv::Mat> pixelizedFrames;

  if(!video.isOpened()) return std::vector<cv::Mat>();

  video.set(cv::CAP_PROP_POS_FRAMES,0);

  while(video.read(frame)){
    //cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::resize(frame, smallFrame, cv::Size(frame.cols / blockSize, frame.rows / blockSize), 0, 0, cv::INTER_LINEAR);
    cv::resize(smallFrame, pixelizedFrame, cv::Size(frame.cols, frame.rows), 0, 0, cv::INTER_NEAREST);
    
    pixelizedFrames.push_back(pixelizedFrame.clone());
  }  
  return pixelizedFrames;
 }

void VideoProcessor::displayPixelizedVideo(const std::string &windowName, int blockSize){
  std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);
  
  if (pixelizedFrames.empty()) return;

  for(const cv::Mat &frame : pixelizedFrames){
    cv::imshow(windowName, frame);
    if(cv::waitKey(delay) == 'q') break;
  } 
}

std::vector<cv::Mat> VideoProcessor::getResizedVideo(int newWidth, double aspectRatio, int blockSize){

  std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);

  if(pixelizedFrames.empty()) return std::vector<cv::Mat>();

  std::vector<cv::Mat> resizedFrames;
  cv::Mat resizedFrame;  

  double proportion = static_cast<int>(pixelizedFrames[0].rows) / static_cast<double>(pixelizedFrames[0].cols);
  int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);

  for(const cv::Mat &frame : pixelizedFrames){
    cv::resize(frame, resizedFrame, cv::Size(newWidth, newHeight));
    resizedFrames.push_back(resizedFrame.clone());
  }

  return resizedFrames; 
}

std::vector<cv::Mat> VideoProcessor::getGrayResizedVideo(int newWidth, double aspectRatio, int blockSize){

  cv::Mat frame_gray;
  std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);

  if(pixelizedFrames.empty()) return std::vector<cv::Mat>();

  std::vector<cv::Mat> resizedFrames;
  cv::Mat resizedFrame;  

  double proportion = static_cast<int>(pixelizedFrames[0].rows) / static_cast<double>(pixelizedFrames[0].cols);
  int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);

  for(const cv::Mat &frame : pixelizedFrames){
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::resize(frame_gray, resizedFrame, cv::Size(newWidth, newHeight));
    resizedFrames.push_back(resizedFrame.clone());
  }

  return resizedFrames; 
}

void VideoProcessor::displayResizedVideo(const std::string &windowName){
  std::vector<cv::Mat> resizedFrames = getResizedVideo(150, 1.8, 6);
  
  if(resizedFrames.empty()) return;   

  for(const cv::Mat &frame : resizedFrames){
    cv::imshow(windowName, frame);
    if(cv::waitKey(delay) == 'q') break;
  }
}

std::string getColorCodeVideo(cv::Vec3b bgr){
  int b, g, r;
  b = bgr[0];
  g = bgr[1];
  r = bgr[2];
  
  std::string code = "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";

  return code;
}


// simula as cores de tons de branco e preto, ja quer grayscale tem apenas 1 canal de cor e não 3
std::string getColorCodeVideo(uchar intensity){
  return "\033[48;2;" + std::to_string(intensity) + ";" + std::to_string(intensity) + ";" + std::to_string(intensity) + "m";
}

std::vector<std::string> VideoProcessor::generateGrayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length(); 
  std::vector<std::string> asciiFrames;

  for(const cv::Mat &frame : video){
    if(frame.empty()) continue;
    
    std::string frameAscii; // Reset frameAscii for each new frame
    
    for(int y = 0; y < frame.rows; y++){      
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(y, x);
        int intensity = frame.at<uchar>(y, x); 
        std::string colorCode = getColorCodeVideo(intensity);
        char asciiChar = asciiChars[intensity * numChars / 256];
        asciiLine += colorCode + asciiChar + color::off; 
      }
      frameAscii += asciiLine + "\n";
    }
    asciiFrames.push_back(frameAscii);
  }
  return asciiFrames;
}

std::vector<std::string> VideoProcessor::generateColoredASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length();
  std::vector<std::string> asciiFrames;
  //std::string frameAscii;


  for(const cv::Mat &frame : video){
    if(frame.empty()) continue;
    
    std::string frameAscii; // Reset frameAscii for each new frame
    
    for(int y = 0; y < frame.rows; y++){      
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(y, x);
        int intensity = (pixelColor[0] + pixelColor[1] + pixelColor[2]) / 3; 
        char asciiChar = asciiChars[intensity * numChars / 256];
        std::string colorCode = getColorCodeVideo(pixelColor);
        asciiLine += colorCode + asciiChar + color::off; 
      }
      frameAscii += asciiLine + "\n";
    }
    asciiFrames.push_back(frameAscii);
  }
  return asciiFrames;
}

void VideoProcessor::displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars, bool isColored = true) const{

  std::vector<std::string> asciiFrames;

  isColored ? asciiFrames = generateColoredASCIIArt(video, asciiChars) : asciiFrames = generateGrayASCIIArt(video, asciiChars);

  WriteBin writer("ascii_video.bin");
  writer.saveASCIIFrames(asciiFrames);
  asciiFrames.clear();

  ReadBin reader("ascii_video.bin");
  std::vector<std::string> loadedFrames = reader.loadASCIIFrames();
  
  bool running = true;
  while(running){
    for (const std::string& frame : loadedFrames) {
      // Clear the screen before displaying each frame
      std::cout << "\033[2J" << "\033[H";
      
      // Display the current frame
      std::cout << frame << std::flush;
      
      // Wait for the specified delay
      std::this_thread::sleep_for(std::chrono::milliseconds(delay)); 
    }
  }
}

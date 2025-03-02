#include "VideoProcessor.hpp"
#include "colors.hpp"
 

VideoProcessor::VideoProcessor(const std::string &videoPath){
  video = cv::VideoCapture(videoPath);
  double fps = video.get(cv::CAP_PROP_FPS);
  delay = (fps > 0) ? static_cast<int>(1000.0 / fps) : 33;
  //delay = 1000;



 //cv::VideoCapture cap("input_video.mp4");
  if(!video.isOpened()){
    throw std::runtime_error("Could not read the video: " + videoPath);
  }
}

void VideoProcessor::displayVideo(const std::string &windowName) {        
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
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::resize(frame_gray, smallFrame, cv::Size(frame_gray.cols / blockSize, frame_gray.rows / blockSize), 0, 0, cv::INTER_LINEAR);
    cv::resize(smallFrame, pixelizedFrame, cv::Size(frame_gray.cols, frame_gray.rows), 0, 0, cv::INTER_NEAREST);
    
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

// diplayPixelized pega de -> std::vector<cv::Mat> pixelizedFrames = getPixelizedVideo(blockSize);

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

// peguei o pixelizedVideo e o resizedVideo, agora falta o dispayResizedvideo

void VideoProcessor::displayResizedVideo(const std::string &windowName){
  std::vector<cv::Mat> resizedFrames = getResizedVideo(150, 1.8, 6);
  
  if(resizedFrames.empty()) return; 

  for(const cv::Mat &frame : resizedFrames){
    cv::imshow(windowName, frame);
    if(cv::waitKey(delay) == 'q') break;
  }
}

void VideoProcessor::displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length();
  //std::string asciiLine;
  
  //cv::startWindowThread();
  //cv::namedWindow("hidden", cv::WINDOW_NORMAL);
  //cv::moveWindow("hidden", -1000, -1000);
  //cv::resizeWindow("hidden", 1, 1); 

  for(const cv::Mat &frame : video){
    if(frame.empty()) continue;
    for(int y = 0; y < frame.rows; y++){
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        int intensity = frame.at<uchar>(y, x);
        std::string colorCode = hey::off; 
        if(intensity < 85){
          colorCode = hey::black;
        } else if(intensity < 170){
          colorCode = hey::grayclaro;
        } else {
          colorCode = hey::white;
        }
        char asciiChar = asciiChars[intensity * numChars / 256];
        asciiLine += colorCode + asciiChar + hey::off;
      }
      std::cout << " " << asciiLine << std::endl;
    }
    std::cout << std::flush;
    // gambiarra, o certo era usar cv::waitKey(delay); mas ele só é iniciado após uma serie de eventos na tela
    // ou seja... pra ele funcionar é preciso iniciar uma tela, mesmo q seja em branco, e eu nao queria isso, pois ia poluir a tela do usuario
    // com coisas sem sentido (tela preta sem nada), então eu adicionei o delay de forma manual com this_thread sleep e chrono passando o delay
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    system("clear");
  }
}

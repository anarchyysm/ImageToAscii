#include "VideoProcessor.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <ostream>
#include <string>

VideoProcessor::VideoProcessor(const std::string &videoPath){
  video = cv::VideoCapture(videoPath);
  double fps = video.get(cv::CAP_PROP_FPS);
  delay = (fps > 0) ? static_cast<int>(1000.0 / fps) : 33;

  if(!video.isOpened()){
    throw std::runtime_error("Could not read the video: " + videoPath);
  }
}

void VideoProcessor::displayVideo(const std::string &windowName) {
  
  while(video.read(frame)){
    cv::imshow(windowName, frame);

    if (cv::waitKey(delay) >= 0) break;
  }
  cv::destroyWindow(windowName);
}

cv::Mat VideoProcessor::PixelizeFrame(cv::Mat& frame, int blockSize) {

  cv::Mat smallFrame, pixelizedFrame;
  cv::resize(frame, smallFrame, cv::Size(frame.cols / blockSize, frame.rows / blockSize), 0, 0, cv::INTER_LINEAR);
  cv::resize(smallFrame, pixelizedFrame, cv::Size(frame.cols, frame.rows), 0, 0, cv::INTER_NEAREST);

  return pixelizedFrame;
}

cv::Mat VideoProcessor::ResizeFrame(cv::Mat& frame, int newWidth, double aspectRatio){

  cv::Mat resizedFrame;
  double proportion = static_cast<int>(frame.rows) / static_cast<double>(frame.cols);
  int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);  
  cv::resize(frame, resizedFrame, cv::Size(newWidth, newHeight));

  return resizedFrame;
}

cv::Mat VideoProcessor::toGrayFrame(cv::Mat& frame){

  cv::Mat grayFrame;
  cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

  return grayFrame;
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

std::string VideoProcessor::generateGrayASCIIArt(const cv::Mat &frame, const std::string &asciiChars) const{
  int numChars = asciiChars.length();
  std::string frameAscii;
  frameAscii.reserve(frame.rows * (frame.cols + 1));
  
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
  //std::cout << std::flush;
  // gambiarra, o certo era usar cv::waitKey(delay); mas ele só é iniciado após uma serie de eventos na tela
  // ou seja... pra ele funcionar é preciso iniciar uma tela, mesmo q seja em branco, e eu nao queria isso, pois ia poluir a tela do usuario
  // com coisas sem sentido (tela preta sem nada), então eu adicionei o delay de forma manual com this_thread sleep e chrono passando o delay
  /*std::this_thread::sleep_for(std::chrono::milliseconds(delay));*/
  /*system("clear");*/
  return frameAscii;
}

std::string VideoProcessor::generateColoredASCIIArt(const cv::Mat &frame, const std::string &asciiChars) const{
  int numChars = asciiChars.length();
  std::string frameAscii;
  frameAscii.reserve(frame.rows * (frame.cols + 1));

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

  return frameAscii;
}

void VideoProcessor::displayASCIIArt(const std::string &asciiChars, int width, double aspectRatio, int blockSize, bool isColored = true){
  
  if(!video.isOpened()){
    std::cerr << "video dont oppened!\n";
    return;
  }

  video.set(cv::CAP_PROP_POS_FRAMES, 0);

  cv::Mat frame;
  while(video.read(frame)){
    frame = PixelizeFrame(frame, blockSize);
    frame = ResizeFrame(frame, width, aspectRatio);
    if(!isColored){
      frame = toGrayFrame(frame); 
    }
    std::string frameAscii =  isColored ? generateColoredASCIIArt(frame, asciiChars) : generateGrayASCIIArt(frame, asciiChars);

    std::cout << "\033[2J\033[1;1H" << frameAscii << std::flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
 
}

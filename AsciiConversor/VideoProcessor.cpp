#include "VideoProcessor.hpp"

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

void VideoProcessor::displayGrayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length(); 

  for(const cv::Mat &frame : video){
    if(frame.empty()) continue;
    for(int y = 0; y < frame.rows; y++){      
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(y, x);
        int intensity = frame.at<uchar>(y, x); 
        std::string colorCode = getColorCodeVideo(intensity);
        char asciiChar = asciiChars[intensity * numChars / 256];
        asciiLine += colorCode + asciiChar + color::off; 
      }
      std::cout << asciiLine << std::endl; 
    }
    std::cout << std::flush;

    // gambiarra, o certo era usar cv::waitKey(delay); mas ele só é iniciado após uma serie de eventos na tela
    // ou seja... pra ele funcionar é preciso iniciar uma tela, mesmo q seja em branco, e eu nao queria isso, pois ia poluir a tela do usuario
    // com coisas sem sentido (tela preta sem nada), então eu adicionei o delay de forma manual com this_thread sleep e chrono passando o delay
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    system("clear");
  }
}

void VideoProcessor::displayColoredASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const{
  int numChars = asciiChars.length(); 

  for(const cv::Mat &frame : video){
    if(frame.empty()) continue;
    for(int y = 0; y < frame.rows; y++){      
      std::string asciiLine;
      for(int x = 0; x < frame.cols; ++x){
        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(y, x);
        int intensity = (pixelColor[0] + pixelColor[1] + pixelColor[2]) / 3; 
        char asciiChar = asciiChars[intensity * numChars / 256];
        std::string colorCode = getColorCodeVideo(pixelColor);
        asciiLine += colorCode + asciiChar + color::off; 
      }
      std::cout << asciiLine << std::endl; 
    }
    std::cout << std::flush;

    // gambiarra, o certo era usar cv::waitKey(delay); mas ele só é iniciado após uma serie de eventos na tela
    // ou seja... pra ele funcionar é preciso iniciar uma tela, mesmo q seja em branco, e eu nao queria isso, pois ia poluir a tela do usuario
    // com coisas sem sentido (tela preta sem nada), então eu adicionei o delay de forma manual com this_thread sleep e chrono passando o delay
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    system("clear");
  }
}

void VideoProcessor::displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars, bool isColored = true) const{

  isColored ? displayColoredASCIIArt(video, asciiChars) : displayGrayASCIIArt(video, asciiChars);

}

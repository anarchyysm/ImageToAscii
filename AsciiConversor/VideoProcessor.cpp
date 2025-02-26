#include "VideoProcessor.hpp"
#include <opencv2/videoio.hpp>
#include <stdexcept>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>
// //#include <stdexcept>
// VideoProcessor::VideoProcessor(const std::string &videoPath){
//   video = cv::imread(videoPath, cv::IMREAD_GRAYSCALE);
//   if(video.empty()){
//     throw std::runtime_error("Could not read the image: " + videoPath);
//   }
// }
//

VideoProcessor::VideoProcessor(const std::string &videoPath){
  //abrir videoPath
  //
  //cv::Mat frame;
  // while (cap.read(frame)) {
  //     // Aqui você pode modificar o frame, se quiser (ex.: redimensionar, aplicar filtro)
  //     // Exemplo: cv::resize(frame, frame, cv::Size(320, 240));
  //
  //     writer.write(frame); // Gravar o quadro no vídeo de saída
  //     cv::imshow("Frame", frame); // Opcional: exibir o quadro
  //     if (cv::waitKey(30) >= 0) break; // Parar com uma tecla
  // }
  // converter pra grayscale cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY)
  // pra adiquirir a taxa de velocidade original do video, usar:
  // int delay = static_cast<int>(1000 / video.get(cv::CAP_PROP_FPS));
  //
  video = cv::VideoCapture(videoPath);
  //cv::VideoCapture cap("input_video.mp4");
  if(!video.isOpened()){
    throw std::runtime_error("Cold not read the video: " + videoPath);
  }
}
//
// void VideoProcessor::displayVideo(const std::string &windowName) const{
//   cv::imshow(windowName, video);
// }
//

void VideoProcessor::displayVideo(const std::string &windowName) {
  
  int delay = static_cast<int>(1000 / video.get(cv::CAP_PROP_FPS));

  while(video.read(frame)){
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::imshow(windowName, frame_gray);
    if (cv::waitKey(delay) >= 0) break;
  }
}

cv::Mat VideoProcessor::getPixelizedImage(int blockSize) const{
  cv::Mat smallImg, pixelizedImg;
  cv::resize(video, smallImg, cv::Size(video.cols / blockSize, video.rows / blockSize), 0, 0, cv::INTER_LINEAR);
  cv::resize(smallImg, pixelizedImg, cv::Size(video.cols, video.rows), 0, 0, cv::INTER_NEAREST);
  return pixelizedImg;
}

cv::Mat VideoProcessor::getResizedImage(int newWidth, double aspectRatio) const{
  double proportion = static_cast<double>(video.rows) / static_cast<double>(video.cols);
  int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);
  cv::Mat resizedImage;
  cv::resize(video, resizedImage, cv::Size(newWidth, newHeight));
  return resizedImage;
}

void VideoProcessor::displayASCIIArt(const cv::Mat &img, const std::string &asciiChars) const{
  int numChars = asciiChars.length();
  for(int y = 0; y < img.rows; y++){
    std::string asciiLine;
    for(int x = 0; x < img.cols; ++x){
      int intensity = img.at<uchar>(y, x);
      char asciiChar = asciiChars[intensity * numChars / 256];
      asciiLine += asciiChar;
    }
    std::cout << " " << asciiLine << std::endl;
  }
}

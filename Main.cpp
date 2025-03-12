#include "AsciiConversor/ImageProcessor.hpp"
#include "AsciiConversor/VideoProcessor.hpp"

int main() {

  std::string videoPath, imagePath;
  int opc;
  bool isColored = true;

  // Entrada de dados
  std::cout << "Image to ASCII\n" << std::endl;
  std::cout << "Options: " << std::endl;
  std::cout << "1. Image " << std::endl;
  std::cout << "2. Video " << std::endl;
  std::cout << "3. Help  \n" << std::endl;
  std::cout << "Option: ";

  // temp
  std::cin >> opc;

  switch (opc) {
  case 1:
    std::cout << "image path (inside cmake-build-debug): ";
    std::cin >> imagePath;

    std::cout << "imagem colorida? 1(y)/0(n)";
    std::cin >> opc;

    if(opc == 0) isColored = false;

    try {
      ImageProcessor processor(imagePath);
      cv::Mat image;
      std::string asciiChars = "  `.'`^,-~:;<>i!lI?+_1|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao23456789*#MW&8%B@";
      if(isColored){
        image = processor.getResizedImage(240, 1.8, 5);
      } else {
        image = processor.getGrayResizedImage(240, 1.8, 5);
      }
      processor.displayASCIIArt(image, asciiChars, isColored);
      cv::waitKey(0);
    } catch (const std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
    }
    break;
  case 2:
    

    std::cout << "video path (inside cmake-build-debug): ";
    std::cin >> videoPath;

    std::cout << "video colorido? 1(y)/0(n)";
    std::cin >> opc;

    if(opc == 0) isColored = false;
    
    try {
      VideoProcessor processor(videoPath);
      std::vector<cv::Mat> video;
      std::string asciiChars = "  ` .'`^,-~:;<>i!lI?+_1|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao23456789*#MW&8%B@";
      if(isColored){
        video = processor.getResizedVideo(320, 1.8, 5);
      } else {
        video = processor.getGrayResizedVideo(320, 1.8, 5);
      }
      processor.displayASCIIArt(video, asciiChars, isColored);
    } catch (const std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
    }
    break;
  case 3:
    std::cout << "Please, doesnt upload long videos, this take so much processing, wait for updates :)" << std::endl;
    std::cout << "The image/video needs to be inside of dir cmake-build-debug/<image path>" << std::endl;
    break;
  }

  return 0;
}

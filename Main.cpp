#include "AsciiConversor/ImageProcessor.hpp"
#include "AsciiConversor/VideoProcessor.hpp"
#include <getopt.h>

int main(int argc, char *argv[]) {

  int width = 320;
  float aspectRatio = 1.8;
  int blockSize = 5;
  bool isColored = true;
  std::string asciiChars = "  `.'`^,-~:;<>i!lI?+_1|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao23456789*#MW&8%B@";
  std::string videoPath = "";
  std::string imagePath = "";

  static struct option long_options[] = {
    {"width", required_argument, 0, 'w'},
    {"aspect-ratio", required_argument, 0, 'a'},
    {"blockSize", required_argument, 0, 'b'},
    {"ascii-chars", required_argument, 0, 'c'},
    {"isColored", no_argument, 0, 'i'},
    {"isNotColored", no_argument, 0, 'n'},
    {"video-path", required_argument, 0, 'v'},
    {"image-path", required_argument, 0, 'p'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  int opt;
  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "w:a:b:c:in:v:p:h", long_options, &option_index)) != -1) {
    switch (opt) {
      case 'w':
        width = std::stoi(optarg);
        break;
      case 'a':
        aspectRatio = std::stof(optarg);
        break;
      case 'b':
        blockSize = std::stoi(optarg);
        break;
      case 'c':
        asciiChars = optarg;
        break;
      case 'i':
        isColored = true;
        break;
      case 'n':
        isColored = false;
        break;
      case 'v':
        videoPath = optarg;
        break;
      case 'p':
        imagePath = optarg;
        break;
      case 'h':
        std::cout << "Usage: " << argv[0] << " [options]\n";
        std::cout << "Options:\n";
        std::cout << "  --width, -w          Set width (default: 320)\n";
        std::cout << "  --aspect-ratio, -a   Set aspect ratio (default: 1.8)\n";
        std::cout << "  --blockSize, -b      Set block size (default: 5)\n";
        std::cout << "  --ascii-chars, -c    Set ASCII characters\n";
        std::cout << "  --isColored, -i      Enable colored output (default: true)\n";
        std::cout << "  --isNotColored, -n   Disable colored output\n";
        std::cout << "  --video-path, -v     Path to video file\n";
        std::cout << "  --image-path, -p     Path to image file\n";
        std::cout << "  --help, -h           Show this help message\n";
        std::cout << "Note: Either --video-path or --image-path is required\n";
        return 0;
      default:
        std::cerr << "Invalid option. Use --help for usage information.\n";
        return 1;
    }
  }

  if (videoPath.empty() && imagePath.empty()) {
    std::cerr << "Error: Please specify either --video-path or --image-path\n";
    std::cerr << "Use --help for usage information\n";
    return 1;
  }

  if (!imagePath.empty()) {
    try {
      ImageProcessor processor(imagePath);
      cv::Mat image;
      if (isColored) {
        image = processor.getResizedImage(width, aspectRatio, blockSize);
      } else {
        image = processor.getGrayResizedImage(width, aspectRatio, blockSize);
      }
      processor.displayASCIIArt(image, asciiChars, isColored);
      cv::waitKey(0);
    } catch (const std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
    }
  }

  else if (!videoPath.empty()) {
    try {
      VideoProcessor processor(videoPath);
      while (1) {
        processor.displayASCIIArt(asciiChars, width, aspectRatio, blockSize, isColored);
      }
    } catch (const std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
    }
  }

  return 0;
}

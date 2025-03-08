#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor(const std::string& imagePath) {
    image = cv::imread(imagePath);
    if (image.empty()) {
        throw std::runtime_error("Could not read the image: " + imagePath);
    }
}

void ImageProcessor::displayImage(const std::string& windowName) const {
    cv::imshow(windowName, image);
}

cv::Mat ImageProcessor::getPixelizedImage(int blockSize) const {
    cv::Mat smallImg, pixelizedImg;
    cv::resize(image, smallImg, cv::Size(image.cols / blockSize, image.rows / blockSize), 0, 0, cv::INTER_LINEAR);
    cv::resize(smallImg, pixelizedImg, cv::Size(image.cols, image.rows), 0, 0, cv::INTER_NEAREST);
    return pixelizedImg;
}

cv::Mat ImageProcessor::getResizedImage(int newWidth, double aspectRatio, int blocksize) const {
    double proportion = static_cast<double>(image.rows) / static_cast<double>(image.cols);
    int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);
    cv::Mat resizedImage;
    cv::Mat pixelizedImg = getPixelizedImage(blocksize);
    cv::resize(pixelizedImg, resizedImage, cv::Size(newWidth, newHeight));
    return resizedImage;
}

cv::Mat ImageProcessor::getGrayResizedImage(int newWidth, double aspectRatio, int blocksize) const { 
    double proportion = static_cast<double>(image.rows) / static_cast<double>(image.cols);
    int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);
    cv::Mat resizedImage, image_gray;
    cv::Mat pixelizedImg = getPixelizedImage(blocksize);
    cv::cvtColor(pixelizedImg, image_gray, cv::COLOR_BGR2GRAY);
    cv::resize(image_gray, resizedImage, cv::Size(newWidth, newHeight));
    return resizedImage;
}

std::string getColorCodeImage(cv::Vec3b bgr){
  int b, g, r;
  b = bgr[0];
  g = bgr[1];
  r = bgr[2];

  std::string code = "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";

  return code;
}

std::string getColorCodeImage(uchar intensity){

  std::string code = "\033[48;2;" + std::to_string(intensity) + ";" + std::to_string(intensity) + ";" + std::to_string(intensity) + "m";
  return code;
}


void ImageProcessor::displayGrayASCIIArt(const cv::Mat& img, const std::string& asciiChars) const {
    int numChars = asciiChars.length();
    for (int y = 0; y < img.rows; ++y) {
        std::string asciiLine;
        for (int x = 0; x < img.cols; ++x){
            int intensity = img.at<uchar>(y, x);
            char asciiChar = asciiChars[intensity * numChars / 256];
            std::string colorCode = getColorCodeImage(intensity);
            asciiLine += colorCode + asciiChar + color::off;
        }
        std::cout << asciiLine << std::endl;
    }
    std::cout << std::flush;
}

void ImageProcessor::displayColoredASCIIArt(const cv::Mat& img, const std::string& asciiChars) const {
    int numChars = asciiChars.length();
    for (int y = 0; y < img.rows; ++y) {
        std::string asciiLine;
        for (int x = 0; x < img.cols; ++x){
            cv::Vec3b pixelColor = img.at<cv::Vec3b>(y, x);
            int intensity = (pixelColor[0] + pixelColor[1] + pixelColor[2]) / 3;
            char asciiChar = asciiChars[intensity * numChars / 256];
            std::string colorCode = getColorCodeImage(pixelColor);
            asciiLine += colorCode + asciiChar + color::off;
        }
        std::cout << asciiLine << std::endl;
    }
    std::cout << std::flush;
}

void ImageProcessor::displayASCIIArt(const cv::Mat& img, const std::string& asciiChars, bool isColored = true) const {

  isColored ? displayColoredASCIIArt(img, asciiChars) : displayGrayASCIIArt(img, asciiChars); 

}


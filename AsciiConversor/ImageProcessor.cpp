#include "ImageProcessor.hpp"
#include <iostream>
//#include <opencv2/imgproc.hpp>

ImageProcessor::ImageProcessor(const std::string& imagePath) {
    image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
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

cv::Mat ImageProcessor::getResizedImage(int newWidth, double aspectRatio) const {
    double proportion = static_cast<double>(image.rows) / static_cast<double>(image.cols);
    int newHeight = static_cast<int>(newWidth * proportion / aspectRatio);
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(newWidth, newHeight));
    return resizedImage;
}

void ImageProcessor::displayASCIIArt(const cv::Mat& img, const std::string& asciiChars) const {
    int numChars = asciiChars.length();
    for (int y = 0; y < img.rows; ++y) {
        std::string asciiLine;
        for (int x = 0; x < img.cols; ++x){
            int intensity = img.at<uchar>(y, x);
            char asciiChar = asciiChars[intensity * numChars / 256];
            asciiLine += asciiChar;
        }
        std::cout << " " << asciiLine << std::endl;
    }
}


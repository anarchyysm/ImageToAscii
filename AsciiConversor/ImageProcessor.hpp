#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <Dependencies.hpp>

class ImageProcessor {
public:
    ImageProcessor(const std::string& imagePath);
    void displayImage(const std::string& windowName) const;
    cv::Mat getGrayResizedImage(int newWidth, double aspectRatio, int blocksize) const; 
    cv::Mat getPixelizedImage(int blockSize) const;
    cv::Mat getResizedImage(int newWidth, double aspectRatio, int blocksize) const;
    void displayASCIIArt(const cv::Mat& img, const std::string& asciiChars, bool isColored) const;
    void displayGrayASCIIArt(const cv::Mat& img, const std::string& asciiChars) const;
    void displayColoredASCIIArt(const cv::Mat& img, const std::string& asciiChars) const;
private:
    cv::Mat image;
};

#endif // IMAGE_PROCESSOR_HPP


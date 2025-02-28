#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <Dependencies.hpp>
#include <string>   

class ImageProcessor {
public:
    ImageProcessor(const std::string& imagePath);
    void displayImage(const std::string& windowName) const;
    cv::Mat getPixelizedImage(int blockSize) const;
    cv::Mat getResizedImage(int newWidth, double aspectRatio) const;
    void displayASCIIArt(const cv::Mat& img, const std::string& asciiChars) const;

private:
    cv::Mat image;
};

#endif // IMAGE_PROCESSOR_HPP


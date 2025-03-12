#ifndef VIDEO_PROCESSOR_HPP
#define VIDEO_PROCESSOR_HPP

#include <Dependencies.hpp>
#include <opencv2/videoio.hpp>

class VideoProcessor {
  public: 
    VideoProcessor(const std::string& imagePath);
    void displayVideo(const std::string &windowName);
    std::vector<cv::Mat> getPixelizedVideo(int blockSize);
    std::vector<cv::Mat> getResizedVideo(int newWidth, double aspectRatio, int blockSize);
    std::vector<cv::Mat> getGrayResizedVideo(int newWidth, double aspectRatio, int blockSize);
    std::vector<std::string> generateGrayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const;
    std::vector<std::string> generateColoredASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const;
    void displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars, bool isColored) const;
    void displayPixelizedVideo(const std::string &windowName, int blockSize);
    void displayResizedVideo(const std::string &windowName);
  private:
    cv::VideoCapture video;
    cv::Mat frame;
    int delay;
};

#endif // VIDEO_PROCESSOR_HPP


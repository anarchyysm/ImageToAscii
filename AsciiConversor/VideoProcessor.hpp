#ifndef VIDEO_PROCESSOR_HPP
#define VIDEO_PROCESSOR_HPP

#include <Dependencies.hpp>
#include <opencv2/videoio.hpp>

class VideoProcessor {
public:
  VideoProcessor(const std::string& videoPath);
  void displayVideo(const std::string &windowName);
  cv::Mat PixelizeFrame(cv::Mat& frame, int blockSize);
  cv::Mat ResizeFrame(cv::Mat& frame, int newWidth, double aspectRatio);
  cv::Mat toGrayFrame(cv::Mat&frame);
  std::string generateGrayASCIIArt(const cv::Mat &frame, const std::string &asciiChars) const;
  std::string generateColoredASCIIArt(const cv::Mat &frame, const std::string &asciiChars) const;
  void displayASCIIArt(const std::string &asciiChars, int width, double aspectRatio, int blockSize, bool isColored);

private:
  cv::VideoCapture video;
  cv::Mat frame;
  int delay;
};

#endif // VIDEO_PROCESSOR_HPP

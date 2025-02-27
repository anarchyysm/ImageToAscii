#ifndef VIDEO_PROCESSOR_HPP
#define VIDEO_PROCESSOR_HPP

#include <Dependencies.hpp>
#include <opencv2/videoio.hpp>

class VideoProcessor {
  public: 
    VideoProcessor(const std::string& imagePath);
    void displayVideo(const std::string &windowName);
    std::vector<cv::Mat> getPixelizedVideo(int blockSize);
    std::vector<cv::Mat> getResizedVideo(int newWidth, double aspectRatio);
    void displayASCIIArt(const std::vector<cv::Mat> &video, const std::string &asciiChars) const;
    void displayPixelizedVideo(const std::string &windowName, int blockSize);
    void displayResizedVideo(const std::string &windowName);

  private:
    cv::VideoCapture video;
    cv::Mat frame, frame_gray;
    int delay;
};

#endif // VIDEO_PROCESSOR_HPP


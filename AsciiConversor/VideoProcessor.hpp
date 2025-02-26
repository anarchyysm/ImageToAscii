#ifndef VIDEO_PROCESSOR_HPP
#define VIDEO_PROCESSOR_HPP

#include <OpenCVDependencies.hpp>
#include <opencv2/videoio.hpp>

class VideoProcessor {
  public: 
    VideoProcessor(const std::string& imagePath);
    void displayVideo(const std::string &windowName);
    cv::Mat getPixelizedImage(int blockSize) const;
    cv::Mat getResizedImage(int newWidth, double aspectRatio) const;
    void displayASCIIArt(const cv::Mat &img, const std::string &asciiChars) const;

  private:
    cv::VideoCapture video;
    cv::Mat frame, frame_gray;
};

#endif // VIDEO_PROCESSOR_HPP


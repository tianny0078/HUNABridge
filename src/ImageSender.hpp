#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cctype> // isdigit
#include <sys/socket.h>
#include <netinet/in.h>

#include <opencv2/core/core.hpp>
struct __attribute__((__packed__)) HNPoint{
  long long _windowsPadding;
  float  _x, _y;
  HNPoint(float x, float y){
    _x = x;
    _y = y;
  }
  HNPoint(){ _x = 0.0f, _y = 0.0f; _windowsPadding = 0.0f;}
};

struct __attribute__((__packed__)) HNDimension{
  long long _windowsPadding;
  HNPoint topLeft, bottomRight;
  void set(float tlx, float tly, float brx, float bry){
      topLeft._x = tlx;
      topLeft._y = tly;
      bottomRight._x = brx;
      bottomRight._y = bry;
  }
  void set(float width, float height){
    set(0, 0, width - 1, height - 1);
  }
  HNDimension() {set (0, 0);_windowsPadding = 0.0f;}
  HNDimension(float width, float height) {set(width, height);}
};

class ImageSender {
 public:
  ImageSender(int port);
  void ConnectToNetwork();
  void ReceiveImageDims();
  void ReceiveImage(cv::Mat& image);
  void WriteImage(cv::Mat& image);
  void SendTestInt();
  void GenerateImage(int rows,int cols, cv::Mat& image);
  bool SendImage(cv::Mat& image);
  int GetWidth();
  int GetHeight();

 private:
  cv::Size2i image_dims_;
  struct sockaddr_in server_addr_;
  struct sockaddr_in client_addr_;
  std::string pic_filename_;
  std::string out_path_;
  socklen_t client_len_;
  size_t server_addr_size_;
  int port_;
  int pic_count_;
  int sock_fdesc_init_;
  int sock_fdesc_conn_;
};

#endif

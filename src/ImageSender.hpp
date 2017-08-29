#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cctype> // isdigit
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <netdb.h>

#include <opencv2/core/core.hpp>
#include "DataWrapper.hpp"

class ImageSender {
 public:
  ImageSender(int port);
  ~ImageSender(){delete wrapper;}
  void ConnectToNetwork();
  void ConnectToNetwork(char * hostname, int port);
  void ReceiveImageDims();
  void ReceiveImage(cv::Mat& image);
  void WriteImage(cv::Mat& image);
  void SendTestInt();
  void GenerateImage(int rows,int cols, cv::Mat& image);
  bool SendImage(cv::Mat& image);
  bool SendDepthImage(cv::Mat& depthImage);
  bool SendColorImage(cv::Mat& colorImage);
  bool SendKinectData(cv::Mat& depthImage, cv::Mat& colorImage);
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
  const char * hostname_;
  int pic_count_;
  int sock_fdesc_init_;
  int sock_fdesc_conn_;
  int socket_fdesc_;
  DataWrapper * wrapper;
};

#endif

#include "ImageSender.hpp"

#include <unistd.h> // close
#include <sys/types.h>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>

ImageSender::ImageSender(int port) :
    image_dims_(cv::Size2i(0, 0)),
    out_path_("D:\\"),
    client_len_(0),
    server_addr_size_(sizeof(server_addr_)),
    port_(port),
    pic_count_(0),
    sock_fdesc_init_(0),
    sock_fdesc_conn_(0) {
  client_len_ = server_addr_size_;
}

void ImageSender::ConnectToNetwork() {

  // Initialize Socket
  sock_fdesc_init_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fdesc_init_ == -1) {
    close(sock_fdesc_init_);
    perror("Couldn't create socket!\n");
    exit(1);
  }

  // Zero out server address struct
  memset((char*)&server_addr_, 0, server_addr_size_);

  // Set server address struct
  server_addr_.sin_family = AF_INET;
  server_addr_.sin_addr.s_addr = INADDR_ANY;
  server_addr_.sin_port = htons(port_);

  // Assign server address to initial socket file descriptor
  if (bind(sock_fdesc_init_, (struct sockaddr*)&server_addr_,
           server_addr_size_) == -1) {
    perror("Couldn't bind initial socket file descriptor!");
    printf("Trying again after killing existing process on port %d...\n",
           port_);
    close(sock_fdesc_init_);
    if (bind(sock_fdesc_init_, (struct sockaddr*)&server_addr_,
             server_addr_size_) == -1) {
      perror("Couldn't bind initial socket file descriptor after retry!");
      exit(1);
    }
    printf("Successful bind to port %d after killing previous process\n",
           port_);
  }

  // Enable listening on initial socket file descriptor
  listen(sock_fdesc_init_, 5);

  // Block process until connection with client has been established.
  // 'client_fdesc' set as new file descriptor to be used for communication
  sock_fdesc_conn_ = accept(sock_fdesc_init_, (struct sockaddr*)&client_addr_,
                            &client_len_);
  if (sock_fdesc_conn_ == -1) {
    perror("ERROR! Client couldn't connect!");
    exit(1);
  }
}

void ImageSender::SendTestInt() {
  unsigned int eventid = 10001;
  long long time = 20170713;
  int content = 2;
  int packageType = 1;
  long long timestep = 77;
  int size1 = sizeof(eventid) + sizeof(time) + sizeof(content)
              + sizeof(packageType) + sizeof(timestep);
 //if(send(sock_fdesc_conn_, &size, sizeof(size), 0) == -1)
   // printf("error sending size \n");

  std::vector<unsigned char> data;
  data.resize(size1);
  auto pdata = &data[0];
  long pos = 0;
  memcpy(pdata+pos, &size1, sizeof(size1));
  pos += sizeof(size1);
  memcpy(pdata+pos, &eventid, sizeof(eventid));
  pos += sizeof(eventid);
  memcpy(pdata+pos, &time, sizeof(time));
  pos += sizeof(time);
  memcpy(pdata+pos, &content, sizeof(content));
  pos += sizeof(content);
  memcpy(pdata+pos, &packageType, sizeof(packageType));
  pos += sizeof(packageType);
  memcpy(pdata+pos, &timestep, sizeof(timestep));
  pos += sizeof(timestep);
  size1 += sizeof(size1);
  if(send(sock_fdesc_conn_,(char *)&data[0], size1, 0) == -1)
     printf("erro sending data\n");
}

void ImageSender::GenerateImage(int rows, int cols, cv::Mat& image){
  int lol = 0;
  image = cv::Mat::zeros(rows, cols, CV_16U);
  for(int row = 0; row < rows; ++row){
    for(int col = 0; col < cols; ++col){
       image.at<unsigned short>(row, col) = 100;
    }
  }
}

bool ImageSender::SendImage(cv::Mat& mat) {
  unsigned int eventid = 10001;
  long long time = 20170713;
  int packageType = 1;
  long long timestep = 77;
  int size1 = sizeof(eventid) + sizeof(time)
              + sizeof(packageType) + sizeof(timestep);
  int rows = mat.rows, cols = mat.cols;
  int imgsize = rows * cols;
  size1 += sizeof(imgsize);
  size1 += sizeof(unsigned short) * imgsize;
  HNDimension sz, asz;
  sz.set((float)cols, (float)rows);
  asz.set((float)cols, (float)rows);
  size1 += sizeof(sz) + sizeof(asz);
  //printf("%d %d \n", sizeof(HNDimension), sizeof(HNPoint));
 //if(send(sock_fdesc_conn_, &size, sizeof(size), 0) == -1)
   // printf("error sending size \n");

  std::vector<unsigned char> data;
  data.resize(size1);
  auto pdata = &data[0];
  long pos = 0;
  // total size
  memcpy(pdata+pos, &size1, sizeof(size1));
  pos += sizeof(size1);
  // eventid + capturedTime
  memcpy(pdata+pos, &eventid, sizeof(eventid));
  pos += sizeof(eventid);
  memcpy(pdata+pos, &time, sizeof(time));
  pos += sizeof(time);
  // image
  memcpy(pdata+pos, &imgsize, sizeof(imgsize));
  pos += sizeof(imgsize);
  memcpy(pdata+pos, mat.data, sizeof(unsigned short) * imgsize);
  pos += sizeof(unsigned short) * imgsize;
  memcpy(pdata+pos, &sz, sizeof(sz));
  pos += sizeof(sz);
  memcpy(pdata+pos, &asz, sizeof(asz));
  pos += sizeof(asz);
  // packageType + timestep
  memcpy(pdata+pos, &packageType, sizeof(packageType));
  pos += sizeof(packageType);
  memcpy(pdata+pos, &timestep, sizeof(timestep));
  pos += sizeof(timestep);
  size1 += sizeof(size1);
  //printf("size %d\n", size1);
  if(send(sock_fdesc_conn_,(char *)&data[0], size1, 0) == -1)
     return false;
  return true;
}

void ImageSender::ReceiveImageDims() {

  ssize_t bytes_sent = 0;
  size_t dims_size = 0;

  int cols = 0;
  int rows = 0;

  size_t sizeof_dims = sizeof(image_dims_.height);

  if (bytes_sent = recv(sock_fdesc_conn_, (char*)&cols, sizeof_dims, 0) == -1) {
    printf("ERROR!: recv failed\n"
           "sock_fdesc: %d\n"
           "image_size: %zu\n"
           "bytes_sent: %zu\n", sock_fdesc_conn_, dims_size, bytes_sent);
    exit(1);
  }
  else {
      printf("Received rows: %d, cols: %d\n", rows, cols);
    }

  if (bytes_sent = recv(sock_fdesc_conn_, (char*)&rows, sizeof_dims, 0) == -1) {
    printf("ERROR!: recv failed\n"
           "sock_fdesc: %d\n"
           "image_size: %zu\n"
           "bytes_sent: %zu\n", sock_fdesc_conn_, dims_size, bytes_sent);
    exit(1);
  }
  image_dims_ = cv::Size2i(cols, rows);
  printf("Image dimensions: [%dx%d]\n", cols, rows);
}

void ImageSender::ReceiveImage(cv::Mat& image) {

  int num_bytes = 0;
  int image_ptr = 0;
  int image_size = 0;

  // Reset image
  //image = cv::Mat::zeros(image_dims_, CV_8UC4);
  image = cv::Mat::zeros(image_dims_, CV_8UC3);

  // Get image size
  image_size = image.total() * image.elemSize();

  // Allocate space for image buffer
  uchar sock_data[image_size];

  // Save image data to buffer
  for (int i = 0; i < image_size; i += num_bytes) {
    num_bytes = recv(sock_fdesc_conn_, sock_data + i, image_size - i, 0);
    if (num_bytes == -1) {
      printf("ERROR!: recv failed\n"
             "i: %d\n"
             "sock_fdesc: %d\n"
             "image_size: %d\n"
             "num_bytes: %d\n", i, sock_fdesc_conn_, image_size, num_bytes);
      exit(1);
    }
  }

  // Write image data to cv::Mat
  for (int i = 0;  i < image_dims_.height; ++i) {
    for (int j = 0; j < image_dims_.width; ++j) {
      /*
      image.at<cv::Vec4b>(i,j) = cv::Vec4b(sock_data[image_ptr+0],
                                           sock_data[image_ptr+1],
                                           sock_data[image_ptr+2],
                                           sock_data[image_ptr+3]);
      image_ptr += 4;
      */
      image.at<cv::Vec3b>(i,j) = cv::Vec3b(sock_data[image_ptr+0],
                                           sock_data[image_ptr+1],
                                           sock_data[image_ptr+2]);
      image_ptr += 3;
    }
  }
  std::ostringstream oss;
  oss << out_path_ << "/pic_" << std::to_string(pic_count_++) << ".jpg";
  pic_filename_ = oss.str();
}

void ImageSender::WriteImage(cv::Mat& image) {
  cv::imwrite(pic_filename_, image);
}

int ImageSender::GetWidth() {
  return image_dims_.width;
}

int ImageSender::GetHeight() {
  return image_dims_.height;
}

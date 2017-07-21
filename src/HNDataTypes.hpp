/*
 * HNDataTypes.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mmlab
 */

#ifndef SRC_HNDATATYPES_HPP_
#define SRC_HNDATATYPES_HPP_


#define HNCAPTUREDKINECTDATA_ID 1900
#define HNCAPTUREDDEPTHIMAGE_ID 1011
#define HNCAPTUREDINDEXIMAGE_ID 1012
#define HNCAPTUREDIMAGE_ID 1010

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

typedef struct tagRGBQUAD{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;


#endif /* SRC_HNDATATYPES_HPP_ */

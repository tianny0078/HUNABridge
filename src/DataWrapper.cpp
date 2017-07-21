/*
 * pdataWrapper.cpp
 *
 *  Created on: Jul 20, 2017
 *      Author: mmlab
 */

#include "DataWrapper.hpp"

int DataWrapper::WrapDepthImage(cv::Mat & depthImage, std::vector<unsigned char>& data){
	int size = GetTailSize() + GetDepthImageSize(depthImage);
	int packetSize = size + sizeof(size);
    data.resize(packetSize);
    auto pdata = &data[0];
    int pos = 0;
    memcpy(pdata+pos, &size, sizeof(size));
    pos += sizeof(size);
    WrapHeader(HNCAPTUREDDEPTHIMAGE_ID, pdata, pos);
    WrapDepthImage(depthImage, pdata, pos);
    WrapTail(1, 1, pdata, pos);
    return packetSize;
}

int DataWrapper::WrapColorImage(cv::Mat & colorImage, std::vector<unsigned char>& data){
	int size = GetTailSize() + GetColorImageSize(colorImage);
	int packetSize = size + sizeof(size);
    data.resize(packetSize);
    auto pdata = &data[0];
    int pos = 0;
    memcpy(pdata+pos, &size, sizeof(size));
    pos += sizeof(size);
    WrapHeader(HNCAPTUREDIMAGE_ID, pdata, pos);
    WrapColorImage(colorImage, pdata, pos);
    WrapTail(1, 1, pdata, pos);
    return packetSize;
}

int DataWrapper::WrapDepthImage(cv::Mat & depthImage, unsigned char * pdata, int &pos){
	int imgsize = depthImage.rows * depthImage.cols;
	HNDimension sz, asz;
	sz.set((float)depthImage.cols, (float)depthImage.rows);
	asz.set((float)depthImage.cols, (float)depthImage.rows);
	// image
	memcpy(pdata+pos, &imgsize, sizeof(imgsize));
	pos += sizeof(imgsize);
	memcpy(pdata+pos, depthImage.data, sizeof(unsigned short) * imgsize);
	pos += sizeof(unsigned short) * imgsize;
	memcpy(pdata+pos, &sz, sizeof(sz));
	pos += sizeof(sz);
	memcpy(pdata+pos, &asz, sizeof(asz));
	pos += sizeof(asz);
	return 0;
}

int DataWrapper::WrapColorImage(cv::Mat & colorImage, unsigned char * pdata, int &pos){
	int imgsize = colorImage.rows * colorImage.cols;
	HNDimension sz, asz;
	sz.set((float)colorImage.cols, (float)colorImage.rows);
	asz.set((float)colorImage.cols, (float)colorImage.rows);
	// image
	memcpy(pdata+pos, &imgsize, sizeof(imgsize));
	pos += sizeof(imgsize);
	memcpy(pdata+pos, colorImage.data, sizeof(RGBQUAD) * imgsize);
	pos += sizeof(RGBQUAD) * imgsize;
	memcpy(pdata+pos, &sz, sizeof(sz));
	pos += sizeof(sz);
	memcpy(pdata+pos, &asz, sizeof(asz));
	pos += sizeof(asz);
	return 0;

}

int DataWrapper::WrapKinectData(cv::Mat & detphImage, cv::Mat & colorImage, unsigned char * data, int &pos){
    return 0;
}


int DataWrapper::WrapHeader(unsigned int eventid, unsigned char * pdata, int & pos){
	long long time = 20170713;
	// wrap eventid and capturedTime
	memcpy(pdata+pos, &eventid, sizeof(eventid));
	pos += sizeof(eventid);
	memcpy(pdata+pos, &time, sizeof(time));
	pos += sizeof(time);
    return pos;
}

int DataWrapper::WrapIndexImage(cv::Mat & indexImage, unsigned char * pdata, int &pos){
    return 0;
}

int DataWrapper::WrapTail(int packageType, long long timestep,
		unsigned char * pdata, int& pos){
	  memcpy(pdata+pos, &packageType, sizeof(packageType));
	  pos += sizeof(packageType);
	  memcpy(pdata+pos, &timestep, sizeof(timestep));
	  pos += sizeof(timestep);
	  return pos;
}

int DataWrapper::GetHeaderSize(){
	return sizeof(unsigned int) + sizeof(long long);
}
int DataWrapper::GetDepthImageSize(cv::Mat & depthImage){
	// including depth image size and header size
	int imgsize = depthImage.rows * depthImage.cols;
	return sizeof(imgsize) + imgsize * sizeof(unsigned short) + sizeof(HNDimension) * 2 + GetHeaderSize();
}
int DataWrapper::GetColorImageSize(cv::Mat & colorImage){
	int imgsize = colorImage.rows * colorImage.cols;
	return sizeof(imgsize) + imgsize * sizeof(RGBQUAD) + sizeof(HNDimension) * 2 + GetHeaderSize();
}
int DataWrapper::GetIndexImageSize(){
    return 0;
}
int DataWrapper::GetKinetDataSize(){
    return 0;
}
int DataWrapper::GetTailSize(){
	return sizeof(int) + sizeof(long long);
}


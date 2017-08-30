/*
 * pdataWrapper.cpp
 *
 *  Created on: Jul 20, 2017
 *      Author: mmlab
 */

#include "DataWrapper.hpp"
//public
void DataWrapper::UnwrapXYZWData(unsigned char * pdata, float &x, float &y, float &z, float& w){
	// parse the saved data into (x, y, z, w).
	long long time;
	unsigned int eventid;
	int pos = 0;
	// wrap eventid and capturedTime
	memcpy(&eventid, pdata+pos, sizeof(eventid));
	pos += sizeof(eventid);
	memcpy(&time, pdata+pos, sizeof(time));
	pos += sizeof(time);
	HNPointXYZW xyzw;
	memcpy(&xyzw, pdata+pos, sizeof(xyzw));
	pos += sizeof(xyzw);
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
	w = xyzw.w;
	int packageType;
	long long timestep;
	memcpy(&packageType, pdata+pos, sizeof(packageType));
	pos += sizeof(packageType);
	memcpy(&timestep, pdata+pos, sizeof(timestep));
	pos += sizeof(timestep);
}


int DataWrapper::WrapDepthImage(cv::Mat & depthImage, std::vector<unsigned char>& data){
	int size = GetTailSize() + GetDepthImageSize(depthImage);
	int packetSize = size + sizeof(size);
    data.resize(packetSize);
    auto pdata = &data[0];
    int pos = 0;
    memcpy(pdata+pos, &size, sizeof(size));
    pos += sizeof(size);
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
    WrapColorImage(colorImage, pdata, pos);
    WrapTail(1, 1, pdata, pos);
    return packetSize;
}

int DataWrapper::WrapKinectData(cv::Mat & depthImage, cv::Mat & colorImage, std::vector<unsigned char>& data){
    int size = GetTailSize() + GetKinectDataSize(depthImage, colorImage);
    int packetSize = size + sizeof(size);
	data.resize(packetSize);
	auto pdata = &data[0];
	int pos = 0;
	memcpy(pdata+pos, &size, sizeof(size));
	pos += sizeof(size);
	WrapKinectData(depthImage, colorImage, pdata, pos);
	WrapTail(1, 1, pdata, pos);
	return packetSize;
}

////////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////////
int DataWrapper::WrapDepthImage(cv::Mat & depthImage, unsigned char * pdata, int &pos){
	int imgsize = depthImage.rows * depthImage.cols;
	HNDimension sz, asz;
	sz.set((float)depthImage.cols, (float)depthImage.rows);
	asz.set((float)depthImage.cols, (float)depthImage.rows);
	//header
    WrapHeader(HNCAPTUREDDEPTHIMAGE_ID, pdata, pos);
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
	// header
    WrapHeader(HNCAPTUREDIMAGE_ID, pdata, pos);
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

int DataWrapper::WrapIndexImage(cv::Mat & indexImage, unsigned char * pdata, int &pos){
	int imgsize = indexImage.rows * indexImage.cols;
	HNDimension sz, asz;
	sz.set((float)indexImage.cols, (float)indexImage.rows);
	asz.set((float)indexImage.cols, (float)indexImage.rows);
	// header
    WrapHeader(HNCAPTUREDINDEXIMAGE_ID, pdata, pos);
	// image
	memcpy(pdata+pos, &imgsize, sizeof(imgsize));
	pos += sizeof(imgsize);
	memcpy(pdata+pos, indexImage.data, sizeof(unsigned char) * imgsize);
	pos += sizeof(unsigned char) * imgsize;
	memcpy(pdata+pos, &sz, sizeof(sz));
	pos += sizeof(sz);
	memcpy(pdata+pos, &asz, sizeof(asz));
	pos += sizeof(asz);
	return 0;
}

int DataWrapper::WrapKinectData(cv::Mat & depthImage, cv::Mat & colorImage, unsigned char * pdata, int &pos){
	// header
	WrapHeader(HNCAPTUREDKINECTDATA_ID, pdata, pos);
	// images
    cv::Mat fakeDepthImage = cv::Mat::zeros(cv::Size2i(1, 1), CV_16U);
    WrapDepthImage(fakeDepthImage, pdata, pos);
	WrapDepthImage(depthImage, pdata, pos);
    cv::Mat fakeIndexImage = cv::Mat::zeros(cv::Size2i(1, 1), CV_8U);
    WrapIndexImage(fakeIndexImage, pdata, pos);
	WrapColorImage(colorImage, pdata, pos);
	unsigned char activeSkeleton = 2;
    memcpy(pdata+pos, &activeSkeleton, sizeof(activeSkeleton));
    pos += sizeof(activeSkeleton);
    unsigned int skeletonSize = 0;
    memcpy(pdata+pos, &skeletonSize, sizeof(skeletonSize));
    pos += sizeof(skeletonSize);
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
	return sizeof(imgsize) + imgsize * sizeof(unsigned short) + sizeof(HNDimension) * 2  + GetHeaderSize();
}
int DataWrapper::GetColorImageSize(cv::Mat & colorImage){
	int imgsize = colorImage.rows * colorImage.cols;
	return sizeof(imgsize) + imgsize * sizeof(RGBQUAD) + sizeof(HNDimension) * 2 + GetHeaderSize();
}
int DataWrapper::GetIndexImageSize(cv::Mat & indexImage){
	int imgsize = indexImage.rows * indexImage.cols;
    return sizeof(imgsize) + imgsize * sizeof(unsigned char) + sizeof(HNDimension) * 2 + GetHeaderSize();
}
int DataWrapper::GetKinectDataSize(cv::Mat & depthImage, cv::Mat & colorImage){
	cv::Mat fakeDepthImage = cv::Mat::zeros(cv::Size2i(1, 1), CV_16U);
	cv::Mat fakeIndexImage = cv::Mat::zeros(cv::Size2i(1, 1), CV_8U);
	unsigned char activeSkeleton = 'x';
	unsigned int skeletonSize = 0;
    return GetDepthImageSize(depthImage) + GetDepthImageSize(fakeDepthImage) + GetColorImageSize(colorImage)
    		+ GetIndexImageSize(fakeIndexImage) + sizeof(activeSkeleton) + sizeof(skeletonSize) + GetHeaderSize();
}
int DataWrapper::GetTailSize(){
	return sizeof(int) + sizeof(long long);
}


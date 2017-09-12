/*
 * DataWrapper.h
 *  DataWrapper is a class to wrap up data including depth image,
 *  color image, or combination of depth and color image
 *  Created on: Jul 20, 2017
 *      Author: Yuan Tian
 */

#ifndef SRC_DATAWRAPPER_HPP_
#define SRC_DATAWRAPPER_HPP_

#include <opencv2/core/core.hpp>
#include <string>
#include <vector>
#include "HNDataTypes.hpp"
#include <sys/time.h>


class DataWrapper{
public:
	DataWrapper(){}
    int WrapDepthImage(cv::Mat& depthImage, std::vector<unsigned char>& data);
    int WrapColorImage(cv::Mat& colorImage, std::vector<unsigned char>& data);
    int WrapKinectData(cv::Mat & depthImage, cv::Mat & colorImage, std::vector<unsigned char>& data);
    void UnwrapXYZWData(unsigned char * data, float &x, float &y, float &z, float& w);
private:
    int WrapDepthImage(cv::Mat & depthImage, unsigned char * data, int &pos);
    int WrapColorImage(cv::Mat & colorImage, unsigned char * data, int &pos);
    int WrapKinectData(cv::Mat & depthImage, cv::Mat & colorImage, unsigned char * data, int &pos);
    int WrapHeader(unsigned int eventid, unsigned char * data, int &pos);
    int WrapIndexImage(cv::Mat & indexImage, unsigned char * data, int &pos);
    int WrapTail(int packageType, long long timestep, unsigned char * data, int &pos);
    int GetHeaderSize();
    int GetDepthImageSize(cv::Mat & depthImage);
    int GetColorImageSize(cv::Mat & colorImage);
    int GetIndexImageSize(cv::Mat & indexImage);
    int GetKinectDataSize(cv::Mat & depthImage, cv::Mat & colorImage);
    int GetTailSize();
    long long GetTimeStep();
};



#endif /* SRC_DATAWRAPPER_HPP_ */

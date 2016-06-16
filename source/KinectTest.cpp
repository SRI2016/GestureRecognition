#include "KinectMotion.h"
#include <opencv\cv.h>
#include <vector>

//int * KinectMotion::palmCenter2(cv::Mat image) {
//
	//double current_distance;
	//double current_min;
	//double maxMin = 0;
	//Point maxMin_p;
	//Point edge_p;
	//for (int i = xMin; i < xMax; ++i)
	//{
	//	for (int j = yMin; j < yMax; ++j)
	//	{
	//		current_min = 10000;
	//		for (int k = 0; k < edges.size(); ++k)
	//		{
	//			int x = (edges.at(k).i - i);
	//			int y = (edges.at(k).j - j);
	//			current_distance = sqrt((x*x) + (y*y));
	//			if (current_distance < current_min)
	//			{
	//				current_min = current_distance;
	//				edge_p.i = edges.at(k).i;
	//				edge_p.j = edges.at(k).j;
	//			}
	//		}
	//		if (current_min > maxMin)
	//		{
	//			maxMin = current_min;
	//			maxMin_p.i = i;
	//			maxMin_p.j = j;
	//		}
	//	}
	//}
	//
	//image = makeEdgeImage(image);
	//image.at<cv::Vec3b>(xMin_p.i, xMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(yMin_p.i, yMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(xMax_p.i, xMax_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(yMax_p.i, yMax_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(maxMin_p.i, maxMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(edge_p.i, edge_p.j) = cv::Vec3b(255, 255, 255);
//
//}

//void KinectMotion::blob(cv::Mat imMat) {
//cv::SimpleBlobDetector::Params params;
//params.minThreshold = 1;
//params.maxThreshold = 25;
//params.filterByArea = false;
//params.filterByCircularity = false;
//params.filterByConvexity = false;
//params.filterByInertia = false;
//cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
//std::vector<cv::KeyPoint> keypoints;
//detector->detect(imMat, keypoints);
//cv::Mat iDepthMat_with_keypoints;
//drawKeypoints(imMat, keypoints, iDepthMat_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//imshow("keypoints", iDepthMat_with_keypoints);
//cv::waitKey(0);
//}
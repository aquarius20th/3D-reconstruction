// PCLICPTest.cpp : 定义控制台应用程序的入口点。
//

// pointviewer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
#include"stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include"cv.h"
#include<Eigen\Dense>
#include <opencv2/opencv.hpp>  //头文件
using namespace cv;  //包含cv命名空间
using namespace std;
using namespace Eigen;

int
main(int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>);

	// Fill in the CloudIn data
	//cloud_in->width = 339310;
	cloud_in->width = 40000000;
	cloud_in->height = 1;
	cloud_in->is_dense = false;
	cloud_in->points.resize(cloud_in->width * cloud_in->height);

	cloud_out->width = 339310;
	cloud_out->height = 1;
	cloud_out->is_dense = false;
	cloud_out->points.resize(cloud_in->width * cloud_in->height);

	int totalsize;
	char str[30];
	int k = 0;
	for (int z = 1; z <=400; z++)
	{
		printf("z=%d ", z);
		sprintf(str, "E:\\v1_400\\%d.tif", z);
		IplImage *src = cvLoadImage(str, 0);
		int width = src->width;
		int height = src->height;
		for (int rows = 0; rows < height; rows++)
		{
			uchar *ptr = (uchar*)src->imageData + rows*src->width;
			for (int cols = 0; cols < width; cols++)
			{
				int intensity = ptr[cols];

				if (intensity > 5)
				{
					cloud_in->points[k].x = cols;
					cloud_in->points[k].y = rows;
					cloud_in->points[k].z = z;
					k++;
					printf("i=%d x=%d y=%d z=%d\n", k , cols, rows, z);
				}

			}
		}
		totalsize = k;

	}
	printf("k=%d\n", k);
	k = 0;

	Matrix3d p,p1;
	RowVector3d m, mul;
	p << 0.999384, 0.0346903, 0.00565249,
		-0.0347993, 0.999183, 0.0206506,
		-0.00493163, -0.0208353, 0.999773;
	p1 << 0.999967, 0.00114429, -0.00837694,
		-0.00114611, 1, -0.000169035,
		0.00837672, 0.000178473, 0.999966;


	for (int z = 1; z <=400; z++)
	{
		printf("z=%d ", z);
		sprintf(str, "E:\\v3_flip_400\\%d.tif", z);
		IplImage *src = cvLoadImage(str, 0);
		int width = src->width;
		int height = src->height;
		for (int rows = 0; rows < height; rows++)
		{
			uchar *ptr = (uchar*)src->imageData + rows*src->width;
			for (int cols = 0; cols < width; cols++)
			{
				int intensity = ptr[cols];

				if (intensity > 5)
				{
					/*m[0] = 401-z;
					m[1] = rows;
					m[2] = cols;
					mul = m*p;*/
					cloud_out->points[k].x = cols;
					cloud_out->points[k].y = rows;
					cloud_out->points[k].z = 401-z;
					k++;
					printf("i=%d x=%d y=%d z=%d\n", k, cols, rows, z);
				}

			}
		}
		totalsize = k;
	}
	std::cout << "Saved " << totalsize << " data points to input:"
		<< std::endl;

	/**cloud_out = *cloud_in;
	std::cout << "size:" << cloud_out->points.size() << std::endl;
	for (size_t i = 0; i < cloud_in->points.size(); ++i)
		cloud_out->points[i].x = cloud_in->points[i].x + 0.7f;
	std::cout << "Transformed " << cloud_in->points.size() << " data points:"
		<< std::endl;
	for (size_t i = 0; i < cloud_out->points.size(); ++i)
		std::cout << "    " << cloud_out->points[i].x << " " <<
		cloud_out->points[i].y << " " << cloud_out->points[i].z << std::endl;*/
	pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
	icp.setInputSource(cloud_in);
	icp.setInputTarget(cloud_out);
	pcl::PointCloud<pcl::PointXYZ> Final;
	icp.align(Final);
	std::cout << "has converged:" << icp.hasConverged() << " score: " <<
		icp.getFitnessScore() << std::endl;
	std::cout << icp.getFinalTransformation() << std::endl;
	system("pause");
		return (0);
}

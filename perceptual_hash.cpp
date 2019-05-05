#include <iostream>
#include <Windows.h>
#include <dirent.h>
#include <sys/stat.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

using namespace std;
using namespace cv;

#define N 8

//使用感知哈希算法进行图片去重
//MatStruct：存储对应名称和要比较的内容（N*N的灰度图）
typedef struct MatStruct {
	string name;
	unsigned char buf[N*N];
	MatStruct() {
		name = "";
		memset(buf, 0, N*N);
	}
	MatStruct(const struct MatStruct &ms) {
		name = ms.name;
		memset(buf, 0, N*N);
		memcpy(buf, ms.buf, N*N);
	}
	//重载比较函数
	bool operator<(const struct MatStruct &ms)const {
		return name < ms.name;
	}
}MatStruct;

//获取该目录下所有文件的图像指纹存入images
void GetDirImages(const string path, std::vector<MatStruct> &images) {
	cout << "\nGet dir------------" << endl;
	std::vector<MatStruct> tmpimages;
	DIR *dir;
	struct dirent *ptr;
	//打开文件目录且遍历
	dir = opendir(path.c_str());
	while ((ptr = readdir(dir)) != NULL) {
		if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)) {
			continue;
		}
		
		string fullname = path + "/" + ptr->d_name;
		Mat img = imread(fullname, 0);//参数2设为0，打开为灰度图
		if (img.empty()) {
			continue;
		}
		MatStruct ms;
		ms.name = ptr->d_name;
		Mat gray, res;
		//缩放成32x32大小
		resize(img, res, Size(32, 32));
		//dct必须先转换成CV_32F
		res.convertTo(res, CV_32F);

		Mat srcDCT;
		//核心！离散余弦变换
		dct(res, srcDCT);
		srcDCT = abs(srcDCT);

		//取左上角的N*N
		double sum = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				sum += srcDCT.at<float>(i, j);

		double average = sum / (N*N);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				ms.buf[i * N + j] = srcDCT.at<float>(i, j) > average ? 1 : 0;

		cout << "get " << fullname << endl;
		tmpimages.push_back(ms);
	}
	closedir(dir);
	std::sort(tmpimages.begin(), tmpimages.end());
	tmpimages.swap(images);
}


//删除图像
void DeleteImage(const string path, std::vector<std::vector<MatStruct>::iterator> &delImages, std::vector<MatStruct> &images) {
	//必须从后往前删，从前面开始的话会导致delImages[i+1]以及之后存储的迭代器失效
	for (int i = delImages.size() - 1; i >= 0; i--) {
		cout << "Remove " << delImages[i]->name << endl;
		string fullpath = path + "/" + delImages[i]->name;
		unlink(fullpath.c_str());
		images.erase(delImages[i]);
	}
}

//比较两个图片的相似度
bool IsTheSame(std::vector<MatStruct>::iterator x, std::vector<MatStruct>::iterator y) {
	int diff = 0;
	if (x != y) {
		//计算相似度(这里的计算方法也称hammingDistance汉明距离)，不同的地方不超过5（阈值可以自己确定），则为相似的图片
		for (int i = 0; i < N*N; i++) {
			if (x->buf[i] != y->buf[i]) {
				diff++;
			}
		}
		return diff <= 3;
	}
	return false;
}

void CompareImage(const string path, std::vector<MatStruct> &images) {
	cout << "\nStart compare-----------" << endl;
	std::vector<MatStruct>::iterator it1 = images.begin();
	while (it1 != images.end()) {
		std::vector<MatStruct>::iterator it2 = it1;
		//存储需要删除的迭代对象
		std::vector<std::vector<MatStruct>::iterator> delImages;
		cout << it1->name << endl;
		while (it2 != images.end()) {
			if (IsTheSame(it2, it1)) {
				delImages.push_back(it2);
			}
			it2++;
		}
		if (delImages.size() > 0) {
			DeleteImage(path, delImages, images);
		}
		it1++;
	}
}



void PerceptualHash(const string path) {
	std::vector<MatStruct> images;
	GetDirImages(path, images);
	CompareImage(path, images);
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		cout << "Usage: " << argv[0] << " picturePath" << endl;
		return -1;
	}

	struct stat st;
	//必须有该路径且是目录
	if (stat(argv[1], &st) != 0 || (st.st_mode & S_IFDIR) != S_IFDIR) {
		cout << "Error! " << argv[1] << " is not dir" << endl;
		return -1;
	}
	PerceptualHash(argv[1]);
	return 0;
}

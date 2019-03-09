#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <UrlMon.h>
#pragma comment(lib, "urlmon.lib")
#include <regex>
#include "tinyxml2.h"


//改背景图
bool changeBgPicture(std::string s) {
	return SystemParametersInfoA(
		SPI_SETDESKWALLPAPER,
		0,
		(PVOID)s.c_str(),
		1);
}

//下载为文件
bool downloadToFile(std::string url, std::string local) {
	bool flag = URLDownloadToFileA(
		NULL,
		url.c_str(),
		local.c_str(),
		0,
		NULL);
	if (flag == S_OK) {
		std::cout << "成功下载文件在 " << local << std::endl;
	} else {
		std::cout << "下载文件到 " << local << " 失败" << std::endl;
	}
	return flag;
}//"https:\\\\cn.bing.com\\sa\\simg\\hpb\\NorthMale_EN-US8782628354_1920x1080.jpg"

//通过接口获取今日图片的url，xml的解析借助了开源库tinyxml2
std::string getPicTureXmlAndUrl(std::string path) {
	std::string url = "http://cn.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1";
	std::string local = path + "\\today.xml";

	downloadToFile(url, local);

	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(local.c_str()))
		std::cout << "读取Xml文件异常！" << std::endl;
	tinyxml2::XMLElement *images = doc.RootElement();
	tinyxml2::XMLElement *image = images->FirstChildElement("image");

	//图片Url
	std::string WebPicturedomain = "http://cn.bing.com";
	std::string WebPictureUrl = "";

	if (image != NULL)
		WebPictureUrl = image->FirstChildElement("url")->GetText();

	std::string WebPictureFullpath = WebPicturedomain + WebPictureUrl;
	std::cout << "Xml解析成功！Url为：" << std::endl;
	std::cout << WebPictureFullpath << std::endl;

	DeleteFile(local.c_str());//删掉xml

	return WebPictureFullpath;
}

//获取年月日
std::string getYearMonthDay() {
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);

	std::string Year = std::to_string(t_tm->tm_year + 1900);
	std::string Month = std::to_string(t_tm->tm_mon + 1);
	std::string Day = std::to_string(t_tm->tm_mday);
	std::string PictureName = Year + "-" + Month + "-" + Day;

	return PictureName;
}

//创文件夹
void mkdirIfNotExist(std::string path) {
	if (CreateDirectory(path.c_str(), NULL)) {
		std::cout << "CreateDirectory E:\\bing-bg-pictures\n" << std::endl;
	} else {
		std::cout << "E:\\bing-bg-pictures have been exist\n" << std::endl;
	}
}

//有的时候解析到的图片url里不是1920x1080，故替换
void changeTo1920x1080(std::string &str) {
	std::regex pattern("_(\\d+)x(\\d+).jpg");
	str = std::regex_replace(str, pattern, "_1920x1080.jpg");
}

int main() {
	std::string path = "E:\\bing-bg-pictures";
	std::string localUrl = path + "\\" + getYearMonthDay() + ".jpg";
	mkdirIfNotExist(path);
	std::string url = getPicTureXmlAndUrl(path);
	changeTo1920x1080(url);
	downloadToFile(url, localUrl);
	if (changeBgPicture(localUrl)) {
		std::cout << "\n更换壁纸成功" << std::endl;
	}

	return 0;
}
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <UrlMon.h>
#pragma comment(lib, "urlmon.lib")
#include <regex>
#include "tinyxml2.h"


//�ı���ͼ
bool changeBgPicture(std::string s) {
	return SystemParametersInfoA(
		SPI_SETDESKWALLPAPER,
		0,
		(PVOID)s.c_str(),
		1);
}

//����Ϊ�ļ�
bool downloadToFile(std::string url, std::string local) {
	bool flag = URLDownloadToFileA(
		NULL,
		url.c_str(),
		local.c_str(),
		0,
		NULL);
	if (flag == S_OK) {
		std::cout << "�ɹ������ļ��� " << local << std::endl;
	} else {
		std::cout << "�����ļ��� " << local << " ʧ��" << std::endl;
	}
	return flag;
}//"https:\\\\cn.bing.com\\sa\\simg\\hpb\\NorthMale_EN-US8782628354_1920x1080.jpg"

//ͨ���ӿڻ�ȡ����ͼƬ��url��xml�Ľ��������˿�Դ��tinyxml2
std::string getPicTureXmlAndUrl(std::string path) {
	std::string url = "http://cn.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1";
	std::string local = path + "\\today.xml";

	downloadToFile(url, local);

	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(local.c_str()))
		std::cout << "��ȡXml�ļ��쳣��" << std::endl;
	tinyxml2::XMLElement *images = doc.RootElement();
	tinyxml2::XMLElement *image = images->FirstChildElement("image");

	//ͼƬUrl
	std::string WebPicturedomain = "http://cn.bing.com";
	std::string WebPictureUrl = "";

	if (image != NULL)
		WebPictureUrl = image->FirstChildElement("url")->GetText();

	std::string WebPictureFullpath = WebPicturedomain + WebPictureUrl;
	std::cout << "Xml�����ɹ���UrlΪ��" << std::endl;
	std::cout << WebPictureFullpath << std::endl;

	DeleteFile(local.c_str());//ɾ��xml

	return WebPictureFullpath;
}

//��ȡ������
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

//���ļ���
void mkdirIfNotExist(std::string path) {
	if (CreateDirectory(path.c_str(), NULL)) {
		std::cout << "CreateDirectory E:\\bing-bg-pictures\n" << std::endl;
	} else {
		std::cout << "E:\\bing-bg-pictures have been exist\n" << std::endl;
	}
}

//�е�ʱ���������ͼƬurl�ﲻ��1920x1080�����滻
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
		std::cout << "\n������ֽ�ɹ�" << std::endl;
	}

	return 0;
}
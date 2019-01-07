﻿// ConsoleExample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\types_c.h>
#include <fstream>
#include <cassert>
#include <string>
#include "md5.h"
#include "LinkList.h"
#include "DuLinkList.h"
#include "SqList.h"

using namespace cv;
using namespace std;

bool getUrl(char *filename)
{
	CURL *curl;
	CURLcode res;
	FILE *fp;
	if ((fp = fopen(filename, "w")) == NULL)  // 返回结果用文件存储
		return false;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();    // 初始化
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
		res = curl_easy_perform(curl);   // 执行
		if (res != 0) {

			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}
		fclose(fp);
		return true;
	}
}

bool curlGet(char *url)
{
	CURL *curl;
	CURLcode res;
	FILE *fp;
	if ((fp = fopen("get.txt", "w")) == NULL)  // 返回结果用文件存储
		return false;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();    // 初始化
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
		//curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
		res = curl_easy_perform(curl);   // 执行
		if (res != 0) {

			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}
		fclose(fp);
		return true;
	}
}
bool postUrl(char *filename)
{
	CURL *curl;
	CURLcode res;
	FILE *fp;
	if ((fp = fopen(filename, "w")) == NULL)
		return false;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86");    // 指定post内容
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
		curl_easy_setopt(curl, CURLOPT_URL, " http://mail.sina.com.cn/cgi-bin/login.cgi ");   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	fclose(fp);
	return true;
}

bool curlPost(char *url, char *params)
{
	CURL *curl;
	CURLcode res;
	FILE *fp;
	if ((fp = fopen("post.txt", "w")) == NULL)
		return false;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt"); // 指定cookie文件
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);    // 指定post内容
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
		curl_easy_setopt(curl, CURLOPT_URL, url);   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	fclose(fp);
	return true;
}

void readTxt(string file)
{
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string s;
	while (getline(infile, s))
	{
		cout << s << endl;
	}
	infile.close();             //关闭文件输入流 
}

int kmeans()
{
	const int MAX_CLUSTERS = 5;
	Scalar colorTab[] =
	{
		Scalar(0, 0, 255),
		Scalar(0,255,0),
		Scalar(255,100,100),
		Scalar(255,0,255),
		Scalar(0,255,255)
	};

	Mat img(500, 500, CV_8UC3);
	RNG rng(12345);

	for (;;)
	{
		int k, clusterCount = rng.uniform(2, MAX_CLUSTERS + 1);
		int i, sampleCount = rng.uniform(1, 1001);
		Mat points(sampleCount, 1, CV_32FC2), labels;

		clusterCount = MIN(clusterCount, sampleCount);
		std::vector<Point2f> centers;

		/* generate random sample from multigaussian distribution */
		for (k = 0; k < clusterCount; k++)
		{
			Point center;
			center.x = rng.uniform(0, img.cols);
			center.y = rng.uniform(0, img.rows);
			Mat pointChunk = points.rowRange(k*sampleCount / clusterCount,
				k == clusterCount - 1 ? sampleCount :
				(k + 1)*sampleCount / clusterCount);
			rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
		}

		randShuffle(points, 1, &rng);

		double compactness = kmeans(points, clusterCount, labels,
			TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);

		img = Scalar::all(0);

		for (i = 0; i < sampleCount; i++)
		{
			int clusterIdx = labels.at<int>(i);
			Point ipt = points.at<Point2f>(i);
			circle(img, ipt, 2, colorTab[clusterIdx], FILLED, LINE_AA);
		}
		for (i = 0; i < (int)centers.size(); ++i)
		{
			Point2f c = centers[i];
			circle(img, c, 40, colorTab[i], 1, LINE_AA);
		}
		cout << "Compactness: " << compactness << endl;

		imshow("clusters", img);

		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
			break;
	}

	return 0;
}

char* string2char(string str) {
	int nStrLength = str.length();
	char chStr[255];
	int i;
	for (i = 0; i < nStrLength; i++)
		chStr[i] = str[i];
	chStr[i] = '\0';
	return chStr;
}

int verifyCode(string sImgSrc) {
	//Process image to extract contour
	Mat thr, gray, con;
	Mat src = imread(sImgSrc, 1);
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, thr, 200, 255, THRESH_BINARY_INV); //Threshold to find contour
	thr.copyTo(con);

	// Create sample and label data
	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;
	Mat sample;
	Mat response_array;
	findContours(con, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Find contour

	for (int i = 0; i < contours.size(); i = hierarchy[i][0]) // iterate through first hierarchy level contours
	{
		Rect r = boundingRect(contours[i]); //Find bounding rect for each contour
		rectangle(src, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 0, 255), 2, 8, 0);
		Mat ROI = thr(r); //Crop the image
		Mat tmp1, tmp2;
		resize(ROI, tmp1, Size(10, 10), 0, 0, INTER_LINEAR); //resize to 10X10
		tmp1.convertTo(tmp2, CV_32FC1); //convert to float
		sample.push_back(tmp2.reshape(1, 1)); // Store  sample data
		imshow("src", src);
		int c = waitKey(0); // Read corresponding label for contour from keyoard
		c -= 0x30;     // Convert ascii to intiger value
		response_array.push_back(c); // Store label to a mat
		rectangle(src, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2, 8, 0);
	}

	// Store the data to file
	Mat response, tmp;
	tmp = response_array.reshape(1, 1); //make continuous
	tmp.convertTo(response, CV_32FC1); // Convert  to float

	FileStorage Data("TrainingData.yml", FileStorage::WRITE); // Store the sample data in a file
	Data << "data" << sample;
	Data.release();

	FileStorage Label("LabelData.yml", FileStorage::WRITE); // Store the label data in a file
	Label << "label" << response;
	Label.release();
	cout << "Training and Label data created successfully....!! " << endl;

	imshow("src", src);
	waitKey();
	return 0;
}

int testLinkList() {
	LinkList mylist;
	int i, j, temp, forLocate, forGet, forInsert, forDelete, a[] = { 6,8,16,2,34,56,7,10,22,45 };
	InitList_L(mylist);
	cout << "我初始化了一个头结点。" << endl;
	CreateList_L_Front(mylist, a, 10);
	cout << "我创建了一个带头结点的单链表，表长为：" << ListLength_L(mylist) << endl;
	cout << endl << "表中的元素依次为：" << endl;
	ListTraverse_L(mylist);
	cout << endl << "<====现在执行定位元素操作,您要找的元素值？" << endl;
	cout << "请输入：";
	cin >> forLocate;
	j = LocateElem_L_2(mylist, forLocate);
	if (j != -1 && GetElem_L(mylist, 1, temp))
		cout << "找到了，它是第" << j + 2 << "个元素（其中，" << temp << "是第一个元素）====>" << endl;
	else cout << "没找到这个元素！" << endl;
	cout << endl << "<====现在执行取元素操作,您要找第几个元素？" << endl;
	cout << "请输入：";
	cin >> forGet;
	if (GetElem_L(mylist, forGet, temp))
		cout << "第" << forGet << "个元素的值是:" << temp << "====>" << endl;
	else cout << "元素不存在！" << endl;
	cout << endl << "<====现在执行插入操作,您要将什么元素插入顺序表？" << endl;
	cout << "请输入：";
	cin >> forInsert;
	cout << "插入到第几个位置？" << endl;
	cout << "请输入：";
	cin >> temp;
	if (!ListInsert_L(mylist, temp, forInsert))
	{
		cout << "插入失败!" << endl; return 0;
	}
	else cout << "==>插入成功，插入后的链表为：====>" << endl;
	ListTraverse_L(mylist);
	cout << endl << "<====现在执行删除操作,您要删除第几个元素？" << endl;
	cout << "请输入：";
	cin >> forDelete;
	if (!ListDelete_L(mylist, forDelete, temp))
	{
		cout << "删除失败!" << endl; return 0;
	}
	else cout << "删除成功，删除后的链表为：====>" << endl;
	ListTraverse_L(mylist);
	cout << endl << "<====现在执行销毁操作***" << endl;
	DestroyList_L(mylist);
	cout << "销毁成功====>" << endl;
	return 0;
}

int testDoubleLinkList() {
	int a, e;
	DuLinkList head, p, q;
	p = q = head = (DuLNode*)malloc(sizeof(DuLNode));
	scanf("%d", &a);
	while (a != 0)
	{
		p = (DuLNode*)malloc(sizeof(DuLNode));
		p->data = a;
		p->prior = q;
		q->next = p;
		q = p;
		scanf("%d", &a);
	}
	q->next = NULL;
	printf("插入之前的双向链表是:");
	p = head->next;
	while (p != NULL)
	{
		printf("%3d,", p->data);
		p = p->next;
	}
	printf("\n");
	if (ListInsert_DuL(head, 5, 7))
	{
		printf("插入成功\n");
		printf("插入之后的双向链表是:");
		p = head->next;
		while (p != NULL)
		{
			printf("%3d,", p->data);
			p = p->next;
		}
		printf("\n");
	}
	else printf("插入失败\n");

	if (ListDelete_DuL(head, 5, e))
	{
		printf("删除成功\n");
		printf("被删除的元素是：%d\n", e);
		printf("删除之后的双向链表是:");
		p = head->next;
		while (p != NULL)
		{
			printf("%3d,", p->data);
			p = p->next;
		}
		printf("\n");
	}
	else printf("删除失败\n");
	return;
}

int testSqList() {
	SqList mylist;
	int i, j, temp, forLocate, forGet, forInsert, forDelete;

	InitList_Sq(mylist, 50, 10);
	for (i = 0, j = 1; i < 10; i++, j++)
		if (!ListInsert_Sq(mylist, i, 2 * j))
		{
			cout << "错误!" << endl; return 0;
		}
	cout << "我创建了一个顺序表，表长为：" << ListLength_Sq(mylist) << endl;
	cout << endl << "表中的元素依次为：" << endl;
	ListTraverse_Sq(mylist);


	cout << endl << "***现在执行元素定位操作,请输入需要查找的元素：***" << endl;
	cin >> forLocate;
	j = LocateElem_Sq(mylist, forLocate);
	if (j != -1) cout << "找到了，它是第" << j + 1 << "个元素。" << endl;
	else cout << "定位失败！" << endl;


	cout << endl << "***现在执行取元素操作,您要找第几个元素？请输入：***" << endl;
	cin >> forGet;
	if (GetElem_Sq(mylist, forGet - 1, temp))
		cout << "第" << forGet << "个元素的值是:" << temp << endl;
	else cout << "取元素失败！" << endl;


	cout << endl << "***现在执行插入操作,您要将什么元素插入顺序表？请输入：***" << endl;
	cin >> forInsert;
	cout << "***插入到第几个位置？***" << endl;
	cin >> temp;
	if (!ListInsert_Sq(mylist, temp - 1, forInsert))
	{
		cout << "插入!" << endl; return 0;
	}
	else cout << "插入成功，插入后的顺序表为：" << endl;
	ListTraverse_Sq(mylist);


	cout << endl << "***现在执行删除操作,您要删除第几个元素？请输入：***" << endl;
	cin >> forDelete;
	if (!ListDelete_Sq(mylist, forDelete - 1, temp))
	{
		cout << "删除失败!" << endl; return 0;
	}
	else cout << "删除成功，删除后的顺序表为：" << endl;
	ListTraverse_Sq(mylist);

	//  visualization(mylist,"showsqlist.dot");

	cout << endl << "***现在执行撤销操作***" << endl;
	DestroyList_Sq(mylist);


	cout << "***顺序表已经销毁！***" << endl;
	return 0;
}

int main()
{
	char get_chUrl[] = "http://space.local.com/admin/wechat/user/add";

	//curlGet(get_chUrl);
	//readTxt("get.txt");

	//char post_chUrl[] = "http://space.local.com/api";
	//string sToken = "n0Q9P5rhCXxfMs6auPHB4Pg1PTlHZUlC";
	//string sMethod = "api.index";
	//string sSign = MD5(sToken + sMethod).toString();
	//string post_sParams = "&method="+ sMethod +"&sign="+ sSign+"&psw=xxx86";
	//char *post_chParams = string2char(post_sParams);

	//curlPost(post_chUrl,post_chParams);
	//readTxt("post.txt");

	//kmeans();

	//string sImgSrc = "F:/verifyCode/verify.png";
	//Mat img = imread(sImgSrc,1);
	//imshow("show", img);
	//waitKey();
	//verifyCode(sImgSrc);

	testLinkList();

	return 0;
}

void curlTest() {
	CURL *curl = 0;
	CURLcode res;
	curl = curl_easy_init();
	if (curl != 0) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://space.local.com/admin/");
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
}
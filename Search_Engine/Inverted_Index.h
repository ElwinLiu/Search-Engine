#pragma once
#include <iostream>
#include <unordered_map>
#include <qvector.h>
#include "qfile.h"
using namespace std;

struct Node
{

};

class inverted_index
{
private:
	unordered_map<QString, int> UtoPR;	//URL映射到page rank
	unordered_map<QString, QVector<int>> WtoIndex;	//单词映射到索引
	QFile file;
public:
	void readData(QString file_path);//建立索引
	void getData(QString str, QString& URL);//取出该行数据
	bool FindUrl(QStringList words, QVector<int>& res);//找到10个最可信的URL及其索引位置
	unordered_map<QString, int> get_UtoPR() { return UtoPR; }	//返回page rank
};
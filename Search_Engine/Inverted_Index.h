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
	unordered_map<QString, int> UtoPR;	//URLӳ�䵽page rank
	unordered_map<QString, QVector<int>> WtoIndex;	//����ӳ�䵽����
	QFile file;
public:
	void readData(QString file_path);//��������
	void getData(QString str, QString& URL);//ȡ����������
	bool FindUrl(QStringList words, QVector<int>& res);//�ҵ�10������ŵ�URL��������λ��
	unordered_map<QString, int> get_UtoPR() { return UtoPR; }	//����page rank
};
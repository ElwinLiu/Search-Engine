#include "Inverted_Index.h"
#include "qtextstream.h"

void inverted_index::readData(QString file_path)
{
    file.setFileName(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//���ļ�
    {
        return;
    }
    int Cur_Pos = file.pos(); //��ǰ����λ��
    int Pos = file.pos(); //Ŀ��URL����λ��
    QString line = file.readLine();//��ȡһ�зŵ��ַ�����
    QString head = "";
    QString URL = "";
    QStringList list;
    QString content;
    while (!line.isNull()) //���ж�ȡ
    {
        if (line.size() == 0) 
        {
            line = file.readLine();//��ȡһ�зŵ��ַ�����
            continue; //׼����ȡ��һ��URL
        }
        head = line[0];

        if (head == "P")
        {
            Pos = Cur_Pos;
            if (Pos == 97)
            {
                int j = 0;
            }
            getData(line, URL);      
            if (UtoPR.count(URL) <= 0)
            {//�����URL�Ĺ�ϣ��δ����
                UtoPR[URL] = 0;  //��ʼ��page rankָ��
            }
        }
        else if (head == "Q")
        {
            getData(line, content); //��ȡ������Ϣ
            list = content.split(" ");  //�Կո�Ϊ�ָ���ȡ�������б�
            for (int i = 0; i < list.size(); i++)
            {
                // ����õ���δ������ӳ���δ�����뵱ǰURL������ӳ��
                if (WtoIndex.find(list[i]) == WtoIndex.end()/* || find(WtoIndex[list[i]].begin(), WtoIndex[list[i]].end(), Pos) == WtoIndex[list[i]].end()*/)
                {
                    if (i == list.size() - 1)   
                        list[i] = list[i].simplified();
                    WtoIndex[list[i]].push_back(Pos); //����ӳ�䵽d��ǰURL������λ��
                }
            }

        }
        else if (head == "L")
        {
            if (UtoPR.count(URL) <= 0)
            {//�����URL�Ĺ�ϣ��δ����
                UtoPR[URL] = 0;  //��ʼ��page rankָ��
            }
            else
            {//����Ѿ���������+1
                UtoPR[URL] = UtoPR[URL] + 1;
            }
        }

        Cur_Pos = file.pos(); //��¼���е���ʼλ��
        line = file.readLine();//��ȡһ�зŵ��ַ�����
    }
    return;
}

void inverted_index::getData(QString str, QString& URL)
{
    URL = "";
    for (int i = 2; i < str.size(); i++)
    {
        URL += str[i];
    }
}

bool inverted_index::FindUrl(QStringList words, QVector<int>& res)
{
    QVector<QVector<int>> allIndex;
    for (int i = 0; i < words.size(); i++)
    {
        if (WtoIndex.find(words[i]) == WtoIndex.end())
        {//���û�иõ���
            return false;
        }
        else
        {
            allIndex.push_back(WtoIndex[words[i]]);
        }
    }
    res = allIndex[0];
    QVector<int> tmp;
    for (int j = 1; j < allIndex.size(); j++)
    {//���ȡ����
        sort(res.begin(), res.end());
        sort(allIndex[j].begin(), allIndex[j].end());
        set_intersection(res.begin(), res.end(),
            allIndex[j].begin(), allIndex[j].end(),
            std::back_inserter(tmp));
    }
    if (allIndex.size() > 1)    //�����������������
        res = tmp;
    file.close();
    return true;
}


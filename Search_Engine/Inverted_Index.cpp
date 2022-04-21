#include "Inverted_Index.h"
#include "qtextstream.h"

void inverted_index::readData(QString file_path)
{
    file.setFileName(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//打开文件
    {
        return;
    }
    int Cur_Pos = file.pos(); //当前索引位置
    int Pos = file.pos(); //目标URL索引位置
    QString line = file.readLine();//读取一行放到字符串里
    QString head = "";
    QString URL = "";
    QStringList list;
    QString content;
    while (!line.isNull()) //逐行读取
    {
        if (line.size() == 0) 
        {
            line = file.readLine();//读取一行放到字符串里
            continue; //准备读取下一个URL
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
            {//如果该URL的哈希表还未建立
                UtoPR[URL] = 0;  //初始化page rank指数
            }
        }
        else if (head == "Q")
        {
            getData(line, content); //提取内容信息
            list = content.split(" ");  //以空格为分隔提取出单词列表
            for (int i = 0; i < list.size(); i++)
            {
                // 如果该单词未建立过映射或未建立与当前URL索引的映射
                if (WtoIndex.find(list[i]) == WtoIndex.end()/* || find(WtoIndex[list[i]].begin(), WtoIndex[list[i]].end(), Pos) == WtoIndex[list[i]].end()*/)
                {
                    if (i == list.size() - 1)   
                        list[i] = list[i].simplified();
                    WtoIndex[list[i]].push_back(Pos); //单词映射到d当前URL的索引位置
                }
            }

        }
        else if (head == "L")
        {
            if (UtoPR.count(URL) <= 0)
            {//如果该URL的哈希表还未建立
                UtoPR[URL] = 0;  //初始化page rank指数
            }
            else
            {//如果已经建立，则+1
                UtoPR[URL] = UtoPR[URL] + 1;
            }
        }

        Cur_Pos = file.pos(); //记录该行的起始位置
        line = file.readLine();//读取一行放到字符串里
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
        {//如果没有该单词
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
    {//逐个取交集
        sort(res.begin(), res.end());
        sort(allIndex[j].begin(), allIndex[j].end());
        set_intersection(res.begin(), res.end(),
            allIndex[j].begin(), allIndex[j].end(),
            std::back_inserter(tmp));
    }
    if (allIndex.size() > 1)    //如果单词两个及以上
        res = tmp;
    file.close();
    return true;
}


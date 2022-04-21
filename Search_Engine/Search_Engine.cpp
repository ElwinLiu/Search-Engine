#pragma execution_character_set("utf-8")
#include "Search_Engine.h"
#include <qfiledialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qpair.h>
Search_Engine::Search_Engine(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());
    QMenuBar* menubar = new QMenuBar;
    QMenu* menu = new QMenu;
    QAction* action_new_file = new QAction();

    menu->setTitle(tr("先点我"));
    //this->menuBar()的作用是跟mainwindow相关联让其作为父级，不然不显示。
    //menubar=this->menuBar();
    setMenuBar(menubar);

    //addmenu()将菜单追加到菜单栏。
    menubar->addMenu(menu);

    action_new_file->setText(tr("导入数据"));

    menu->setToolTipDuration(2000);
    menu->setToolTipsVisible(true);
    action_new_file->setToolTip(tr("选择你要导入的文本文件"));

    //把具体操作添加到菜单中
    menu->addAction(action_new_file);

    connect(action_new_file, SIGNAL(triggered()), this, SLOT(onAction_load_file()));    //导入数据文件响应函数
    connect(ui.search_btn, SIGNAL(clicked()), this, SLOT(search_btn_click()));  //搜索响应函数
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(item_click(QListWidgetItem*)));

}

void Search_Engine::onAction_load_file()
{
    file_path = QFileDialog::getOpenFileName(this, "open file dialog", " ", "Txt files(*.*)");
    Index.readData(file_path);
}

void Search_Engine::search_btn_click()
{
    ui.listWidget->clear();
    QString words = ui.textEdit->toPlainText();   //获取搜索框中的字符串
    QStringList wordsList = words.split(" ");   //空格分割
    QVector<int> res;
    if (!Index.FindUrl(wordsList, res))
    {
        QMessageBox::critical(this, tr("抱歉"), tr("查无关键词"));
        return;
    }
    unordered_map<QString, int> UtoPR = Index.get_UtoPR();  //获取URL到page rank的映射
    QVector<QPair<int, int>> Index_PR;

    QFile file(file_path);
    QString tmp1, tmp;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//打开文件
    {
        QMessageBox::critical(this, tr("抱歉"), tr("文件打开失败"));
        return;
    }

    for (int i = 0; i < res.size(); i++)
    {
        file.seek(res[i]); //文件定位到索引位置
        tmp1 = file.readLine();  //获取URL
        Index.getData(tmp1, tmp);   //除去前缀
        QPair<int, int> p(res[i], UtoPR[tmp]);
        Index_PR.push_back(p);
    }

    sort(Index_PR.begin(), Index_PR.end(), compare);    //由pr值进行索引的排序

    if (Index_PR.size() > 10)
    {
        auto head = Index_PR.begin();
        auto tail = Index_PR.begin() + 10;
        QVector<QPair<int, int>> t(head, tail); //截取前十个网址
        Index_PR = t;
    }

    for (int i = 0; i < Index_PR.size(); i++)
    {
        QListWidgetItem* Item = new QListWidgetItem;
        Item->setWhatsThis(QString::number(Index_PR[i].first));//每个条目备注为索引号

        file.seek(res[i]); //文件定位到索引位置
        tmp1 = file.readLine();  //获取URL
        Index.getData(tmp1, tmp);   //除去前缀

        QString text = "（被引用:" + QString::number(Index_PR[i].second) + "次） " + tmp;
        Item->setText(text);

        ui.listWidget->addItem(Item);
    }
    file.close();
}

void Search_Engine::item_click(QListWidgetItem* item)
{

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//打开文件
    {
        QMessageBox::critical(this, tr("抱歉"), tr("文件打开失败"));
        return;
    }

    QString index = item->whatsThis();
    file.seek(index.toInt());   //定位到索引位置

    QString tmp1, tmp;
    QString content = "";

    tmp1 = file.readLine();  

    QDialog dialog(this);
    QFormLayout form(&dialog);
    dialog.setWindowTitle("网页中的关键字");

    int cnt = 0; //鉴于有的内容太多，最多只取10行

    while (tmp1.size() != 0)
    {
        if (cnt++ == 10)
            break;
        if (tmp1[0] == "Q")
        {
            Index.getData(tmp1, tmp);   //除去前缀
            content += tmp;
        }
        tmp1 = file.readLine();
    }

    QLabel* text1 = new  QLabel;
    text1->setText(content);
    text1->setStyleSheet("background-color: rgb(255, 255, 255);");
    form.addRow(text1);

    file.close();

    dialog.exec();
}

bool compare(QPair<int, int> a, QPair<int, int> b)
{
    return a.second > b.second; //降序排列
}
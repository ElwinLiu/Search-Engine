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

    menu->setTitle(tr("�ȵ���"));
    //this->menuBar()�������Ǹ�mainwindow�����������Ϊ��������Ȼ����ʾ��
    //menubar=this->menuBar();
    setMenuBar(menubar);

    //addmenu()���˵�׷�ӵ��˵�����
    menubar->addMenu(menu);

    action_new_file->setText(tr("��������"));

    menu->setToolTipDuration(2000);
    menu->setToolTipsVisible(true);
    action_new_file->setToolTip(tr("ѡ����Ҫ������ı��ļ�"));

    //�Ѿ��������ӵ��˵���
    menu->addAction(action_new_file);

    connect(action_new_file, SIGNAL(triggered()), this, SLOT(onAction_load_file()));    //���������ļ���Ӧ����
    connect(ui.search_btn, SIGNAL(clicked()), this, SLOT(search_btn_click()));  //������Ӧ����
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
    QString words = ui.textEdit->toPlainText();   //��ȡ�������е��ַ���
    QStringList wordsList = words.split(" ");   //�ո�ָ�
    QVector<int> res;
    if (!Index.FindUrl(wordsList, res))
    {
        QMessageBox::critical(this, tr("��Ǹ"), tr("���޹ؼ���"));
        return;
    }
    unordered_map<QString, int> UtoPR = Index.get_UtoPR();  //��ȡURL��page rank��ӳ��
    QVector<QPair<int, int>> Index_PR;

    QFile file(file_path);
    QString tmp1, tmp;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//���ļ�
    {
        QMessageBox::critical(this, tr("��Ǹ"), tr("�ļ���ʧ��"));
        return;
    }

    for (int i = 0; i < res.size(); i++)
    {
        file.seek(res[i]); //�ļ���λ������λ��
        tmp1 = file.readLine();  //��ȡURL
        Index.getData(tmp1, tmp);   //��ȥǰ׺
        QPair<int, int> p(res[i], UtoPR[tmp]);
        Index_PR.push_back(p);
    }

    sort(Index_PR.begin(), Index_PR.end(), compare);    //��prֵ��������������

    if (Index_PR.size() > 10)
    {
        auto head = Index_PR.begin();
        auto tail = Index_PR.begin() + 10;
        QVector<QPair<int, int>> t(head, tail); //��ȡǰʮ����ַ
        Index_PR = t;
    }

    for (int i = 0; i < Index_PR.size(); i++)
    {
        QListWidgetItem* Item = new QListWidgetItem;
        Item->setWhatsThis(QString::number(Index_PR[i].first));//ÿ����Ŀ��עΪ������

        file.seek(res[i]); //�ļ���λ������λ��
        tmp1 = file.readLine();  //��ȡURL
        Index.getData(tmp1, tmp);   //��ȥǰ׺

        QString text = "��������:" + QString::number(Index_PR[i].second) + "�Σ� " + tmp;
        Item->setText(text);

        ui.listWidget->addItem(Item);
    }
    file.close();
}

void Search_Engine::item_click(QListWidgetItem* item)
{

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//���ļ�
    {
        QMessageBox::critical(this, tr("��Ǹ"), tr("�ļ���ʧ��"));
        return;
    }

    QString index = item->whatsThis();
    file.seek(index.toInt());   //��λ������λ��

    QString tmp1, tmp;
    QString content = "";

    tmp1 = file.readLine();  

    QDialog dialog(this);
    QFormLayout form(&dialog);
    dialog.setWindowTitle("��ҳ�еĹؼ���");

    int cnt = 0; //�����е�����̫�࣬���ֻȡ10��

    while (tmp1.size() != 0)
    {
        if (cnt++ == 10)
            break;
        if (tmp1[0] == "Q")
        {
            Index.getData(tmp1, tmp);   //��ȥǰ׺
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
    return a.second > b.second; //��������
}
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Search_Engine.h"
#include "Inverted_Index.h"

class Search_Engine : public QMainWindow
{
    Q_OBJECT

public:
    Search_Engine(QWidget *parent = Q_NULLPTR);

private slots:
    void onAction_load_file();
    void search_btn_click();
    void item_click(QListWidgetItem* item);
private:
    Ui::Search_EngineClass ui;
    inverted_index Index;
    QString file_path;
};

bool compare(QPair<int, int> a, QPair<int, int> b); //±È½Ïº¯Êý
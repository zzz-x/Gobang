#pragma once
#pragma execution_character_set("utf-8") 
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"
#include "AI.h"
#include<qstring.h>
#include"chessThread.h"

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = Q_NULLPTR);
    ~QtWidgetsApplication2();

public slots:
    void paintComputer();
protected:
    void paintEvent(QPaintEvent* event);
    void showSearchInfo(const SEARCH_INFO info);
    void mousePressEvent(QMouseEvent* event); //人走
private:
    Ui::QtWidgetsApplication2Class ui;
    int gridW; //格子宽度
    int gridH; //格子高度
    int startX; //起始坐标
    int startY;

    chessThread* subthread;
    

    int chessX;
    int chessY;

    bool allowMouse;
    bool firstPlay;
    
};

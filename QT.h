#pragma once
#pragma execution_character_set("utf-8") 
#include <QtWidgets/QMainWindow>
#include "ui_QT.h"
#include "AI.h"
#include<qstring.h>
#include"chessThread.h"
#include<qbuttongroup.h>
#include<qpainter.h>
#include<qpen.h>
#include<qpixmap.h>
#include<QDebug>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QLabel>
#include<qmessagebox.h>
#include<QString>
#include<QMenu>
#include<QAction>
#include<qbuttongroup.h>
#include<QMenubar>
#include<qbrush.h>
#include<qpropertyanimation.h>
#include<qvector.h>

class QT : public QMainWindow
{
    Q_OBJECT
public:
    QT(QWidget* parent = Q_NULLPTR);
    Ui::QTClass ui;
public slots:
    void paintComputer();
protected:
    void paintEvent(QPaintEvent* event);
    void showSearchInfo(const SEARCH_INFO info);
    void mousePressEvent(QMouseEvent* event); //人走

private:
    int gridW; //格子宽度
    int gridH; //格子高度
    int startX; //起始坐标
    int startY;

    chessThread* subthread;


    int chessX;
    int chessY;

    bool isbegin;
    bool allowMouse;
    int firstPlay;
    int depth;
    QVector<QPushButton*>group_button;
};

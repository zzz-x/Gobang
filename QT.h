#pragma once
#pragma execution_character_set("utf-8") 
#include <QtWidgets/QMainWindow>
#include "ui_QT.h"
#include "AI.h"
#include<qstring.h>
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
#include<qstack.h>
#include<QMenu>
#include<QAction>
#include<qbuttongroup.h>
#include<QMenubar>
#include<qbrush.h>
#include<qpropertyanimation.h>
#include<qvector.h>
#include<qthread.h>
#include<qfile.h>


struct ChessInfo {
    int row;
    int col;
    bool player;
};

class QT : public QMainWindow
{
    Q_OBJECT
public:
    QT(QWidget* parent = Q_NULLPTR);
    Ui::QTClass ui;
public slots:
    void paintComputer();
    void undo();
    void replay();
    void start();
protected:
    void paintEvent(QPaintEvent* event);
    void showSearchInfo(const SEARCH_INFO info);
    void mousePressEvent(QMouseEvent* event); //人走
    void drawChess(int row, int col, int player);

private:
    AI ai;
    int gridW; //格子宽度
    int gridH; //格子高度
    int startX; //起始坐标
    int startY;

    int chessX;
    int chessY;

    ChessInfo recentPos;

    bool isbegin;     //是否开始
    bool receiveAns;  //是否接收搜索数据
    bool allowMouse; //是否允许鼠标点击
    int firstPlay;  //谁是先手
    int depth;      //搜索深度

    QVector<QPushButton*>group_button;
    QThread* mythread;

    QStack<ChessInfo>history;
    
    QVector<QVector<QLabel*>>label{ROW_RANGE,QVector<QLabel*>(COL_RANGE)};
    void connectButton();
    void clearSelectStyle(QVector<QPushButton*>&group,int depth);

signals:
    void beginSearch();
    void humanTurn();
    void aiTurn();
};

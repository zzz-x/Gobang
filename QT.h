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
    void mousePressEvent(QMouseEvent* event); //����
    void drawChess(int row, int col, int player);

private:
    AI ai;
    int gridW; //���ӿ��
    int gridH; //���Ӹ߶�
    int startX; //��ʼ����
    int startY;

    int chessX;
    int chessY;

    ChessInfo recentPos;

    bool isbegin;     //�Ƿ�ʼ
    bool receiveAns;  //�Ƿ������������
    bool allowMouse; //�Ƿ����������
    int firstPlay;  //˭������
    int depth;      //�������

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

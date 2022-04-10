#pragma once
#pragma execution_character_set("utf-8") 
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"
#include "AI.h"

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = Q_NULLPTR);
    ~QtWidgetsApplication2();
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event); //����
    void AiPlay();
private:
    Ui::QtWidgetsApplication2Class ui;
    AI ai;
    int gridW; //���ӿ��
    int gridH; //���Ӹ߶�
    int startX; //��ʼ����
    int startY;

    int chessX;
    int chessY;
    bool isBegin;

    bool isAI=0;
    bool firstPlay;
    
};

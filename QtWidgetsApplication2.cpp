#include "QtWidgetsApplication2.h"
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
#include<QMenubar>
#include<qbrush.h>

 
QtWidgetsApplication2::QtWidgetsApplication2(QWidget *parent)
    : QMainWindow(parent)
{
    chessX = chessY = -1;
    
    subthread = new chessThread;
    subthread->init(4);
    QMenuBar* mBar =menuBar();
    QMenu* difficulty = mBar->addMenu("难度");
    QAction* easy = difficulty->addAction("初级(2层搜索)");
    QAction* medium = difficulty->addAction("中级(4层搜索)");
    QAction* hard= difficulty->addAction("高级(6层搜索)");
    
    QMenu* order = mBar->addMenu("先手");
    QAction* humanfirst = order->addAction("玩家先手");
    QAction* aifirst = order->addAction("电脑先手");

    connect(humanfirst, &QAction::triggered, [=]() {
        firstPlay = HUMAN;
        allowMouse = 1;
        });
    connect(aifirst, &QAction::triggered, [=]() {
        firstPlay = COMPUTER;
        allowMouse =1;
        });
    connect(easy, &QAction::triggered, [=]() {
        subthread->setDepth(2);
        });

    connect(medium, &QAction::triggered, [=]() {
        subthread->setDepth(4); 
        });

    connect(hard, &QAction::triggered, [=]() {
        subthread->setDepth(6); 
        });
    ui.setupUi(this);
    ui.centralWidget->setMouseTracking(true);

    allowMouse = 0;
}

QtWidgetsApplication2::~QtWidgetsApplication2()
{

}

void QtWidgetsApplication2::paintEvent(QPaintEvent* event)
{
    int val = (height() > width()) ? width() : height();
    gridH = val/ 17;
    gridW = val/ 17;

    startX = gridW;
    startY = 2*gridH;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
   
    QPen pen;
    pen.setWidth(2);
    p.setPen(pen);
   
    for (int i = 0; i < 15; i++) {
        //横线
        p.drawLine(startX, startY+i*gridH, startX + 14 * gridW, startY + i * gridH);
        //竖线
        p.drawLine(startX+i*gridW, startY, startX + i * gridW, startY + 14 * gridH);
    }
    if (firstPlay == COMPUTER) {
        QPixmap pix;
        pix = QPixmap("black.png");
        QLabel* lab = new QLabel(this);
        int pixelX = startX - gridW / 2 + 7 * gridW;
        int pixelY = startY - gridH / 2 + 7* gridH;
        pix = pix.scaled(gridH, gridW);
        lab->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
        lab->setPixmap(pix);
        lab->show();
        subthread->setChess(7, 7, COMPUTER);
    }
 }

void QtWidgetsApplication2::showSearchInfo(const SEARCH_INFO info)
{
    ui.label->setText(QString("思考时间:")+QString::number(info.time)+QString(" ms"));
    ui.label_2->setText(QString("搜索结点数:")+QString::number(info.cutNodes));
    ui.label_3->setText(QString("剪枝结点数:") + QString::number(info.validNodes));
}

void QtWidgetsApplication2::mousePressEvent(QMouseEvent* event)
{
    if (allowMouse) {
        int clickXpos = event->x();
        int clickYpos = event->y();
        if (!(clickXpos >= startX - gridW / 2 && clickXpos <= startX - gridW / 2 + 15 * gridW
            && clickYpos >= startY - gridH / 2 && clickYpos <= startY - gridH / 2 + 15 * gridH))
            return;
        //禁用鼠标
        qDebug() << "mouse" << allowMouse << endl;

        allowMouse = 0;

        //棋盘位置转化为数组坐标值
        chessX = (clickXpos - startX + gridW / 2) / gridW;
        chessY = (clickYpos - startY + gridH / 2) / gridH;
        if (subthread->check(chessY, chessX, COMPUTER) || subthread->check(chessY, chessX, HUMAN))
            return;
        subthread->setChess(chessY, chessX, HUMAN);

        QPixmap pix;

        if (firstPlay == HUMAN)
            pix = QPixmap("black.png");
        else
            pix = QPixmap("white.png");

        pix = pix.scaled(gridH, gridW);
        QLabel* lab = new QLabel(this);
        int pixelX = startX - gridW / 2 + chessX * gridW;
        int pixelY = startY - gridH / 2 + chessY * gridH;
        lab->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
        lab->setPixmap(pix);
        lab->show();

        if (subthread->gameOver(HUMAN)) {
            int ret = QMessageBox::question(this,
                tr("游戏结果"),
                tr("你赢了"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
            return;
        }
        else {
            subthread->start();
            connect(subthread, SIGNAL(isDone()), this, SLOT(paintComputer()));
        }
    }
}


void QtWidgetsApplication2::paintComputer() {
    SEARCH_INFO res = subthread->getResult();
    showSearchInfo(res);
    chessX = res.nextPosCol;
    chessY = res.nextPosRow;
    QPixmap pix;

    if (firstPlay == HUMAN)
        pix = QPixmap("white.png");
    else
        pix = QPixmap("black.png");

    pix = pix.scaled(gridH, gridW);
    QLabel* lab = new QLabel(this);
    lab->setPixmap(pix);
    int pixelX = startX - gridW / 2 + chessX * gridW;
    int pixelY = startY - gridH / 2 + chessY * gridH;
    lab->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
    lab->show();
    if (subthread->gameOver(COMPUTER)) {
        int ret = QMessageBox::question(this,
            tr("游戏结果"),
            tr("你输了"),
            QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    allowMouse = 1;
}


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
    ai.SearchDepth = 4;

    ai.ZobristInit();
    QMenuBar* mBar =menuBar();
    QMenu* difficulty = mBar->addMenu("�Ѷ�");
    QAction* easy = difficulty->addAction("����(2������)");
    QAction* medium = difficulty->addAction("�м�(4������)");
    QAction* hard= difficulty->addAction("�߼�(6������)");
    
    QMenu* order = mBar->addMenu("����");
    QAction* humanfirst = order->addAction("�������");
    QAction* aifirst = order->addAction("��������");

    connect(humanfirst, &QAction::triggered, [=]() {
        firstPlay = HUMAN;
        });
    connect(aifirst, &QAction::triggered, [=]() {
        firstPlay = COMPUTER;
        });
    connect(easy, &QAction::triggered, [=]() {
        ai.setDepth(2);
        });

    connect(medium, &QAction::triggered, [=]() {
        ai.setDepth(4);
        });

    connect(hard, &QAction::triggered, [=]() {
        ai.setDepth(6);
        });
    ui.setupUi(this);
    ui.centralWidget->setMouseTracking(true);
    isBegin = 0;
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
    p.setRenderHint(QPainter::Antialiasing, true); // �����
   
    QPen pen;
    pen.setWidth(2);
    p.setPen(pen);
   

    for (int i = 0; i < 15; i++) {
        //����
        p.drawLine(startX, startY + i * gridH, startX + 14 * gridW, startY + i * gridH);
        //����
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
        ai.aiChess[7][7] = 1;
    }
    //QPainter blackp(this);
    //blackp.setRenderHint(QPainter::Antialiasing, true);  //������Ⱦ,���������
    //blackp.setPen(QPen(Qt::black, 3));
    //QBrush blackb(Qt::SolidPattern);
    //blackb.setColor(Qt::black);
    //blackp.setBrush(blackb);

    //QPainter whitep(this);
    //whitep.setRenderHint(QPainter::Antialiasing, true);  //������Ⱦ,���������
    //whitep.setPen(QPen(Qt::white, 3));
    //QBrush whiteb(Qt::SolidPattern);
    //whiteb.setColor(Qt::white);
    //whitep.setBrush(whiteb);
    //
    //if (firstPlay == COMPUTER&&isBegin==0) {
    //    int pixelX = startX-gridH/2+ 7 * gridW;
    //    int pixelY = startY-gridW/2+ 7 * gridH;
    //    blackp.drawEllipse(pixelX, pixelY, gridH-2, gridW-2);
    //    update();
    //    isBegin = 1;
    //}
    //else {
    //    for (int i = 0; i < ROW_RANGE; i++) {
    //        for (int j = 0; j < COL_RANGE; j++)
    //        {
    //            int pixelX = startX - gridH / 2 + j * gridW;
    //            int pixelY = startY - gridW / 2 + i * gridH;
    //            if (ai.aiChess[i][j]) {
    //                if (firstPlay == COMPUTER)
    //                    blackp.drawEllipse(pixelX, pixelY, gridH - 2, gridW - 2);
    //                else
    //                    whitep.drawEllipse(pixelX, pixelY, gridH - 2, gridW - 2);
    //            }
    //            else if (ai.manChess[i][j]) {
    //                if (firstPlay == COMPUTER)
    //                    whitep.drawEllipse(pixelX, pixelY, gridH - 2, gridW - 2);
    //                else
    //                    blackp.drawEllipse(pixelX, pixelY, gridH - 2, gridW - 2);
    //            }
    //        }
    //    }
    //    update();
    //}
}

void QtWidgetsApplication2::mousePressEvent(QMouseEvent* event)
{
    int clickXpos = event->x();
    int clickYpos = event->y();
    if (!(clickXpos >= startX - gridW / 2 && clickXpos <= startX - gridW / 2 + 15 * gridW
        && clickYpos >= startY - gridH / 2 && clickYpos <= startY - gridH / 2 + 15 * gridH))
        return;

    //����λ��ת��Ϊ��������ֵ
    chessX = (clickXpos - startX + gridW / 2) / gridW;
    chessY = (clickYpos - startY + gridH / 2) / gridH;
    if (ai.aiChess[chessY][chessX] || ai.manChess[chessY][chessX])
        return;
    ai.manChess[chessY][chessX] = true;
    isAI = false;
    ai.nextPos = { chessY,chessX };
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
    //update();

    if (ai.gameOver(HUMAN)) {
        int ret = QMessageBox::question(this,
            tr("��Ϸ���"),
            tr("��Ӯ��"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
        return;
    }
    AiPlay();
    if (ai.gameOver(COMPUTER)) {
        int ret = QMessageBox::question(this,
            tr("��Ϸ���"),
            tr("����Ӯ��"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
        return;
    }
    isAI = 1;
}

void QtWidgetsApplication2::AiPlay()
{
   
   ai.MinMaxSearch(COMPUTER, ai.SearchDepth, -0x3fffffff, 0x3fffffff);
   chessX = ai.nextPos.col;
   chessY = ai.nextPos.row;
   ai.aiChess[chessY][chessX] = true;

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
   //update();
}

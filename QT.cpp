#include "QT.h"



QT::QT(QWidget* parent)
    : QMainWindow(parent)
{
    chessX = chessY = -1;
    ui.setupUi(this);

    firstPlay = -1;
    isbegin = 0;
    depth = -1;

    subthread = new chessThread;
    subthread->init(4);
    
    group_button.push_back(ui.pushButton);
    group_button.push_back(ui.pushButton_2);
    group_button.push_back(ui.pushButton_3);
    group_button.push_back(ui.pushButton_4);
    group_button.push_back(ui.pushButton_5);
    group_button.push_back(ui.pushButton_6);

    ui.pushButton_6->setVisible(0);


    connect(ui.pushButton, &QPushButton::clicked, [=] {
        for (int i = 0; i < 3; i++) {
            if ((i + 1) * 2 == depth)
                group_button[i]->setStyleSheet(QString("background-color:;"));
        }
        subthread->setDepth(depth=2);
        group_button[0]->setStyleSheet(QString("background-color:rgb(204,236,255);"));
        });
    connect(ui.pushButton_2, &QPushButton::clicked, [=] {
        for (int i = 0; i < 3; i++) {
            if ((i + 1) * 2 == depth)
                group_button[i]->setStyleSheet(QString("background-color:;"));
        }
        group_button[1]->setStyleSheet(QString("background-color:rgb(204,236,255);"));
        subthread->setDepth(depth=4);
        });
    connect(ui.pushButton_3, &QPushButton::clicked, [=] {
        for (int i = 0; i < 3; i++) {
            if ((i + 1) * 2 == depth)
                group_button[i]->setStyleSheet(QString("background-color:;"));
        }
        group_button[2]->setStyleSheet(QString("background-color:rgb(204,236,255);"));
        subthread->setDepth(depth=6);
        });

    connect(ui.pushButton_4, &QPushButton::clicked, [=] {
        firstPlay = HUMAN;
        ui.pushButton_4->setStyleSheet(QString("background-color:rgb(204,236,255);"));
        ui.pushButton_5->setEnabled(0);
        allowMouse = 1;
        });

    connect(ui.pushButton_5, &QPushButton::clicked, [=] {
        firstPlay = COMPUTER;
        ui.pushButton_5->setStyleSheet(QString("background-color:rgb(204,236,255);"));
        ui.pushButton_4->setEnabled(0);
        allowMouse = 1;
        });

    connect(ui.pushButton_6, &QPushButton::clicked, [=] {
        ;
        });
    connect(ui.pushButton_7, &QPushButton::clicked, [=] {
        if (firstPlay == -1 || depth == -1) {
            int ret = QMessageBox::warning(this,
                tr(""),
                tr("��ѡ����ѶȺ�ģʽ"),
                QMessageBox::Ok ,
                QMessageBox::Ok);
            return;
        }
        else {
            ui.pushButton_7->setVisible(0);
            ui.pushButton_6->setVisible(1);
            isbegin = 1;
        }
        });





    ui.centralWidget->setMouseTracking(true);

    allowMouse = 0;
}



void QT::paintEvent(QPaintEvent* event)
{
    int val = (height() > width()) ? width() : height();
    gridH = val / 17;
    gridW = val / 17;

    startX = gridW;
    startY = 2 * gridH;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true); // �����

    QPen pen;
    pen.setWidth(2);
    p.setPen(pen);

    for (int i = 0; i < 15; i++) {
        //����
        p.drawLine(startX, startY + i * gridH, startX + 14 * gridW, startY + i * gridH);
        //����
        p.drawLine(startX + i * gridW, startY, startX + i * gridW, startY + 14 * gridH);
    }
    if (firstPlay == COMPUTER&&isbegin) {
        QPixmap pix;
        pix = QPixmap("black.png");
        QLabel* lab = new QLabel(this);
        int pixelX = startX - gridW / 2 + 7 * gridW;
        int pixelY = startY - gridH / 2 + 7 * gridH;
        pix = pix.scaled(gridH, gridW);
        lab->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
        lab->setPixmap(pix);
        lab->show();
        subthread->setChess(7, 7, COMPUTER);
    }
}

void QT::showSearchInfo(const SEARCH_INFO info)
{
    ui.label->setText(QString("˼��ʱ��:") + QString::number(info.time) + QString(" ms"));
    ui.label_2->setText(QString("���������:") + QString::number(info.cutNodes));
    ui.label_3->setText(QString("��֦�����:") + QString::number(info.validNodes));
}

void QT::mousePressEvent(QMouseEvent* event)
{
    if (allowMouse) {
        int clickXpos = event->x();
        int clickYpos = event->y();
        if (!(clickXpos >= startX - gridW / 2 && clickXpos <= startX - gridW / 2 + 15 * gridW
            && clickYpos >= startY - gridH / 2 && clickYpos <= startY - gridH / 2 + 15 * gridH))
            return;
        //�������
        qDebug() << "mouse" << allowMouse << endl;

        allowMouse = 0;

        //����λ��ת��Ϊ��������ֵ
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
                tr("��Ϸ���"),
                tr("��Ӯ��"),
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



void QT::paintComputer() {
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
    allowMouse = 1;
    if (subthread->gameOver(COMPUTER)) {
        allowMouse = 0;
        int ret = QMessageBox::question(this,
            tr("��Ϸ���"),
            tr("������"),
            QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
}

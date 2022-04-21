#include "QT.h"

const QString selectStyle = QString("background-color: #87CEFF;border: 1px solid #dcdfe6;border-radius: 10px; ");


QT::QT(QWidget* parent)
    : QMainWindow(parent)
{    
    //CAE5E8;
    QPalette palette(this->palette());

    palette.setColor(QPalette::Background, QColor(0xECECEC));

    this->setPalette(palette);

    chessX = chessY = -1;
    ui.setupUi(this);
    /********初始化元素*********/
    firstPlay = -1, depth = -1;
    isbegin = false, receiveAns = true;
    

    mythread = new QThread;
    ai.moveToThread(mythread); 
    connect(this, SIGNAL(beginSearch()), &ai, SLOT(search()));
    connect(&ai, SIGNAL(isDone()), this, SLOT(paintComputer()));
    mythread->start();

    ui.pushButton_6->setVisible(0);
    group_button.push_back(ui.pushButton);
    group_button.push_back(ui.pushButton_2);
    group_button.push_back(ui.pushButton_3);
    group_button.push_back(ui.pushButton_4);
    group_button.push_back(ui.pushButton_5);
    group_button.push_back(ui.pushButton_6);

    QFile qss("QPushButton_Mode.qss");
    qss.open(QFile::ReadOnly);
    QString style = QLatin1String(qss.readAll());
    for (int i = 0; i < 3; i++)
        group_button[i]->setStyleSheet(style);
    qss.close();

    qss.setFileName("QPushButton_Start.qss");
    qss.open(QFile::ReadOnly);
    style = QLatin1String(qss.readAll());
    for (int i = 4; i < 6; i++)
        group_button[i]->setStyleSheet(style);
    qss.close();

    connectButton();

    ui.centralWidget->setMouseTracking(true);

    allowMouse = 0;

    recentPos.row = recentPos.col = -1;
}



void QT::undo()
{
    if(!history.isEmpty()) {
        auto point = history.top();
        qDebug()<<"undo "<< point.row << ' ' << point.col << endl;
        history.pop();
        delete label[point.row][point.col];

        mythread->terminate();
        mythread->wait();
        delete mythread;
        mythread = new QThread;
        ai.moveToThread(mythread);
        mythread->start();
        allowMouse = true;
        qDebug() << "allow mouse" << endl;
    }
}

void QT::replay()
{
    //取消重新开始，启用开始
    ui.pushButton->setChecked(false);
    ui.pushButton_2->setChecked(false);
    ui.pushButton_3->setChecked(false);

    clearSelectStyle(group_button, depth);

    ui.pushButton_5->setVisible(true);
    ui.pushButton_6->setVisible(false);
    allowMouse = false;
    isbegin = false;
    firstPlay = -1;
    depth = -1;
    while (!history.empty()) {
        auto point = history.top();
        delete label[point.row][point.col];
        history.pop();
    }
    ai.clear();
}

void QT::start()
{
    if (depth == -1) {
        QMessageBox::warning(this,
            tr("注意事项"),
            tr("请先选择难度"),
            tr("确定"));
    }
    else {
        int ret = QMessageBox::information(this,
            tr("选择先手"),
            tr("请选择谁是先手"),
            tr("我先下"),
            tr("电脑先下")
        );
        firstPlay = ret;
        isbegin = true;
        allowMouse = true;
        ui.pushButton_5->setVisible(0);
        ui.pushButton_6->setVisible(1);
        if (firstPlay == COMPUTER) {
            ai.setChess(7, 7, COMPUTER);
            history.push_back({ 7,7,COMPUTER });
            recentPos = { 7,7,COMPUTER };//记录
            drawChess(7, 7, COMPUTER);
        }
    }
}

void QT::paintEvent(QPaintEvent* event)
{
    int val = (height() > width()) ? width() : height();
    gridH = val / 18;
    gridW = val / 18;


    startX = gridW+10;
    startY = gridH+10;
    QPainter painter(this);
    painter.setBrush(QBrush(qRgb(0xd2, 0xaf, 0x8f)));
    painter.drawRect(startX/2+5, startY/2+5, gridH * 15, gridW * 15);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    //背景色
    QPen pen;
    pen.setWidth(2);
    p.setPen(pen);

    for (int i = 0; i < 15; i++) {
        //横线
        p.drawLine(startX, startY + i * gridH, startX + 14 * gridW, startY + i * gridH);
        //竖线
        p.drawLine(startX + i * gridW, startY, startX + i * gridW, startY + 14 * gridH);
    }
   
    p.setBrush(Qt::black);
    p.drawRect(startX + 3 * gridW - 3, startY + 3 * gridH - 3, 6, 6);
    p.drawRect(startX + 11 * gridW - 3, startY +3 * gridH - 3, 6, 6);
    p.drawRect(startX + 3 * gridW - 3, startY + 11 * gridH - 3, 6, 6);
    p.drawRect(startX + 11 * gridW - 3, startY + 11 * gridH - 3, 6, 6);
    p.drawRect(startX + 7 * gridW - 3, startY + 7 * gridH - 3, 6, 6);
}

void QT::showSearchInfo(const SEARCH_INFO info)
{
    ui.label->setText(QString("思考时间:") + QString::number(info.time) + QString(" ms"));
    ui.label_2->setText(QString("搜索结点数:") + QString::number(info.cutNodes));
    ui.label_3->setText(QString("剪枝结点数:") + QString::number(info.validNodes));
}

void QT::mousePressEvent(QMouseEvent* event)
{
    if (allowMouse) {
        int clickXpos = event->x();
        int clickYpos = event->y();
        if (!(clickXpos >= startX - gridW / 2 && clickXpos <= startX - gridW / 2 + 15 * gridW
            && clickYpos >= startY - gridH / 2 && clickYpos <= startY - gridH / 2 + 15 * gridH))
            return;
        //防止重复点击
        allowMouse = 0;
        //棋盘位置转化为数组坐标值
        chessX = (clickXpos - startX + gridW / 2) / gridW;
        chessY = (clickYpos - startY + gridH / 2) / gridH;
        if (ai.checkChess(chessY, chessX, HUMAN) || ai.checkChess(chessY, chessX, COMPUTER)) {  //判断该位置是否有棋子
            allowMouse = true;
            qDebug() << "already have chess" << endl;
            return;
        }
        ai.setChess(chessY, chessX, HUMAN);
        history.push({ chessY,chessX,HUMAN });  //压入栈
        drawChess(chessY, chessX, HUMAN);
        recentPos = { chessY,chessX,HUMAN };//记录位置

        if (ai.gameOver(HUMAN)&&isbegin) {
            int ret = QMessageBox::information(this,
                tr("游戏结果"),
                tr("你赢了"),
                QMessageBox::Ok,
                QMessageBox::Ok);
            isbegin = 0;
            allowMouse = false;
            return;
        }
        else {
            ui.pushButton_4->setEnabled(1);
            emit beginSearch();
        }
    }
}

void QT::drawChess(int row, int col, int player)
{
    QPixmap pix;
    if (player == firstPlay)
        pix = QPixmap("black_select.png");
    else
        pix = QPixmap("white_select.png");
    label[row][col] = new QLabel(this);
    int pixelX = startX - gridW / 2 + col * gridW;
    int pixelY = startY - gridH / 2 + row * gridH;
    pix = pix.scaled(gridH, gridW);
    label[row][col]->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
    label[row][col]->setPixmap(pix);
    label[row][col]->show();

    if (recentPos.row != -1) {
        if (recentPos.player == firstPlay)
            pix = QPixmap("black.png");
        else
            pix = QPixmap("white.png");
        delete label[recentPos.row][recentPos.col];
        label[recentPos.row][recentPos.col] = new QLabel(this);
        int pixelX = startX - gridW / 2 + recentPos.col * gridW;
        int pixelY = startY - gridH / 2 + recentPos.row * gridH;
        pix = pix.scaled(gridH, gridW);
        label[recentPos.row][recentPos.col]->setGeometry(QRect(pixelX, pixelY, pix.width(), pix.height()));
        label[recentPos.row][recentPos.col]->setPixmap(pix);
        label[recentPos.row][recentPos.col]->show();
    }
}

void QT::connectButton()
{
    connect(ui.pushButton, &QPushButton::clicked, [=] {
        clearSelectStyle(group_button, depth);
        ai.setDepth(depth = 2);
        group_button[0]->setStyleSheet(selectStyle);
        });
    connect(ui.pushButton_2, &QPushButton::clicked, [=] {
        clearSelectStyle(group_button, depth);
        group_button[1]->setStyleSheet(selectStyle);
        ai.setDepth(depth = 4);
        });
    connect(ui.pushButton_3, &QPushButton::clicked, [=] {
        clearSelectStyle(group_button, depth);
        group_button[2]->setStyleSheet(selectStyle);
        ai.setDepth(depth = 6);
        });

    connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(undo()));

    connect(ui.pushButton_5, SIGNAL(clicked()),this,SLOT(start()));

    connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(replay()));
}

void QT::clearSelectStyle(QVector<QPushButton*>& group, int depth)
{
    QFile qss("QPushButton_Mode.qss");
    qss.open(QFile::ReadOnly);
    QString style = QLatin1String(qss.readAll());
    for (int i = 0; i < 3; i++)
        if ((i + 1) * 2 == depth)
            group[i]->setStyleSheet(style);
    qss.close();
    
}

void QT::paintComputer() {
    ui.pushButton_4->setEnabled(0);
    if (!receiveAns) {
        receiveAns = true;
        return;
    }
    SEARCH_INFO res = ai.searchRes;   //获取search结果
    showSearchInfo(res);        //QT界面显示信息
    chessX = res.nextPosCol;
    chessY = res.nextPosRow;
    ai.setChess(chessY, chessX, COMPUTER);
    history.push({ chessY,chessX,COMPUTER});
    drawChess(chessY, chessX, COMPUTER);
    recentPos = { chessY,chessX,COMPUTER };

    if (isbegin) {
        if (ai.gameOver(COMPUTER)) {
            allowMouse =isbegin = 0;
            int ret = QMessageBox::information(this,
                tr("游戏结果"),
                tr("你输了"),
                QMessageBox::Ok,
                QMessageBox::Ok);
            return;
        }
        else
            allowMouse = 1;
    }
}

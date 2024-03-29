#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "annotationitem.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QGridLayout>
#include <QListWidget>
#include <QList>
#include <QDebug>
#include <QColorDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_image2d(nullptr),
    m_paintBoard2d(nullptr),
    m_openStatus(0)
{
    // ui准备
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->listWidget->setVisible(false);
    ui->pushButton->setVisible(false);


    // 初始化图像类
    m_image2d = new Image2D(this);
    m_image3d = new Image3D(this);

    // 链接Action和槽
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(selectFile()));
    connect(ui->actionFolder2D, SIGNAL(triggered()), this, SLOT(selectFolder()));
    connect(ui->actionFolder3D, SIGNAL(triggered()), this, SLOT(selectFolder3D()));
    connect(ui->actionSave2, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));

    // 画图操作的链接
    connect(ui->actionRectangle, SIGNAL(triggered()), this, SLOT(drawRect()));
    connect(ui->actionRoundPaint, SIGNAL(triggered()), this, SLOT(drawAreaRound()));
    connect(ui->actionSquarePaint, SIGNAL(triggered()), this, SLOT(drawAreaSquare()));
    // connect(this, SIGNAL(chooseShape2d(int)), m_paintBoard2d, SLOT(drawShape(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 直接打开2D
void MainWindow::selectFile()
{
    if(m_openStatus != 0)
    {
        closeCurrent();
    }

    m_image2d->selectFile();

    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->listWidget->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->listWidget->clear();

    /*
    if(m_annotationList != nullptr)
    {
        delete m_annotationList;
    }
    */
    m_annotationList = new AnnotationList;
    connect(m_annotationList, SIGNAL(addListItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));
    connect(m_annotationList, SIGNAL(popListItem(int)), this, SLOT(popListItem(int)));

    /*
    if(m_paintBoard2d != nullptr)
    {
        delete m_paintBoard2d;
    }
    */
    m_paintBoard2d = new PaintBoard(this, m_image2d->getPixmap(), ui->textBrowser);
    m_paintBoard2d->registerAnnotationList(m_annotationList);
    ui->gridLayout->addWidget(m_paintBoard2d, 0, 0);
    connect(this, SIGNAL(chooseShape2d(int, QColor)), m_paintBoard2d, SLOT(drawShape(int, QColor)));
    connect(m_paintBoard2d, SIGNAL(addItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));

    QString filename(m_image2d->m_filename);
    filename += ".txt";
    m_annotationList->loadFromFile(filename);
    m_paintBoard2d->loadAnnotation();

    m_openStatus = 1;
}

// 文件夹形式打开2D
void MainWindow::selectFolder()
{
    if(m_openStatus != 0)
    {
        closeCurrent();
    }

    m_image2d->selectFolder();

    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->listWidget->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->listWidget->clear();
    ui->actionNext->setEnabled(true);
    ui->actionPrevious->setEnabled(true);

    m_annotationList = new AnnotationList;
    connect(m_annotationList, SIGNAL(addListItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));
    connect(m_annotationList, SIGNAL(popListItem(int)), this, SLOT(popListItem(int)));

    m_paintBoard2d = new PaintBoard(this, m_image2d->getPixmap(), ui->textBrowser);
    m_paintBoard2d->registerAnnotationList(m_annotationList);
    ui->gridLayout->addWidget(m_paintBoard2d, 0, 0);
    connect(this, SIGNAL(chooseShape2d(int, QColor)), m_paintBoard2d, SLOT(drawShape(int, QColor)));
    connect(m_paintBoard2d, SIGNAL(addItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));

    QString filename(m_image2d->m_filename);
    filename += ".txt";
    m_annotationList->loadFromFile(filename);
    m_paintBoard2d->loadAnnotation();

    m_openStatus = 2;
}

// 打开3D图片
void MainWindow::selectFolder3D()
{
    if(m_openStatus != 0)
    {
        closeCurrent();
    }

    if(!m_image3d->selectFolder())
    {
        return;
    }

    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->listWidget->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->listWidget->clear();

    // 实例化数据
    m_annotationList = new AnnotationList;
    connect(m_annotationList, SIGNAL(addListItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));
    connect(m_annotationList, SIGNAL(popListItem(int)), this, SLOT(popListItem(int)));

    // 实例化交互面板
    m_interface3d = new Interface3D(this,
                                    m_image3d->getX(),
                                    m_image3d->getY(),
                                    m_image3d->getZ(),
                                    m_annotationList,
                                    ui->gridLayout);
    connect(this, SIGNAL(chooseShape3d(int, QColor)), m_interface3d, SLOT(drawShape(int, QColor)));
    connect(m_interface3d, SIGNAL(mousePosition(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_interface3d, SIGNAL(addItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));

    QString filename(m_image3d->m_dir);
    filename += "3d.txt";
    m_annotationList->loadFromFile(filename);
    m_interface3d->loadAnnotation();

    m_openStatus = 3;
}

// 关闭当前画板
void MainWindow::closeCurrent()
{
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->listWidget->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->listWidget->clear();

    switch (m_openStatus)
    {
    case 2:
        // 打开2D图片文件夹
        ui->actionNext->setEnabled(false);
        ui->actionPrevious->setEnabled(false);

    case 1:
        // 打开2D图片文件
        delete m_annotationList;
        delete m_paintBoard2d;
        break;

    case 3:
        delete m_interface3d;
        delete m_annotationList;
        break;
    }
    m_openStatus = 0;
}

void MainWindow::drawRect()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        QColor color = QColorDialog::getColor(Qt::black, this);
        emit chooseShape2d(1, color);
    }
    else if(m_openStatus == 3)
    {
        QColor color = QColorDialog::getColor(Qt::black, this);
        emit chooseShape3d(1, color);
    }
}

void MainWindow::drawAreaSquare()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        QColor color = QColorDialog::getColor(Qt::black, this);
        emit chooseShape2d(2, color);
    }
}

void MainWindow::drawAreaRound()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        QColor color = QColorDialog::getColor(Qt::black, this);
        emit chooseShape2d(3, color);
    }
}


void MainWindow::on_actionBack_triggered()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        // 2D 图片的情况
        if(m_annotationList->back())
        {
            m_paintBoard2d->loadAnnotation();
        }
    }
    else
    {
        // 3D 图片的情况
        if(m_annotationList->back())
        {
            m_interface3d->loadAnnotation();
        }
    }
}


void MainWindow::on_actionForward_triggered()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        // 2D 图片的情况
        if(m_annotationList->forward())
        {
            m_paintBoard2d->loadAnnotation();
        }
    }
    else
    {
        // 3D 图片的情况
        if(m_annotationList->forward())
        {
            m_interface3d->loadAnnotation();
        }
    }
}


void MainWindow::addListItem(std::shared_ptr<Annotation> x)
{
    AnnotationItem * item = new AnnotationItem;
    item->annotation = x;
    item->setTextColor(x->m_color);
    item->setText(x->m_objectType);
    if(x->m_annotationType == 1)
    {
        // 2D矩形检测标注
        item->setIcon(QIcon(":/icon/icon/bigsquare.ico"));
    }
    else if(x->m_annotationType == 2)
    {
        // 2D方形分割标注
        item->setIcon(QIcon(":/icon/icon/squarePaint.png"));
    }
    else if(x->m_annotationType == 3)
    {
        // 2D圆形分割标注
        item->setIcon(QIcon(":/icon/icon/roundPaint.png"));
    }
    else if(x->m_annotationType == 4)
    {
        // 3D矩形检测标注
        item->setIcon(QIcon(":/icon/icon/bigsquare.ico"));
    }
    qDebug()<<999;
    ui->listWidget->addItem(item);
}

void MainWindow::popListItem(int i)
{
    qDebug()<<9090;
    int num = ui->listWidget->count();
    if(i >= 0 && i < num)
    {
        auto item = ui->listWidget->item(i);
        delete item;
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->pushButton->setEnabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    std::shared_ptr<Annotation> recordItem = static_cast<AnnotationItem*>(currentItem)->annotation;
    m_annotationList->deleteAnnotation(recordItem);
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        m_paintBoard2d->loadAnnotation();
    }
    else
    {
        m_interface3d->loadAnnotation();
    }
    delete currentItem;
}

void MainWindow::on_actionPrevious_triggered()
{
    if(m_openStatus != 2)
    {
        return;
    }


    if(m_image2d->prevImg())
    {
        // TODO: 提示是否需要保存

        m_paintBoard2d->loadImage(m_image2d->getPixmap());
        m_annotationList->clear();

        ui->listWidget->clear();

        QString filename(m_image2d->m_filename);
        filename += ".txt";
        m_annotationList->loadFromFile(filename);
        m_paintBoard2d->loadAnnotation();
    }
}

void MainWindow::on_actionNext_triggered()
{
    if(m_openStatus != 2)
    {
        return;
    }

    if(m_image2d->nextImg())
    {
        // TODO: 提示是否需要保存

        m_paintBoard2d->loadImage(m_image2d->getPixmap());
        m_annotationList->clear();

        ui->listWidget->clear();

        QString filename(m_image2d->m_filename);
        filename += ".txt";
        m_annotationList->loadFromFile(filename);
        m_paintBoard2d->loadAnnotation();
    }
}

void MainWindow::updateMousePosition(QString s)
{
    ui->textBrowser->setText(s);
}

void MainWindow::on_actionSave_triggered()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        m_annotationList->saveAsFile(m_image2d->m_filename);
    }
    else if(m_openStatus == 3)
    {
        QString filename(m_image3d->m_dir);
        filename += "3d.txt";
        m_annotationList->saveAsFile(filename);
    }
}

void MainWindow::on_actionReadAnnotation_triggered()
{
    if(m_openStatus == 0)
    {
        return;
    }
    QString filename = QFileDialog::getOpenFileName(
                nullptr,
                "打开文件",
                "c:\\",
                "*.txt");
    m_annotationList->loadFromFile(filename);
}

void MainWindow::on_action_2_triggered()
{
    closeCurrent();
}

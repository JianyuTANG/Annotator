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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_image2d(nullptr),
    m_paintBoard2d(nullptr),
    m_drawType(0),
    m_leftPress(false),
    m_openStatus(0)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->listWidget->setVisible(false);
    ui->pushButton->setVisible(false);


    m_image2d = new Image2D(this);
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(selectFile()));
    connect(ui->actionFolder2D, SIGNAL(triggered()), this, SLOT(selectFolder()));

    connect(ui->actionRectangle, SIGNAL(triggered()), this, SLOT(drawRect()));
    connect(ui->actionRoundPaint, SIGNAL(triggered()), this, SLOT(drawAreaRound()));
    connect(ui->actionSquarePaint, SIGNAL(triggered()), this, SLOT(drawAreaSquare()));
    // connect(this, SIGNAL(chooseShape2d(int)), m_paintBoard2d, SLOT(drawShape(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(m_openStatus == 0)
        {
            // 未打开图片的情况下所有标注无效
            return;
        }
        m_leftPress = true;
        if(m_drawType == 1)
        {
            // 矩形框标注
            QRect tempRect;
            m_rects.push_back(tempRect);
            m_shapes.push_back(1);
            m_record.push_back(1);
            QRect& lastRect = m_rects.back();
            lastRect.setTopLeft(e->pos());
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_leftPress)
    {
        if(m_drawType == 1)
        {
            // 矩形框标注
            QRect& lastRect = m_rects.back();
            lastRect.setBottomRight(e->pos());
            m_leftPress = false;
            update();
        }
        m_drawType = 0;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

}

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
    connect(this, SIGNAL(chooseShape2d(int)), m_paintBoard2d, SLOT(drawShape(int)));
    connect(m_paintBoard2d, SIGNAL(addItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));

    m_openStatus = 1;
}

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
    connect(this, SIGNAL(chooseShape2d(int)), m_paintBoard2d, SLOT(drawShape(int)));
    connect(m_paintBoard2d, SIGNAL(addItem(std::shared_ptr<Annotation>)), this, SLOT(addListItem(std::shared_ptr<Annotation>)));

    m_openStatus = 2;
}

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

    m_annotationList = new AnnotationList;

    m_interface3d = new Interface3D(this,
                                    m_image3d->getX(),
                                    m_image3d->getY(),
                                    m_image3d->getZ(),
                                    m_annotationList,
                                    ui->gridLayout);

    m_openStatus = 3;
}

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
        break;
    }
}

void MainWindow::drawRect()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        emit chooseShape2d(1);
    }
}

void MainWindow::drawAreaSquare()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        emit chooseShape2d(2);
    }
}

void MainWindow::drawAreaRound()
{
    if(m_openStatus == 1 || m_openStatus == 2)
    {
        emit chooseShape2d(3);
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
}


void MainWindow::addListItem(std::shared_ptr<Annotation> x)
{
    AnnotationItem * item = new AnnotationItem;
    item->annotation = x;
    item->setText(x->m_objectType);
    if(x->m_annotationType == 1)
    {
        // 2D矩形检测标注
        item->setIcon(QIcon(":/icon/icon/bigsquare.ico"));
    }
    else if(x->m_annotationType == 2)
    {
        item->setIcon(QIcon(":/icon/icon/squarePaint.png"));
    }
    else if(x->m_annotationType == 3)
    {
        item->setIcon(QIcon(":/icon/icon/roundPaint.png"));
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
    m_paintBoard2d->loadAnnotation();
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
    }
}

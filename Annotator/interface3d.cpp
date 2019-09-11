#include "interface3d.h"
#include <QDebug>


Interface3D::Interface3D(QWidget *parent,
                         const std::vector<QPixmap*> *imgX,
                         const std::vector<QPixmap*> *imgY,
                         const std::vector<QPixmap*> *imgZ,
                         AnnotationList *ann,
                         QGridLayout *l):
    QWidget(parent),
    m_imageX(imgX),
    m_imageY(imgY),
    m_imageZ(imgZ),
    m_annotationList(ann),
    m_layout(l)
{
    m_x = new PaintBoard3D(this);
    m_x->loadImage((*m_imageX)[0]);
    connect(m_x, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_x, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    connect(m_x, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_x, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_x, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));

    m_y = new PaintBoard3D(this);
    m_y->loadImage((*m_imageY)[0]);
    connect(m_y, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_y, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    //connect(m_y, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_y, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_y, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));

    m_z = new PaintBoard3D(this);
    m_z->loadImage((*m_imageZ)[0]);
    connect(m_z, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_z, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    //connect(m_z, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_z, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_z, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));

    m_layout->addWidget(m_x, 0, 0);
    m_layout->addWidget(m_y, 0, 1);
    m_layout->addWidget(m_z, 1, 1);
}

void Interface3D::updateMousePosition(QString s)
{
    qDebug()<<2;
    emit mousePosition(s);
}

void Interface3D::drawShape(int i)
{
    switch (i)
    {
    case 1:
        m_x->setDrawType(1);
        m_y->setDrawType(1);
        m_z->setDrawType(1);
        break;
    }
}

void Interface3D::mainFinish()
{
    if(m_x->m_isMain)
    {
        QRect mainRect(m_x->getLastRect());

        m_y->setDrawType(-1);
        m_y->setTempRect(2, mainRect.x(), mainRect.x() + mainRect.width());

        m_z->setDrawType(-1);
        m_z->setTempRect(2, mainRect.y(), mainRect.y() + mainRect.height());
    }
    else if(m_y->m_isMain)
    {
        QRect mainRect(m_y->getLastRect());

        m_x->setDrawType(-1);
        m_x->setTempRect(2, mainRect.x(), mainRect.x() + mainRect.width());

        m_z->setDrawType(-1);
        m_z->setTempRect(1, mainRect.y(), mainRect.y() + mainRect.height());
    }
    else if(m_z->m_isMain)
    {
        QRect mainRect(m_z->getLastRect());

        m_y->setDrawType(-1);
        m_y->setTempRect(1, mainRect.y(), mainRect.y() + mainRect.height());

        m_x->setDrawType(-1);
        int height = (*m_imageX)[0]->height();
        m_x->setTempRect(1, height - mainRect.x() - mainRect.width(), height - mainRect.x());
    }
}

void Interface3D::finalFinish()
{
    if(!m_x->m_isMain)
    {
        auto y = m_y->getLastRect();
        auto z = m_z->getLastRect();

        int x = y.x();
        int width = y.width();
        int y1 = (*m_imageX)[0]->height() - z.x() - z.width();
        int height = z.width();

        QRect rectX(x, y1, width, height);
        m_x->addRect(rectX);
        m_x->m_direction = 0;
        m_x->update();
    }
    else if(!m_y->m_isMain)
    {
        auto x = m_x->getLastRect();
        auto z = m_z->getLastRect();

        int x1 = x.x();
        int width = x.width();
        int y = z.y();
        int height = z.height();

        QRect rectY(x1, y, width, height);
        m_y->addRect(rectY);
        m_y->m_direction = 0;
        m_y->update();
    }
    else if(!m_z->m_isMain)
    {
        auto x = m_x->getLastRect();
        auto y = m_y->getLastRect();

        int x1 = (*m_imageX)[0]->height() - x.x() - x.width();
        int width = x.height();
        int y1 = y.y();
        int height = y.height();

        QRect rectZ(x1, width, y1, height);
        m_z->addRect(rectZ);
        m_z->m_direction = 0;
        m_z->update();
    }
}

void Interface3D::confirmAnnotation(QString s)
{
    int i = m_x->getLastShape();

    switch(i)
    {
    case 1:
        std::shared_ptr<detect3d> temp = std::make_shared<detect3d>(m_x->getLastRect(),
                      m_y->getLastRect(),
                      m_z->getLastRect(),
                      s,
                      0,
                      4);
        m_annotationList->addAnnotation(temp);
        emit(addItem(temp));
        break;
    }
}

void Interface3D::deconfirmAnnotation()
{
    m_x->deconfirm();
    m_y->deconfirm();
    m_z->deconfirm();
}

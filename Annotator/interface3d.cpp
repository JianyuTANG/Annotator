#include "interface3d.h"


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
    m_y = new PaintBoard3D(this);
    m_y->loadImage((*m_imageY)[0]);
    connect(m_y, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_y, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    m_z = new PaintBoard3D(this);
    m_z->loadImage((*m_imageZ)[0]);
    connect(m_z, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_z, SIGNAL(finalFinish()), this, SLOT(finalFinish()));

    m_layout->addWidget(m_x, 0, 0);
    m_layout->addWidget(m_y, 0, 1);
    m_layout->addWidget(m_z, 1, 1);
}

void Interface3D::updateMousePosition(QString s)
{
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
}

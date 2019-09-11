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
    m_currentx(0),
    m_currenty(0),
    m_currentz(0),
    m_layout(l)
{
    m_x = new PaintBoard3D(this);
    m_x->loadImage((*m_imageX)[0]);
    connect(m_x, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_x, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    connect(m_x, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_x, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_x, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));
    connect(m_x, SIGNAL(changeImg()), this, SLOT(changeImgX()));

    m_y = new PaintBoard3D(this);
    m_y->loadImage((*m_imageY)[0]);
    connect(m_y, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_y, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    connect(m_y, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_y, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_y, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));
    connect(m_y, SIGNAL(changeImg()), this, SLOT(changeImgY()));

    m_z = new PaintBoard3D(this);
    m_z->loadImage((*m_imageZ)[0]);
    connect(m_z, SIGNAL(mainFinish()), this, SLOT(mainFinish()));
    connect(m_z, SIGNAL(finalFinish()), this, SLOT(finalFinish()));
    connect(m_z, SIGNAL(mousePos(QString)), this, SLOT(updateMousePosition(QString)));
    connect(m_z, SIGNAL(confirmAnnotation(QString)), this, SLOT(confirmAnnotation(QString)));
    connect(m_z, SIGNAL(deconfirmAnnotation()), this, SLOT(deconfirmAnnotation()));
    connect(m_z, SIGNAL(changeImg()), this, SLOT(changeImgZ()));

    m_layout->addWidget(m_x, 0, 0);
    m_layout->addWidget(m_y, 0, 1);
    m_layout->addWidget(m_z, 1, 1);
}

void Interface3D::loadAnnotation()
{
    m_x->clearData();
    m_y->clearData();
    m_z->clearData();
    const std::vector<std::shared_ptr<Annotation>>& list = m_annotationList->getList();
    for(auto t:list)
    {
        switch (t->m_annotationType)
        {
        case 4:
        {
            std::shared_ptr<detect3d> temp = std::static_pointer_cast<detect3d>(t);
            m_x->addRect(temp->m_x);
            m_y->addRect(temp->m_y);
            m_z->addRect(temp->m_z);
            m_x->addColor(temp->m_color);
            m_y->addColor(temp->m_color);
            m_z->addColor(temp->m_color);
            break;
        }
        }
    }
    m_x->update();
    m_y->update();
    m_z->update();
}

void Interface3D::updateMousePosition(QString s)
{
    emit mousePosition(s);
}

void Interface3D::drawShape(int i, QColor c)
{
    switch (i)
    {
    case 1:
        m_x->setDrawType(1);
        m_y->setDrawType(1);
        m_z->setDrawType(1);

        m_color = c;
        m_x->setColor(c);
        m_y->setColor(c);
        m_z->setColor(c);
        break;
    }
}

void Interface3D::changeImgX()
{
    if(m_currentx < m_imageX->size() - 1)
    {
        m_currentx ++;
    }
    else
    {
        m_currentx = 0;
    }
    m_x->loadImage((*m_imageX)[m_currentx]);
}

void Interface3D::changeImgY()
{
    if(m_currenty < m_imageY->size() - 1)
    {
        m_currenty ++;
    }
    else
    {
        m_currenty = 0;
    }
    m_y->loadImage((*m_imageY)[m_currenty]);
}

void Interface3D::changeImgZ()
{
    if(m_currentz < m_imageZ->size() - 1)
    {
        m_currentz ++;
    }
    else
    {
        m_currentz = 0;
    }
    m_z->loadImage((*m_imageZ)[m_currentz]);
}

void Interface3D::mainFinish()
{
    //qDebug()<<22222;
    if(m_x->m_isMain)
    {
        //qDebug()<<22222;
        QRect mainRect(m_x->getLastRect());

        m_y->setDrawType(-1);
        m_y->setTempRect(2, mainRect.x(), mainRect.x() + mainRect.width());

        m_z->setDrawType(-1);
        int height = (*m_imageX)[0]->height();
        m_z->setTempRect(2, height - mainRect.y() - mainRect.height(), height - mainRect.y());
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

    qDebug()<<33333;
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
        m_x->addColor(m_color);
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
        m_y->addColor(m_color);
        m_y->m_direction = 0;
        m_y->update();
    }
    else if(!m_z->m_isMain)
    {
        auto x = m_x->getLastRect();
        auto y = m_y->getLastRect();

        int x1 = (*m_imageX)[0]->height() - x.y() - x.height();
        int width = x.height();
        int y1 = y.y();
        int height = y.height();

        QRect rectZ(x1, y1, width, height);
        m_z->addRect(rectZ);
        m_z->addColor(m_color);
        m_z->m_direction = 0;
        m_z->update();
    }

    m_x->clearCurrentStatus();
    m_y->clearCurrentStatus();
    m_z->clearCurrentStatus();
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
                      m_color,
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

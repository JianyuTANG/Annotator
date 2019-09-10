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
    m_y = new PaintBoard3D(this);
    m_y->loadImage((*m_imageY)[0]);
    m_z = new PaintBoard3D(this);
    m_z->loadImage((*m_imageZ)[0]);

    m_layout->addWidget(m_x, 0, 0);
    m_layout->addWidget(m_y, 0, 1);
    m_layout->addWidget(m_z, 1, 1);
}

void Interface3D::updateMousePosition(QString s)
{
    emit mousePosition(s);
}

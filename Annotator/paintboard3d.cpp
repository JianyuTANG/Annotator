#include "paintboard3d.h"
#include <QPaintDevice>
#include <QPainter>

PaintBoard3D::PaintBoard3D(QWidget *parent) : QWidget(parent)
{

}

void PaintBoard3D::loadImage(QPixmap *p)
{
    m_image = p;
}

void PaintBoard3D::paintEvent(QPaintEvent *e)
{
    QPixmap pix(*m_image);
    QPainter p(&pix);
    unsigned int cursor[5] = {0};
    for(int i:m_shapes)
    {
        p.setPen(Qt::RoundCap);
        switch (i)
        {
        case 1:
        {
            QPen pen(p.pen());
            pen.setWidth(1);
            p.setPen(pen);
            p.drawRect(m_rects[cursor[i]++]);
            break;
        }
        case 2:
            p.setPen(Qt::SquareCap);
        case 3:
            QPen pen2(p.pen());
            pen2.setWidth(m_width);
            p.setPen(pen2);
            const std::vector<QPoint> &line = m_areas[cursor[2]++];
            int len = line.size();
            for(int j = 0; j < len - 1; j++)
            {
                p.drawLine(line[j], line[j + 1]);
            }
            break;
        }
    }
    p.end();
    p.begin(this);
    p.drawPixmap(0, 0, pix);
}

void PaintBoard3D::mousePressEvent(QMouseEvent *)
{

}

void PaintBoard3D::mouseReleaseEvent(QMouseEvent *)
{

}

void PaintBoard3D::mouseMoveEvent(QMouseEvent *)
{

}

void PaintBoard3D::changeColor()
{

}

void PaintBoard3D::annotate(QMouseEvent *)
{

}

#include "paintboard3d.h"
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>

PaintBoard3D::PaintBoard3D(QWidget *parent) :
    QWidget(parent),
    m_isMain(false),
    m_drawType(0),
    m_leftPress(false),
    m_direction(0)
{
    setMouseTracking(true);
}

void PaintBoard3D::loadImage(QPixmap *p)
{
    m_image = p;
}

void PaintBoard3D::setDrawType(const int i)
{
    m_drawType =i;
}

void PaintBoard3D::setTempRect(const int direction, const int x1, const int x2)
{
    m_direction = direction;
    m_x1 =x1;
    m_x2= x2;
}

const QRect &PaintBoard3D::getLastRect() const
{
    return m_rects.back();
}

int PaintBoard3D::getLastShape() const
{
    return m_shapes.back();
}

void PaintBoard3D::addRect(const QRect &r)
{
    m_shapes.push_back(1);
    m_rects.push_back(r);
}

void PaintBoard3D::deconfirm()
{
    m_shapes.pop_back();
    m_rects.pop_back();
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

    // 绘制检测任务临时框
    if(m_direction == 1)
    {
        QRect rect;
        rect.setTopLeft(QPoint(0, m_x1));
        rect.setBottomRight(QPoint(pix.width(), m_x2));
        p.drawRect(rect);
    }
    else if(m_direction == 2)
    {
        QRect rect;
        rect.setTopLeft(QPoint(m_x1, 0));
        rect.setBottomRight(QPoint(m_x2, pix.height()));
        p.drawRect(rect);
    }

    p.end();
    p.begin(this);
    p.drawPixmap(0, 0, pix);
}

void PaintBoard3D::mousePressEvent(QMouseEvent *e)
{
    int mouse_x = e->x();
    int mouse_y = e->y();
    if(!(mouse_x < m_image_x && mouse_y < m_image_y
            && mouse_x >= 0 && mouse_y >= 0))
    {
        return;
    }
    if(e->button() == Qt::LeftButton)
    {
        changeColor();
        m_leftPress = true;
        if(m_drawType == 1)
        {
            // 矩形框标注
            QRect tempRect;
            m_rects.push_back(tempRect);
            m_shapes.push_back(1);
            QRect& lastRect = m_rects.back();
            lastRect.setTopLeft(e->pos());
            m_isMain = true;
        }
        else if(m_drawType == -1)
        {
            if(m_isMain)
            {
                return;
            }
            // 矩形框标注
            QRect tempRect;
            m_rects.push_back(tempRect);
            m_shapes.push_back(1);
            QRect& lastRect = m_rects.back();
            lastRect.setTopLeft(e->pos());
        }
        else if(m_drawType == 2 || m_drawType == 3)
        {
            // 涂抹分割标注
            std::vector<QPoint> line;
            m_areas.push_back(line);
            std::vector<QPoint>& lastLine = m_areas.back();
            lastLine.push_back(e->pos());
            m_shapes.push_back(m_drawType);
        }
    }
}

void PaintBoard3D::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_leftPress)
    {
        if(m_drawType == 1)
        {
            int mouse_x = e->x();
            int mouse_y = e->y();
            if(!(mouse_x < m_image_x && mouse_y < m_image_y
                    && mouse_x >= 0 && mouse_y >= 0))
            {
                // 松开区域不在图片上，标注无效
                m_rects.pop_back();
                m_shapes.pop_back();
                m_leftPress = false;
                m_drawType = 0;
                update();
                return;
            }

            // 矩形框标注
            QRect& lastRect = m_rects.back();
            lastRect.setBottomRight(e->pos());
            m_leftPress = false;

            emit mainFinish();

            // 重绘
            update();
        }
        else if(m_drawType == -1)
        {
            int mouse_x = e->x();
            int mouse_y = e->y();
            if((!(mouse_x < m_image_x && mouse_y < m_image_y
                    && mouse_x >= 0 && mouse_y >= 0))
                || (!judgeCrossing(mouse_x, mouse_y)))
            {
                // 松开区域不合法，标注无效
                m_rects.pop_back();
                m_shapes.pop_back();
                m_leftPress = false;
                update();
                return;
            }


            // 矩形框标注
            QRect& lastRect = m_rects.back();
            lastRect.setBottomRight(e->pos());
            int tlx = lastRect.x();
            int tly = lastRect.y();
            int brx = lastRect.x() + lastRect.width();
            int bry = lastRect.y() + lastRect.height();
            m_rects.pop_back();
            getCrossing(tlx, tly, brx, bry);
            m_leftPress = false;
            m_drawType = 0;
            m_direction = 0;
            m_isMain = true;
            emit finalFinish();

            annotate(e);

            update();
        }
        else if(m_drawType == 2 || m_drawType == 3)
        {
            std::vector<QPoint>& lastLine = m_areas.back();
            lastLine.push_back(e->pos());
            m_leftPress = false;

            // 显示输入标注信息的输入框
            annotate(e);

            // 重绘
            update();
        }
    }
}

void PaintBoard3D::mouseMoveEvent(QMouseEvent *e)
{
    int mouse_x = e->x();
    int mouse_y = e->y();
    if(mouse_x < m_image_x && mouse_y < m_image_y
            && mouse_x >= 0 && mouse_y >= 0)
    {
        // m_mousePos->setText(QString("%1, %2").arg(mouse_x).arg(mouse_y));
        emit mousePos(QString("%1, %2").arg(mouse_x).arg(mouse_y));
    }
    else
    {
        // m_mousePos->setText("-");
        emit mousePos("-");
    }

    if(m_leftPress)
    {
        if(m_drawType == 1)
        {
            // 矩形框标注
            QRect& lastRect = m_rects.back();
            lastRect.setBottomRight(e->pos());
            update();
        }
        else if(m_drawType == 2 || m_drawType == 3)
        {
            // 涂抹分割标注
            if(m_areas.size() <= 0)
            {
                return;
            }
            std::vector<QPoint>& lastLine = m_areas.back();
            lastLine.push_back(e->pos());
            update();
        }
    }
}

void PaintBoard3D::getAnnotation()
{
    QString annotation = m_inputAnnotation.toPlainText();
    m_inputAnnotation.clear();
    m_inputAnnotation.setVisible(false);
    m_finishAnnotation.setVisible(false);
    m_cancelAnnotation.setVisible(false);

    emit confirmAnnotation(annotation);
}

void PaintBoard3D::cancelAnnotation()
{
    m_inputAnnotation.clear();
    m_inputAnnotation.setVisible(false);
    m_finishAnnotation.setVisible(false);
    m_cancelAnnotation.setVisible(false);

    emit deconfirmAnnotation();
}

bool PaintBoard3D::judgeCrossing(const int end_x, const int end_y)
{
    int start_x = m_rects.back().x();
    int start_y = m_rects.back().y();

    if(m_direction == 1)
    {
        // 横向
        if(end_y >= m_x2 && start_y <= m_x1)
        {
            return true;
        }
        if(start_y >= m_x2 && end_y <= m_x1)
        {
            return true;
        }
    }
    else if(m_direction == 2)
    {
        // 纵向
        if(end_x >= m_x2 && start_x <= m_x1)
        {
            return true;
        }
        if(start_x >= m_x2 && end_x <= m_x1)
        {
            return true;
        }
    }
    return false;
}

QRect PaintBoard3D::getCrossing(int start_x, int start_y, int end_x, int end_y)
{
    if(start_x > end_x)
    {
        int t = start_x;
        start_x = end_x;
        end_x = t;
    }
    if(start_y > end_y)
    {
        int t = start_y;
        start_y = end_y;
        end_y = t;
    }

    if(m_direction == 1)
    {
        // 横向
        start_y = m_x1;
        end_y = m_x2;
        QRect rect;
        rect.setTopLeft(QPoint(start_x, start_y));
        rect.setBottomRight(QPoint(end_x, end_y));
        m_rects.push_back(rect);
    }
    else if(m_direction == 2)
    {
        // 纵向
        start_x = m_x1;
        end_x = m_x2;
        QRect rect;
        rect.setTopLeft(QPoint(start_x, start_y));
        rect.setBottomRight(QPoint(end_x, end_y));
        m_rects.push_back(rect);
    }
}


void PaintBoard3D::changeColor()
{
}

void PaintBoard3D::annotate(QMouseEvent *e)
{
    m_inputAnnotation.setGeometry(e->pos().x(), e->pos().y(), 100, 50);
    m_finishAnnotation.setGeometry(e->pos().x(), e->pos().y() + 60, 40, 30);
    m_finishAnnotation.setText("确定");
    m_cancelAnnotation.setGeometry(e->pos().x() + 60, e->pos().y() + 60, 40, 30);
    m_cancelAnnotation.setText("取消");
    m_inputAnnotation.show();
    m_finishAnnotation.show();
    m_cancelAnnotation.show();
}

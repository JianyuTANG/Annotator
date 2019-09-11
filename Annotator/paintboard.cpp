#include "paintboard.h"
#include "annotation.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>


PaintBoard::PaintBoard(QWidget *parent, QPixmap *picture, QTextBrowser *showMousePos):
    QWidget(parent),
    m_leftPress(false),
    m_inputAnnotation(this),
    m_finishAnnotation(this),
    m_cancelAnnotation(this),
    m_image(picture),
    m_image_x(picture->size().width()),
    m_image_y(picture->size().height()),
    m_width(10),
    m_mousePos(showMousePos)
{

    // 初始相关不可见
    m_inputAnnotation.setVisible(false);
    m_finishAnnotation.setVisible(false);
    m_cancelAnnotation.setVisible(false);
    connect(&m_finishAnnotation, SIGNAL(clicked()), this, SLOT(getAnnotation()));
    connect(&m_cancelAnnotation, SIGNAL(clicked()), this, SLOT(cancelAnnotation()));

    setMouseTracking(true);
}

void PaintBoard::registerAnnotationList(AnnotationList * const x)
{
    m_annotationList = x;
}

void PaintBoard::loadAnnotation()
{
    m_rects.clear();
    m_areas.clear();
    m_shapes.clear();
    const std::vector<std::shared_ptr<Annotation>>& list = m_annotationList->getList();
    for(auto t: list)
    {
        if(t->m_annotationType == 1)
        {
            // 矩形检测标注
            std::shared_ptr<detect2d> temp = std::static_pointer_cast<detect2d>(t);
            QRect rect;
            rect.setCoords(temp->m_topLeftX,
                           temp->m_topLeftY,
                           temp->m_bottomRightX,
                           temp->m_bottomRightY
                           );
            m_rects.push_back(rect);
            m_shapes.push_back(1);
            m_colors.push_back(temp->m_color);
        }
        else if(t->m_annotationType == 2 || t->m_annotationType == 3)
        {
            // 分割标注
            std::shared_ptr<segmentation2d> temp = std::static_pointer_cast<segmentation2d>(t);
            std::vector<QPoint> line(temp->m_points);
            m_areas.push_back(line);
            m_shapes.push_back(t->m_annotationType);
            m_colors.push_back(temp->m_color);
        }
    }
    update();
}

void PaintBoard::loadImage(QPixmap *picture)
{
    m_image = picture;
    m_image_x = picture->width();
    m_image_y = picture->height();

    m_shapes.clear();
    m_rects.clear();
    m_areas.clear();

    m_leftPress = false;

    update();
}


void PaintBoard::paintEvent(QPaintEvent *)
{
    // 操作2d图片的情况
    QPixmap pix(*m_image);
    QPainter p(&pix);
    unsigned int cursor[5] = {0};
    unsigned int c = 0;
    for(int i:m_shapes)
    {
        p.setPen(Qt::RoundCap);
        p.setPen(m_colors[c]);
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
            p.setPen(m_colors[c]);
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
        c++;
    }
    p.end();
    p.begin(this);
    p.drawPixmap(0, 0, pix);
}

void PaintBoard::mousePressEvent(QMouseEvent *e)
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
        m_leftPress = true;
        if(m_drawType == 1)
        {
            // 矩形框标注
            QRect tempRect;
            m_rects.push_back(tempRect);
            m_shapes.push_back(1);
            m_colors.push_back(m_color);
            QRect& lastRect = m_rects.back();
            lastRect.setTopLeft(e->pos());
        }
        else if(m_drawType == 2 || m_drawType == 3)
        {
            // 涂抹分割标注
            std::vector<QPoint> line;
            m_areas.push_back(line);
            m_colors.push_back(m_color);
            std::vector<QPoint>& lastLine = m_areas.back();
            lastLine.push_back(e->pos());
            m_shapes.push_back(m_drawType);
        }
    }
}

void PaintBoard::mouseReleaseEvent(QMouseEvent *e)
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
                m_colors.pop_back();
                m_leftPress = false;
                m_drawType = 0;
                update();
                return;
            }

            // 矩形框标注
            QRect& lastRect = m_rects.back();
            lastRect.setBottomRight(e->pos());
            m_leftPress = false;

            // 显示输入标注信息的输入框
            annotate(e);

            // 重绘
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

void PaintBoard::mouseMoveEvent(QMouseEvent *e)
{
    int mouse_x = e->x();
    int mouse_y = e->y();
    if(mouse_x < m_image_x && mouse_y < m_image_y
            && mouse_x >= 0 && mouse_y >= 0)
    {
        m_mousePos->setText(QString("%1, %2").arg(mouse_x).arg(mouse_y));
    }
    else
    {
        m_mousePos->setText("-");
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

void PaintBoard::annotate(QMouseEvent *e)
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

void PaintBoard::drawShape(int i, QColor c)
{
    m_drawType = i;
    m_color = c;
}

void PaintBoard::setWidth(int)
{

}

void PaintBoard::getAnnotation()
{
    QString annotation = m_inputAnnotation.toPlainText();
    m_inputAnnotation.clear();
    m_inputAnnotation.setVisible(false);
    m_finishAnnotation.setVisible(false);
    m_cancelAnnotation.setVisible(false);

    if(m_drawType == 1)
    {
        QRect& lastRect = m_rects.back();
        std::shared_ptr<detect2d> temp = std::make_shared<detect2d>(lastRect.x(),
                                      lastRect.y(),
                                      lastRect.x() + lastRect.width(),
                                      lastRect.y() + lastRect.height(),
                                      annotation,
                                      m_color);
        m_annotationList->addAnnotation(temp);

        // 向主界面状态栏列表加入标注信息
        emit(addItem(temp));
    }
    else if(m_drawType == 2 || m_drawType == 3)
    {
        const std::vector<QPoint>& lastLine = m_areas.back();
        std::shared_ptr<segmentation2d> temp = std::make_shared<segmentation2d>(lastLine,
                                                  annotation,
                                                  m_color,
                                                  m_drawType);
        m_annotationList->addAnnotation(temp);

        // 向主界面状态栏列表加入标注信息
        emit(addItem(temp));
    }
    m_drawType = 0;

}

void PaintBoard::cancelAnnotation()
{
    m_inputAnnotation.clear();
    m_inputAnnotation.setVisible(false);
    m_finishAnnotation.setVisible(false);
    m_cancelAnnotation.setVisible(false);

    switch (m_drawType)
    {
    case 1:
        m_rects.pop_back();
        break;
    case 2:
    case 3:
        m_areas.pop_back();
        break;
    }

    m_shapes.pop_back();
    m_colors.pop_back();
    m_drawType = 0;
    update();
}

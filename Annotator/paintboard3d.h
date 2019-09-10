#ifndef PAINTBOARD3D_H
#define PAINTBOARD3D_H

#include "paintboard.h"
#include <QWidget>

class PaintBoard3D : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBoard3D(QWidget *parent = nullptr);

    void loadImage(QPixmap *p);

protected:
    void paintEvent(QPaintEvent *);          // 重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);     // 重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);   // 重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);      // 重写鼠标移动事件

signals:

public slots:

private:
    int m_drawType;       // 当前画的图像类型
    bool m_leftPress;     // 鼠标左键是否按下

    AnnotationList *m_annotationList;

    int m_width;
    int m_color;

    void changeColor();


    QTextEdit m_inputAnnotation;
    QPushButton m_finishAnnotation, m_cancelAnnotation;

    QPixmap *m_image;
    int m_image_x, m_image_y;

    QTextBrowser *m_mousePos;

    std::vector<int> m_shapes;
    std::vector<QRect> m_rects;
    std::vector<std::vector<QPoint>> m_areas;

    void annotate(QMouseEvent *);
};

#endif // PAINTBOARD3D_H

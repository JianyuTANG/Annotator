
#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include "annotationlist.h"
#include <QWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <vector>
#include <QPixmap>


class PaintBoard : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBoard(QWidget *parent = nullptr,
                        QPixmap *picture = nullptr,
                        QTextBrowser *showMousePos = nullptr);

    void registerAnnotationList(AnnotationList *const x);

    void loadAnnotation();

    void loadImage(QPixmap *picture);

protected:
    void paintEvent(QPaintEvent *);          // 重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);     // 重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);   // 重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);      // 重写鼠标移动事件

private:
    // 状态
    int m_drawType;       // 当前画的图像类型
    bool m_leftPress;     // 鼠标左键是否按下
    int m_width;
    QColor m_color;

    // 控件
    QTextEdit m_inputAnnotation;
    QPushButton m_finishAnnotation, m_cancelAnnotation;
    QTextBrowser *m_mousePos;

    // 数据
    QPixmap *m_image;
    int m_image_x, m_image_y;

    AnnotationList *m_annotationList;


    std::vector<int> m_shapes;
    std::vector<QRect> m_rects;
    std::vector<std::vector<QPoint>> m_areas;
    std::vector<QColor> m_colors;

    void annotate(QMouseEvent *);

signals:
    void addItem(std::shared_ptr<Annotation>);

public slots:
    void drawShape(int, QColor);

    void setWidth(int);  // 设置画笔宽度

private slots:
    void getAnnotation();
    void cancelAnnotation();
};

#endif // PAINTBOARD_H

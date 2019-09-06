#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include "annotationlist.h"
#include <QWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <vector>

class PaintBoard : public QWidget
{
    Q_OBJECT
public:
    explicit PaintBoard(QWidget *parent = nullptr,
                        QPixmap *picture = nullptr,
                        QTextBrowser *showMousePos = nullptr);

    void registerAnnotationList(AnnotationList *const x);

    void loadAnnotation();

protected:
    void paintEvent(QPaintEvent *);          // 重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);     // 重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);   // 重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);      // 重写鼠标移动事件

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

signals:
    void addItem(std::shared_ptr<Annotation>);

public slots:
    void drawShape(int);

    void setWidth(int);

private slots:
    void getAnnotation();
    void cancelAnnotation();
};

#endif // PAINTBOARD_H

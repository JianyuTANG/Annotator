#ifndef ANNOTATION_H
#define ANNOTATION_H
#include <QString>
#include <vector>
#include <QPoint>
#include <QRect>
#include <QColor>

class Annotation
{
public:
    Annotation(const int i, const QString& t, const QColor& c):
        m_annotationType(i),
        m_objectType(t),
        m_color(c) {}

    int m_annotationType;  // 标注类型
    QString m_objectType;  // 标注名称
    QColor m_color;        // 颜色

    virtual ~Annotation(){}

    virtual QString writeline() = 0; // 保存
};


// 2D标注
class detect2d:public Annotation
{
public:
    // 矩形框坐标
    int m_topLeftX, m_topLeftY, m_bottomRightX, m_bottomRightY;

    detect2d(const int tlx, const int tly, const int brx, const int bry,const QString& type, const QColor& c):
        Annotation(1, type, c),
        m_topLeftX(tlx),
        m_topLeftY(tly),
        m_bottomRightX(brx),
        m_bottomRightY(bry) {}

    detect2d(): Annotation (1, "", Qt::black) {}

    ~detect2d(){}

    QString writeline();
};


// 2d分割标注
class segmentation2d: public Annotation
{
public:
    // 点集
    std::vector<QPoint> m_points;

    segmentation2d(const std::vector<QPoint> &a, const QString& type, const QColor& c, const int shape):
        Annotation(shape, type, c),
        m_points(a) {}

    segmentation2d(): Annotation (2, "", Qt::black) {}

    ~segmentation2d() {}

    QString writeline();
};


// 3d检测标注
class detect3d: public Annotation
{
public:
    QRect m_x, m_y, m_z;

    detect3d(const QRect& x, const QRect& y, const QRect& z, const QString& type, const QColor& c, const int shape):
        Annotation(4, type, c),
        m_x(x),
        m_y(y),
        m_z(z) {}

    QString writeline();
};

class segmentation3d: public Annotation
{
public:
};

#endif // ANNOTATION_H

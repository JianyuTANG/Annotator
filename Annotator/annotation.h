#ifndef ANNOTATION_H
#define ANNOTATION_H
#include <QString>
#include <vector>
#include <QPoint>

class Annotation
{
public:
    Annotation(const int i, const QString& t):
        m_annotationType(i),
        m_objectType(t) {}

    int m_annotationType;
    QString m_objectType;

    virtual ~Annotation(){}

    //virtual void save()=0;
};



class detect2d:public Annotation
{
public:
    int m_topLeftX, m_topLeftY, m_bottomRightX, m_bottomRightY, m_color;

    detect2d(const int tlx, const int tly, const int brx, const int bry,const QString& type, const int c):
        Annotation(1, type),
        m_topLeftX(tlx),
        m_topLeftY(tly),
        m_bottomRightX(brx),
        m_bottomRightY(bry),
        m_color(c) {}

    detect2d(): Annotation (1, "") {}

    ~detect2d(){}
};

class segmentation2d: public Annotation
{
public:
    std::vector<QPoint> m_points;

    segmentation2d(const std::vector<QPoint> &a, const QString& type, const int c, const int shape):
        Annotation(shape, type),
        m_points(a) {}

    segmentation2d(): Annotation (2, "") {}

    ~segmentation2d() {}
};

#endif // ANNOTATION_H

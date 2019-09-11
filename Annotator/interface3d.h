#ifndef INTERFACE3D_H
#define INTERFACE3D_H

#include "paintBoard3d.h"
#include "image3d.h"
#include <QWidget>
#include <QGridLayout>

class Interface3D : public QWidget
{
    Q_OBJECT
public:
    explicit Interface3D(QWidget *parent = nullptr,
                         const std::vector<QPixmap*>* imgX = nullptr,
                         const std::vector<QPixmap*>* imgY = nullptr,
                         const std::vector<QPixmap*>* imgZ = nullptr,
                         AnnotationList *ann = nullptr,
                         QGridLayout *l = nullptr
            );

private:
    QGridLayout *m_layout;

    PaintBoard3D *m_x, *m_y, *m_z;

    const std::vector<QPixmap*> *m_imageX, *m_imageY, *m_imageZ;

    AnnotationList *m_annotationList;

signals:
    void mousePosition(QString);
    void finish();

public slots:
    void updateMousePosition(QString);

    void drawShape(int);

private slots:
    void mainFinish();
    void finalFinish();
};

#endif // INTERFACE3D_H

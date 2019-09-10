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
                         std::vector<QPixmap>* imgX = nullptr,
                         std::vector<QPixmap>* imgY = nullptr,
                         std::vector<QPixmap>* imgZ = nullptr,
                         AnnotationList *ann = nullptr,
                         QGridLayout *l = nullptr
            );

private:
    QGridLayout *m_layout;

    PaintBoard3D *m_x, *m_y, *m_z;

    std::vector<QPixmap> *m_imageX, *m_imageY, *m_imageZ;

    AnnotationList *m_annotationList;

signals:
    void mousePosition(QString);

public slots:
    void updateMousePosition(QString);
};

#endif // INTERFACE3D_H

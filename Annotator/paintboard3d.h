#ifndef PAINTBOARD3D_H
#define PAINTBOARD3D_H

#include "paintboard.h"
#include <QWidget>

class PaintBoard3D : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBoard3D(QWidget *parent = nullptr);

signals:

public slots:

private:
    PaintBoard m_x, m_y, m_z;
};

#endif // PAINTBOARD3D_H

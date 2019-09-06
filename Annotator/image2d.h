#ifndef IMAGE2D_H
#define IMAGE2D_H

#include <QObject>

class Image2D : public QObject
{
    Q_OBJECT
public:
    explicit Image2D(QObject *parent = nullptr);

    void selectFile();
    QPixmap* getPixmap();

signals:

public slots:


private:
    QString m_filename;
    QImage *m_image;
    QPixmap *m_imageForShow;


    void loadImage();
};

#endif // IMAGE2D_H

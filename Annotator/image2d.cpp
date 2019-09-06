#include "image2d.h"
#include <QFileDialog>

Image2D::Image2D(QObject *parent) :
    QObject(parent),
    m_image(nullptr),
    m_imageForShow(nullptr)
{

}

void Image2D::selectFile()
{
    QString filename = QFileDialog::getOpenFileName(
                nullptr,
                "打开文件",
                "c:\\",
                "Images(*.jpeg *.bmp *.png *.jpg)");
    m_filename = filename;

    loadImage();
}

QPixmap *Image2D::getPixmap()
{
    return m_imageForShow;
}

void Image2D::loadImage()
{
    if(m_image != nullptr)
    {
        delete m_image;
        delete m_imageForShow;
    }
    m_image = new QImage(m_filename);
    m_imageForShow = new QPixmap;
    *m_imageForShow = QPixmap::fromImage(*m_image);
}

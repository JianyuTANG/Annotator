#include "image2d.h"
#include <QFileDialog>
#include "io.h"
#include <QPixmap>
#include <QDebug>


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

void Image2D::selectFolder()
{
    m_dir = QFileDialog::getExistingDirectory(
                nullptr,
                tr("Open Directory"),
                "c:",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );


    std::string search = m_dir.toStdString() + "/*.jpg";
    const char *to_search = search.c_str();                      //在input文件夹下搜索jpg文件
    qDebug() << to_search;
    long handle;                                                //用于查找的句柄
    struct _finddata_t fileinfo;                                //存储文件信息的结构体
    handle = _findfirst(to_search, &fileinfo);                     //第一次查找
    if(handle == -1)
    {
        qDebug() << to_search;
        return;
    }
    QString head, file;
    head = m_dir + "/";
    file = fileinfo.name;
    QString t("777");
    qDebug() << head + file;
    m_filenameList.push_back(head + file);

    while(!_findnext(handle, &fileinfo))                         //查找其他符合的文件
    {
        file = fileinfo.name;
        m_filenameList.push_back(head + file);
        // printf("%s\n", fileinfo.name);
    }
    _findclose(handle);

    m_currentFile = 0;
    m_filename = m_filenameList[0];
    loadImage();
}

bool Image2D::nextImg()
{
    if(m_filenameList.size() == 1)
    {
        return false;
    }

    if(m_currentFile == m_filenameList.size() - 1)
    {
        m_currentFile = -1;
    }
    m_currentFile++;
    m_filename = m_filenameList[m_currentFile];
    loadImage();

    return true;
}

bool Image2D::prevImg()
{
    int len = m_filenameList.size();
    if(len == 1)
    {
        return false;
    }

    if(m_currentFile == 0)
    {
        m_currentFile = len;
    }

    m_currentFile--;
    m_filename = m_filenameList[m_currentFile];
    qDebug() << "clicked";
    loadImage();

    return true;
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
    qDebug() << "success loading";
}

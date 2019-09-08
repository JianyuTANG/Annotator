#include "image3d.h"
#include <QFileDialog>
#include "io.h"
#include <string>


Image3D::Image3D(QObject *parent) : QObject(parent)
{

}

void Image3D::selectFolder()
{
    m_dir = QFileDialog::getExistingDirectory(
                nullptr,
                tr("Open Directory"),
                "c:",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );

    QString head,file;
    std::string search = m_dir.toStdString() + "*.jpg";
    const char *to_search = search.c_str();                      //在input文件夹下搜索jpg文件
    long handle;                                                //用于查找的句柄
    struct _finddata_t fileinfo;                                //存储文件信息的结构体
    handle = _findfirst(to_search, &fileinfo);                     //第一次查找
    if(handle == -1)
        return;
    printf("%s\n",fileinfo.name);                               //打印出找到的文件的文件名
    head = "input\\";
    file = fileinfo.name;
    m_filenameList.push_back(head + file);
    while(!_findnext(handle,&fileinfo))                         //查找其他符合的文件
    {
        file = fileinfo.name;
        m_filenameList.push_back(head + file);
        printf("%s\n",fileinfo.name);
    }
    _findclose(handle);

    // 加载图片
    load_image();
}

const std::vector<QPixmap> &Image3D::getX() const
{
    return m_imageX;
}

const std::vector<QPixmap> &Image3D::getY() const
{
    return m_imageY;
}

const std::vector<QPixmap> &Image3D::getZ() const
{
    return m_imageZ;
}

void Image3D::load_image()
{
    // 加载X方向图片
    for(auto t:m_filenameList)
    {
        QImage temp(t);
        m_imageListX.push_back(temp);
        m_imageX.push_back(QPixmap::fromImage(temp));
    }

    // 计算Y方向图片
    int width = m_imageListX[0].width();
    int height = m_imageListX.size();
    auto format = m_imageListX[0].format();
    int len = m_imageListX[0].height();
    for(int i = 0; i < len; i++)
    {
        QImage temp(width, height, format);
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < height; k++)
            {
                temp.setPixel(j, k, m_imageListX[k].pixel(j, i));
            }
        }
        m_imageListY.push_back(temp);
        m_imageY.push_back(QPixmap::fromImage(temp));
    }

    // 计算Z方向图片
    width = m_imageListX[0].height();
    len = m_imageListX[0].width();
    for(int i = 0; i < len; i++)
    {
        QImage temp(width, height, format);
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < height; k++)
            {
                temp.setPixel(j, k, m_imageListX[j].pixel(i, k));
            }
        }
        m_imageListZ.push_back(temp);
        m_imageZ.push_back(QPixmap::fromImage(temp));
    }
}

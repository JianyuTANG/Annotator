#include "image3d.h"
#include <QFileDialog>
#include "io.h"
#include <string>


Image3D::Image3D(QObject *parent) : QObject(parent)
{

}

bool Image3D::selectFolder()
{
    m_dir = QFileDialog::getExistingDirectory(
                nullptr,
                tr("Open Directory"),
                "c:",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );

    std::string search = m_dir.toStdString() + "/*.jpg";
    const char *to_search = search.c_str();                      //在input文件夹下搜索jpg文件
    //qDebug() << to_search;
    long handle;                                                //用于查找的句柄
    struct _finddata_t fileinfo;                                //存储文件信息的结构体
    handle = _findfirst(to_search, &fileinfo);                     //第一次查找
    if(handle == -1)
    {
        //qDebug() << to_search;
        return false;
    }
    QString head, file;
    head = m_dir + "/";
    file = fileinfo.name;
    QString t("777");
    //qDebug() << head + file;
    m_filenameList.push_back(head + file);

    while(!_findnext(handle, &fileinfo))                         //查找其他符合的文件
    {
        file = fileinfo.name;
        m_filenameList.push_back(head + file);
        // printf("%s\n", fileinfo.name);
    }
    _findclose(handle);

    // 加载图片
    load_image();

    return true;
}

const std::vector<QPixmap*> *Image3D::getX() const
{
    return &m_imageX;
}

const std::vector<QPixmap*> *Image3D::getY() const
{
    return &m_imageY;
}

const std::vector<QPixmap*> *Image3D::getZ() const
{
    return &m_imageZ;
}

const position Image3D::fromXtoY(const int n, const int x, const int y) const
{
    position ans;
    ans.num = y;
    ans.x = x;
    ans.y = n;
    return ans;
}

const position Image3D::fromXtoZ(const int n, const int x, const int y) const
{
    position ans;
    ans.num = x;
    ans.x = m_imageListX[0].height() - 1 - y;
    ans.y = n;
    return ans;
}

const position Image3D::fromYtoX(const int n, const int x, const int y) const
{
    position ans;
    ans.num = y;
    ans.x = x;
    ans.y = n;
    return ans;
}

const position Image3D::fromYtoZ(const int n, const int x, const int y) const
{
    position ans;
    ans.num = x;
    ans.x = m_imageListX[0].height() - 1 - n;
    ans.y = y;
    return ans;
}

const position Image3D::fromZtoX(const int n, const int x, const int y) const
{
    position ans;
    ans.num = y;
    ans.x = n;
    ans.y = m_imageListX[0].height() - 1 - x;
    return ans;
}

const position Image3D::fromZtoY(const int n, const int x, const int y) const
{
    position ans;
    ans.num = m_imageListX[0].height() - 1 - x;
    ans.x = n;
    ans.y = y;
    return ans;
}

void Image3D::load_image()
{
    // 加载X方向图片
    for(auto t:m_filenameList)
    {
        QImage temp(t);
        m_imageListX.push_back(temp);
        QPixmap *tempPix = new QPixmap;
        *tempPix = QPixmap::fromImage(temp);
        m_imageX.push_back(tempPix);
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
        QPixmap *tempPix = new QPixmap;
        *tempPix = QPixmap::fromImage(temp);
        m_imageY.push_back(tempPix);
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
                temp.setPixel(j, k, m_imageListX[k].pixel(i, width - 1 - j));
            }
        }
        m_imageListZ.push_back(temp);
        QPixmap *tempPix = new QPixmap;
        *tempPix = QPixmap::fromImage(temp);
        m_imageZ.push_back(tempPix);
    }
}

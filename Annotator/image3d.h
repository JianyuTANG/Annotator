#ifndef IMAGE3D_H
#define IMAGE3D_H

#include <QObject>
#include <vector>


class Image3D : public QObject
{
    Q_OBJECT
public:
    explicit Image3D(QObject *parent = nullptr);

    void selectFolder();

    const std::vector<QPixmap>& getX() const;
    const std::vector<QPixmap>& getY() const;
    const std::vector<QPixmap>& getZ() const;


private:
    QString m_dir; // 路径
    std::vector<QString> m_filenameList;  // 图片文件列表

    std::vector<QImage> m_imageListX, m_imageListY, m_imageListZ;
    std::vector<QPixmap> m_imageX, m_imageY, m_imageZ;

    void load_image();

signals:

public slots:
};

#endif // IMAGE3D_H

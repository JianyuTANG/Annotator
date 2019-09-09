#ifndef IMAGE3D_H
#define IMAGE3D_H

#include <QObject>
#include <QPixmap>
#include <vector>


struct position
{
    int num;
    int x;
    int y;
};


class Image3D : public QObject
{
    Q_OBJECT
public:
    explicit Image3D(QObject *parent = nullptr);

    void selectFolder();

    const std::vector<QPixmap>& getX() const;
    const std::vector<QPixmap>& getY() const;
    const std::vector<QPixmap>& getZ() const;

    const position fromXtoY(const int n, const int x, const int y) const;
    const position fromXtoZ(const int n, const int x, const int y) const;
    const position fromYtoX(const int n, const int x, const int y) const;
    const position fromYtoZ(const int n, const int x, const int y) const;
    const position fromZtoX(const int n, const int x, const int y) const;
    const position fromZtoY(const int n, const int x, const int y) const;

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

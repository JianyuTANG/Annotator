#ifndef IMAGE2D_H
#define IMAGE2D_H

#include <QObject>

class Image2D : public QObject
{
    Q_OBJECT
public:
    explicit Image2D(QObject *parent = nullptr);

    // 打开单张图片
    void selectFile();

    //打开多张图片
    void selectFolder();
    bool nextImg();
    bool prevImg();

    QPixmap* getPixmap();

    // 当前显示图片
    QString m_filename;

signals:
    void clearList();

public slots:


private:
    // 图片列表
    int m_currentFile;
    QString m_dir; // 路径
    std::vector<QString> m_filenameList;  // 图片文件列表

    QImage *m_image;
    QPixmap *m_imageForShow;

    void loadImage();
};

#endif // IMAGE2D_H

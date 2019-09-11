#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image2d.h"
#include "image3d.h"
#include "paintboard.h"
#include "paintboard3d.h"
#include "interface3d.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QListWidgetItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);          // 重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);     // 重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);   // 重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);      // 重写鼠标移动事件

private:
    Ui::MainWindow *ui;

    Image2D *m_image2d;
    Image3D *m_image3d;

    AnnotationList *m_annotationList;

    QPixmap m_pic2d;
    PaintBoard *m_paintBoard2d;

    Interface3D *m_interface3d;

    int m_drawType;
    bool m_leftPress;
    int m_openStatus;

    std::vector<int> m_record;

    std::vector<int> m_shapes;
    std::vector<QRect> m_rects;

signals:
    void chooseShape2d(int);
    void chooseShape3d(int);

private slots:
    void selectFile();
    void selectFolder();
    void selectFolder3D();
    void closeCurrent();

    void drawRect();
    void drawAreaSquare();
    void drawAreaRound();
    void on_actionBack_triggered();
    void on_actionForward_triggered();

    void addListItem(std::shared_ptr<Annotation>);
    void popListItem(int);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

public slots:


};

#endif // MAINWINDOW_H

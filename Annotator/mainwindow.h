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


private:
    Ui::MainWindow *ui;

    Image2D *m_image2d;  // 图像
    Image3D *m_image3d;

    AnnotationList *m_annotationList;  // 数据

    QPixmap m_pic2d;
    PaintBoard *m_paintBoard2d;

    Interface3D *m_interface3d;

    int m_openStatus; // 状态

signals:
    // 选择绘制类型
    void chooseShape2d(int, QColor);
    void chooseShape3d(int, QColor);

private slots:
    // 文件操作
    void selectFile();
    void selectFolder();
    void selectFolder3D();
    void closeCurrent();

    // 响应工具栏画图操作
    void drawRect();
    void drawAreaSquare();
    void drawAreaRound();
    void on_actionBack_triggered();
    void on_actionForward_triggered();

    // 状态栏操作
    void addListItem(std::shared_ptr<Annotation>);
    void popListItem(int);
    void updateMousePosition(QString);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

    // 响应菜单事件
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();
    void on_actionSave_triggered();
    void on_actionReadAnnotation_triggered();
    void on_action_2_triggered();

public slots:


};

#endif // MAINWINDOW_H

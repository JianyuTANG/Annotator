#ifndef ANNOTATIONLIST_H
#define ANNOTATIONLIST_H

#include "annotation.h"
#include <QObject>
#include <vector>
#include <memory>
#include <QPixmap>



class AnnotationList: public QObject
{
    Q_OBJECT

public:
    AnnotationList();

    Annotation *addAnnotation(const int tlx, const int tly, const int brx, const int bry, const int c);

    // 增删
    void addAnnotation(std::shared_ptr<Annotation> const x);
    void deleteAnnotation(std::shared_ptr<Annotation> const x);

    // 前进后退
    bool back();
    bool forward();

    // 清楚
    void clear();

    // 数据接口
    const std::vector<std::shared_ptr<Annotation>>& getList();

    // 读写
    void saveAsFile(QString filename);
    void loadFromFile(QString filename);

signals:
    // 与主界面交互增删列表
    void popListItem(int);
    void addListItem(std::shared_ptr<Annotation>);


private:
    // 当前显示的记录
    std::vector<std::shared_ptr<Annotation>> m_list;

    std::vector<std::shared_ptr<Annotation>> m_record;
    std::vector<int> m_operRecord;

    std::vector<std::shared_ptr<Annotation>> m_undoRecord;
    std::vector<int> m_undoOperRecord;

    int m_currentState;
};

#endif // ANNOTATIONLIST_H

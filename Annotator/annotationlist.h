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

    Annotation * addAnnotation(const int tlx, const int tly, const int brx, const int bry, const int c);
    void addAnnotation(std::shared_ptr<Annotation> const x);
    void deleteAnnotation(std::shared_ptr<Annotation> const x);

    bool back();
    bool forward();

    void clear();

    const std::vector<std::shared_ptr<Annotation>>& getList();

signals:
    void popListItem(int);
    void addListItem(std::shared_ptr<Annotation>);


private:
    std::vector<std::shared_ptr<Annotation>> m_list;

    std::vector<std::shared_ptr<Annotation>> m_record;
    std::vector<int> m_operRecord;

    std::vector<std::shared_ptr<Annotation>> m_undoRecord;
    std::vector<int> m_undoOperRecord;

    int m_currentState;
};

#endif // ANNOTATIONLIST_H

#ifndef ANNOTATIONLIST_H
#define ANNOTATIONLIST_H

#include "annotation.h"
#include <QObject>
#include <vector>


class AnnotationList: public QObject
{
    Q_OBJECT

public:
    AnnotationList();

    Annotation * addAnnotation(const int tlx, const int tly, const int brx, const int bry, const int c);
    void addAnnotation(Annotation * const x);
    void deleteAnnotation(Annotation * const x);

    bool back();
    bool forward();

    const std::vector<Annotation *>& getList();

signals:
    void popListItem();
    void addListItem(Annotation*);


private:
    std::vector<Annotation *> m_list;

    std::vector<Annotation*> m_record;
    std::vector<int> m_operRecord;

    std::vector<Annotation *> m_undoRecord;
    std::vector<int> m_undoOperRecord;

    int m_currentState;
};

#endif // ANNOTATIONLIST_H

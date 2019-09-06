#ifndef ANNOTATIONITEM_H
#define ANNOTATIONITEM_H

#include "annotation.h"
#include <QWidget>
#include <QListWidgetItem>


class AnnotationItem : public QListWidgetItem
{
public:
    AnnotationItem():QListWidgetItem() {}

    Annotation * annotation;
};

#endif // ANNOTATIONITEM_H

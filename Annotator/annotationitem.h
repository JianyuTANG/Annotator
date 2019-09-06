#ifndef ANNOTATIONITEM_H
#define ANNOTATIONITEM_H

#include "annotation.h"
#include <QWidget>
#include <QListWidgetItem>
#include <memory>


class AnnotationItem : public QListWidgetItem
{
public:
    AnnotationItem():QListWidgetItem() {}

    std::shared_ptr<Annotation> annotation;
};

#endif // ANNOTATIONITEM_H

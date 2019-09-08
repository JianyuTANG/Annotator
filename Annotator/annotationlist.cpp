#include "annotationlist.h"
#include <algorithm>
#include <QDebug>
#include <QPixmap>


AnnotationList::AnnotationList():QObject (), m_currentState(-1) {}


Annotation *AnnotationList::addAnnotation(const int tlx, const int tly, const int brx, const int bry, const int c)
{

}

void AnnotationList::addAnnotation(std::shared_ptr<Annotation> const x)
{
    m_list.push_back(x);
    m_record.push_back(x);
    m_operRecord.push_back(1);
    m_undoRecord.clear();
    m_undoOperRecord.clear();
}

void AnnotationList::deleteAnnotation(std::shared_ptr<Annotation> const x)
{
    auto iter = std::find(m_list.begin(), m_list.end(), x);
    if(iter == m_list.end())
    {
        return;
    }
    m_list.erase(iter);
    m_record.push_back(x);
    m_operRecord.push_back(2);
    m_undoRecord.clear();
    m_undoOperRecord.clear();
}

bool AnnotationList::back()
{
    if(m_operRecord.size() == 0)
    {
        return false;
    }
    switch (m_operRecord.back())
    {
    case 1:
    {
        auto t = m_record.back();
        int len = m_list.size();
        int i = 0;
        for(; i < len; i++)
        {
            if(m_list[i] == t)
            {
                break;
            }
        }
        auto iter = std::find(m_list.begin(), m_list.end(), t);
        m_list.erase(iter);
        emit popListItem(i);
        break;
    }
    case 2:
    {
        auto x = m_record.back();
        m_list.push_back(x);
        emit addListItem(x);
        break;
    }
    }
    auto t = m_record.back();
    int op = m_operRecord.back();
    m_undoRecord.push_back(t);
    m_undoOperRecord.push_back(op);
    m_record.pop_back();
    m_operRecord.pop_back();
    return true;
}

bool AnnotationList::forward()
{
    if(m_undoRecord.size() == 0)
    {
        return false;
    }
    switch (m_undoOperRecord.back())
    {
    case 2:
    {
        auto t = m_undoRecord.back();
        int len = m_list.size();
        int i = 0;
        for(; i < len; i++)
        {
            if(m_list[i] == t)
            {
                break;
            }
        }
        auto iter = std::find(m_list.begin(), m_list.end(), t);
        m_list.erase(iter);
        emit popListItem(i);
        break;
    }
    case 1:
    {
        auto x = m_undoRecord.back();
        m_list.push_back(x);
        emit addListItem(x);
        break;
    }
    }
    m_record.push_back(m_undoRecord.back());
    m_operRecord.push_back(m_undoOperRecord.back());
    m_undoRecord.pop_back();
    m_undoOperRecord.pop_back();
    return true;
}

void AnnotationList::clear()
{
    m_currentState = -1;
    m_list.clear();
    m_record.clear();
    m_operRecord.clear();
    m_undoRecord.clear();
    m_undoOperRecord.clear();
}

const std::vector<std::shared_ptr<Annotation>> &AnnotationList::getList()
{
    return m_list;
}

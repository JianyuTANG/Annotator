#include "annotationlist.h"
#include <algorithm>
#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <string>
#include <sstream>


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

void AnnotationList::saveAsFile(QString filename)
{
    filename += ".txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QTextStream stream(&file);
    for(auto t:m_list)
    {
        stream << t->writeline() << "\n";
    }
    file.close();
}

void AnnotationList::loadFromFile(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull())
    {
        std::string t(line.toStdString());
        std::stringstream ss(t);
        int type;
        ss >> type;
        switch (type)
        {
        case 1:
        {
            int r, g, b;
            ss >> r >> g >> b;
            int tlx, tly, brx, bry;
            ss >> tlx >> tly >> brx >> bry;
            char name[200];
            ss >> name;
            std::shared_ptr<detect2d> temp = std::make_shared<detect2d>(tlx,
                                                                        tly,
                                                                        brx,
                                                                        bry,
                                                                        QString(name),
                                                                        QColor(r, g, b));
            m_list.push_back(temp);
            emit addListItem(temp);
            break;
        }
        case 2:
        {
            int r, g, b;
            ss >> r >> g >> b;
            std::vector<QPoint> qlist;
            int x, y;
            ss >> x;
            while(x != -1000)
            {
                ss >> y;
                QPoint p(x, y);
                qlist.push_back(p);
                ss >> x;
            }
            char name[200];
            ss >> name;
            std::shared_ptr<segmentation2d> temp = std::make_shared<segmentation2d>(qlist,
                                                                                    QString(name),
                                                                                    QColor(r, g, b),
                                                                                    2);
            m_list.push_back(temp);
            emit addListItem(temp);
            break;
        }
        case 3:
        {
            int r, g, b;
            ss >> r >> g >> b;
            std::vector<QPoint> qlist;
            int x, y;
            ss >> x;
            while(x != -1000)
            {
                ss >> y;
                QPoint p(x, y);
                qlist.push_back(p);
                ss >> x;
            }
            char name[200];
            ss >> name;
            std::shared_ptr<segmentation2d> temp = std::make_shared<segmentation2d>(qlist,
                                                                                    QString(name),
                                                                                    QColor(r, g, b),
                                                                                    3);
            m_list.push_back(temp);
            emit addListItem(temp);
            break;
        }
        case 4:
        {
            int r, g, b;
            ss >> r >> g >> b;
            QRect rect[3];
            for(int k = 0; k < 3; k++)
            {
                int x, y, w, h;
                ss >> x >> y >> w >> h;
                rect[k] = QRect(x, y, w, h);
            }
            char name[200];
            ss >> name;
            std::shared_ptr<detect3d> temp = std::make_shared<detect3d>(rect[0],
                          rect[1],
                          rect[2],
                          QString(name),
                          QColor(r, g, b),
                          4);
            m_list.push_back(temp);
            emit addListItem(temp);
            break;
        }
        }
        line = in.readLine();
    }
    file.close();

}

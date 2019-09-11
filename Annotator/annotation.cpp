#include "annotation.h"

QString detect2d::writeline()
{
    QString ans("1");
    ans += QString(" %1 %2 %3 ").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
    ans += QString("%1 %2 %3 %4 ").arg(m_topLeftX).arg(m_topLeftY).arg(m_bottomRightX).arg(m_bottomRightY);
    ans += m_objectType;
    return ans;
}


QString segmentation2d::writeline()
{
    QString ans(QString("%1").arg(m_annotationType));
    ans += QString(" %1 %2 %3 ").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
    for(auto t:m_points)
    {
        ans += QString("%1 %2 ").arg(t.x()).arg(t.y());
    }
    ans += "-1000 ";
    ans += m_objectType;
    return ans;
}


QString detect3d::writeline()
{
    QString ans("4");
    ans += QString(" %1 %2 %3 ").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
    ans += QString("%1 %2 %3 %4 ").arg(m_x.x()).arg(m_x.y()).arg(m_x.width()).arg(m_x.height());
    ans += QString("%1 %2 %3 %4 ").arg(m_y.x()).arg(m_y.y()).arg(m_y.width()).arg(m_y.height());
    ans += QString("%1 %2 %3 %4 ").arg(m_z.x()).arg(m_z.y()).arg(m_z.width()).arg(m_z.height());
    ans += m_objectType;
    return ans;
}

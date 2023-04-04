// Author: Kang Lin <kl222@126.com>

#include "PluginFreeRDP.h"
#include <QDebug>
#include <QApplication>
#include "RabbitCommonDir.h"
#include "ConnecterFreeRDP.h"
#include "winpr/wlog.h"

CPluginFreeRDP::CPluginFreeRDP(QObject *parent)
    : CPluginClient(parent),
      m_Logger("FreeRDP.Plugin")
{
    //qDebug(m_Logger) << "FreeRDP" << "build configure:" << freerdp_get_build_config() << "\n";
    qInfo(m_Logger) << "FreeRDP version:" << freerdp_get_version_string()
                   << "Build version:" << freerdp_get_build_revision();
    //WLog_SetLogLevel(WLog_GetRoot(), WLOG_TRACE);
}

CPluginFreeRDP::~CPluginFreeRDP()
{
    qDebug(m_Logger) << "CPluginFreeRDP::~CPluginFreeRDP()";
}

const QString CPluginFreeRDP::Name() const
{
    return "FreeRDP";
}

const QString CPluginFreeRDP::DisplayName() const
{
    return tr("Free remote desktop");
}

const QString CPluginFreeRDP::Description() const
{
    return tr("RDP(Windows remote desktop protol): Access remote desktops such as windows.");
}

const QString CPluginFreeRDP::Protocol() const
{
    return "RDP";
}

const QIcon CPluginFreeRDP::Icon() const
{
    return QIcon::fromTheme("windows");
}

CConnecter* CPluginFreeRDP::CreateConnecter(const QString &szProtol)
{
    if(Id() == szProtol)
    {   
        return new CConnecterFreeRDP(this);
    }
    return nullptr;
}

#include "ConnecterLibVNCServer.h"
#include "DlgSettingsLibVnc.h"
#include "ConnectLibVNCServer.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "LibVNCServer.Connecter")

CConnecterLibVNCServer::CConnecterLibVNCServer(CPluginClient *plugin)
    : CConnecterThread(plugin)
{
    qDebug(log) << __FUNCTION__;
}

CConnecterLibVNCServer::~CConnecterLibVNCServer()
{
    qDebug(log) << __FUNCTION__;
}

qint16 CConnecterLibVNCServer::Version()
{
    return 0;
}

int CConnecterLibVNCServer::Initial()
{
    qDebug(log) << __FUNCTION__;
    int nRet = 0;
    nRet = CConnecterThread::Initial();
    if(nRet) return nRet;
    nRet = SetParameter(&m_Para);
    return nRet;
}

int CConnecterLibVNCServer::Clean()
{
    qDebug(log) << __FUNCTION__;
    int nRet = 0;
    nRet = CConnecterThread::Clean();
    return nRet;
}

QDialog *CConnecterLibVNCServer::OnOpenDialogSettings(QWidget *parent)
{
    return new CDlgSettingsLibVnc(&m_Para, parent);
}

CConnect* CConnecterLibVNCServer::InstanceConnect()
{
    CConnectLibVNCServer* p = new CConnectLibVNCServer(this);
    return p;
}

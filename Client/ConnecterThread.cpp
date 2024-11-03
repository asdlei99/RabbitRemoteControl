// Author: Kang Lin <kl222@126.com>

#include <QLoggingCategory>
#include <QDesktopServices>
#include <QUrl>

#include <QWidgetAction>
#include <QActionGroup>

#include "ConnecterThread.h"
#include "ConnectThread.h"
#include "PluginClient.h"

static Q_LOGGING_CATEGORY(log, "Client.Connecter.Thread")

CConnecterThread::CConnecterThread(CPluginClient *plugin)
    : CConnecterConnect(plugin)
    , m_pThread(nullptr)
    , m_pView(nullptr)
    , m_pScroll(nullptr)
    , m_psbZoomFactor(nullptr)
{
    bool check = false;
    qDebug(log) << __FUNCTION__;

    m_pView = new CFrmViewer();
    m_pScroll = new CFrmScroll(m_pView);

    QMenu* pMenuZoom = new QMenu(&m_Menu);
    pMenuZoom->setTitle(tr("Zoom"));
    pMenuZoom->setIcon(QIcon::fromTheme("zoom"));
    m_pMenuZoom = m_Menu.addMenu(pMenuZoom);
    m_pZoomToWindow = pMenuZoom->addAction(
        QIcon::fromTheme("zoom-fit-best"), tr("Zoom to window"));
    m_pZoomToWindow->setCheckable(true);
    check = connect(m_pZoomToWindow, &QAction::toggled, this,
                    [&](){
                        m_pScroll->slotSetAdaptWindows(
                            CFrmViewer::ADAPT_WINDOWS::ZoomToWindow);
                    });
    Q_ASSERT(check);
    m_pZoomAspectRatio = pMenuZoom->addAction(
        QIcon::fromTheme("zoom-aspect-ratio"),
        tr("Keep aspect ration to windows"));
    m_pZoomAspectRatio->setCheckable(true);
    check = connect(m_pZoomAspectRatio, &QAction::toggled, this,
                    [&](){
                        m_pScroll->slotSetAdaptWindows(
                            CFrmViewer::ADAPT_WINDOWS::KeepAspectRationToWindow);
                    });
    Q_ASSERT(check);
    m_pZoomOriginal = pMenuZoom->addAction(
        QIcon::fromTheme("zoom-original"), tr("Original"));
    m_pZoomOriginal->setCheckable(true);
    check = connect(m_pZoomOriginal, &QAction::toggled, this,
                    [&](){
                        m_pScroll->slotSetAdaptWindows(
                            CFrmViewer::ADAPT_WINDOWS::Original);
                    });
    Q_ASSERT(check);
    m_pZoomIn = pMenuZoom->addAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"));
    m_pZoomIn->setCheckable(true);
    check = connect(
        m_pZoomIn, &QAction::toggled, this,
        [&](){
            if(m_psbZoomFactor)
                m_psbZoomFactor->setValue((m_pView->GetZoomFactor() + 0.1) * 100);
        });
    Q_ASSERT(check);
    m_pZoomOut = pMenuZoom->addAction(
        QIcon::fromTheme("zoom-out"), tr("Zoom out"));
    m_pZoomOut->setCheckable(true);
    check = connect(
        m_pZoomOut, &QAction::toggled, this,
        [&](){
            if(m_psbZoomFactor)
                m_psbZoomFactor->setValue((m_pView->GetZoomFactor() - 0.1) * 100);
        });
    Q_ASSERT(check);
    QActionGroup* pGBViewZoom = new QActionGroup(this);
    if(pGBViewZoom) {
        pGBViewZoom->addAction(m_pZoomToWindow);
        pGBViewZoom->addAction(m_pZoomAspectRatio);
        pGBViewZoom->addAction(m_pZoomOriginal);
        pGBViewZoom->addAction(m_pZoomIn);
        pGBViewZoom->addAction(m_pZoomOut);
    }
    m_psbZoomFactor = new QSpinBox(pMenuZoom);
    m_psbZoomFactor->setRange(0, 9999999);
    m_psbZoomFactor->setValue(100);
    m_psbZoomFactor->setSuffix("%");
    m_psbZoomFactor->setEnabled(false);
    m_psbZoomFactor->setFocusPolicy(Qt::NoFocus);
    check = connect(
        m_psbZoomFactor, SIGNAL(valueChanged(int)),
        this, SLOT(slotValueChanged(int)));
    Q_ASSERT(check);
    QWidgetAction* pFactor = new QWidgetAction(pMenuZoom);
    pFactor->setDefaultWidget(m_psbZoomFactor);
    pMenuZoom->insertAction(m_pZoomOut, pFactor);

    m_Menu.addSeparator();
    m_pScreenShot =  new QAction(QIcon::fromTheme("camera-photo"),
                                tr("ScreenShot"), this);
    check = connect(
        m_pScreenShot, &QAction::triggered,
        this, [&](){
            if(!GetParameter() || !m_pView)
                return;
            auto &record = GetParameter()->m_Record;
            QString szFile = record.GetImageFile(true);
            bool bRet = m_pView->GrabImage().save(szFile);
            if(bRet)
                qDebug(log) << "Success: save screenshot to" << szFile;
            else
                qCritical(log) << "Fail: save screenshot to" << szFile;
            if(record.GetEnable() != CParameterRecord::ENDACTION::No)
                QDesktopServices::openUrl(QUrl::fromLocalFile(szFile));
        });
    Q_ASSERT(check);
    m_Menu.addAction(m_pScreenShot);
#if HAVE_QT6_RECORD
    m_pRecord = new QAction(QIcon::fromTheme("media-record"), tr("Record"), this);
    m_pRecord->setCheckable(true);
    check = connect(m_pRecord, SIGNAL(toggled(bool)),
                    this, SLOT(slotRecord(bool)));
    Q_ASSERT(check);
    m_Menu.addAction(m_pRecord);
#endif

    m_Menu.addSeparator();
    m_Menu.addAction(m_pSettings);
}

CConnecterThread::~CConnecterThread()
{
    qDebug(log) << __FUNCTION__;
    if(m_pScroll)
    {
        delete m_pScroll;
        m_pScroll = nullptr;
    }
}

QWidget *CConnecterThread::GetViewer()
{
    return m_pScroll;
}

int CConnecterThread::Connect()
{
    qDebug(log) << __FUNCTION__;
    int nRet = 0;
    m_pThread = new CConnectThread(this);
    if(!m_pThread) {
        qCritical(log) << "new CConnectThread fail";
        return -2;
    }
    
    m_pThread->start();
    
    return nRet;
}

int CConnecterThread::DisConnect()
{
    qDebug(log) << __FUNCTION__;
    int nRet = 0;
    if(m_pThread)
    {
        m_pThread->quit();
        //Don't delete m_pThread, See CConnectThread
        m_pThread = nullptr;
    }
    return nRet;
}

QString CConnecterThread::ServerName()
{
    if(GetParameter())
        if(!GetParameter()->GetShowServerName()
            || CConnecter::ServerName().isEmpty())
        {
            if(!GetParameter()->m_Net.GetHost().isEmpty())
                return GetParameter()->m_Net.GetHost() + ":"
                       + QString::number(GetParameter()->m_Net.GetPort());
        }
    return CConnecter::ServerName();
}

int CConnecterThread::Load(QSettings &set)
{
    int nRet = 0;
    Q_ASSERT(m_pView);
    nRet = CConnecterConnect::Load(set);
    if(m_pView && GetParameter())
    {
        m_pView->slotSetZoomFactor(GetParameter()->GetZoomFactor());
        if(m_psbZoomFactor)
            m_psbZoomFactor->setValue(m_pView->GetZoomFactor() * 100);
        CFrmViewer::ADAPT_WINDOWS aw = GetParameter()->GetAdaptWindows();
        if(CFrmViewer::ADAPT_WINDOWS::ZoomToWindow == aw)
            m_pZoomToWindow->setChecked(true);
        else if(CFrmViewer::ADAPT_WINDOWS::KeepAspectRationToWindow == aw)
            m_pZoomAspectRatio->setChecked(true);
        else if(CFrmViewer::ADAPT_WINDOWS::Original == aw)
            m_pZoomOriginal->setChecked(true);
        else if(CFrmViewer::ADAPT_WINDOWS::Zoom == aw)
            m_pZoomIn->setChecked(true);
    }
    return nRet;
}

int CConnecterThread::Save(QSettings &set)
{
    int nRet = 0;
    Q_ASSERT(GetParameter());
    if(GetParameter() && m_pView)
    {
        GetParameter()->SetAdaptWindows(m_pView->GetAdaptWindows());
        GetParameter()->SetZoomFactor(m_pView->GetZoomFactor());
    }
    nRet = CConnecterConnect::Save(set);
    return nRet;
}

void CConnecterThread::slotRecord(bool checked)
{
    qDebug(log) << __FUNCTION__ << checked;
    QAction* pRecord = qobject_cast<QAction*>(sender());
    if(pRecord)
    {
        if(checked) {
            pRecord->setIcon(QIcon::fromTheme("media-playback-stop"));
            pRecord->setText(tr("Stop record"));
        }
        else {
            pRecord->setIcon(QIcon::fromTheme("media-playback-start"));
            pRecord->setText(tr("Start record"));
        }
        emit sigRecord(checked);
    }
}

#if HAVE_QT6_RECORD
void CConnecterThread::slotRecorderStateChanged(
    QMediaRecorder::RecorderState state)
{
    if(QMediaRecorder::StoppedState == state)
        slotRecord(false);
}
#endif

void CConnecterThread::slotValueChanged(int v)
{
    if(!m_pScroll || !m_pView) return;
    m_pView->slotSetZoomFactor(((double)v) / 100);
    m_pScroll->slotSetAdaptWindows(CFrmViewer::ADAPT_WINDOWS::Zoom);
}

// Author: Kang Lin <kl222@126.com>

#ifndef CCONNECTERBACKTHREAD_H
#define CCONNECTERBACKTHREAD_H

#pragma once

#include "Connect.h"
#include "Parameter.h"
#include "FrmViewer.h"

class CConnectThread;

/**
 * \~english
 * \brief it is implement a background connect thread of the remote desktop.
 * \note The interface only is implemented by plugin
 * \details 
 *   1. Implement \ref InstanceConnect()
 *   2. Implement \ref GetDialogSettings
 *   3. Implement \ref OnLoad(QDataStream& d) and \ref OnSave(QDataStream& d)
 * \see CConnectThread
 */
class VIEWER_EXPORT CConnecterDesktop : public CConnecter
{
    Q_OBJECT
public:
    explicit CConnecterDesktop(CPluginViewer *parent = nullptr);
    virtual ~CConnecterDesktop();
    
    /// \~english New connect. the ownership is caller.
    ///  if don't use, the caller must delete it.
    virtual CConnect* InstanceConnect() = 0;
    
    virtual const QString Name();
    virtual QWidget* GetViewer() override;
    virtual QString ServerName() override;
    virtual int Load(QDataStream &d) override;
    virtual int Save(QDataStream &d) override;
    virtual int OpenDialogSettings(QWidget *parent) override;

public Q_SLOTS:
    virtual int Connect() override;
    virtual int DisConnect() override;

protected:
    virtual int OnLoad(QDataStream& d);
    virtual int OnSave(QDataStream& d);
    /**
     * \~english
     * \brief GetDialogSettings
     * \param parent: the parent windows of the dialog of return
     * \return QDialog*: then QDialog must set attribute Qt::WA_DeleteOnClose;
     *         The ownership is caller.
     */
    virtual QDialog* GetDialogSettings(QWidget* parent = nullptr) = 0;

private:
    CConnectThread* m_pThread;
    CFrmViewer *m_pView;

protected:
    CParameter* m_pParameter;
};

#endif // CCONNECTERBACKTHREAD_H
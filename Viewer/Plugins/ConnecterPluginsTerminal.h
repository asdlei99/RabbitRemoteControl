#ifndef CCONNECTERPLUGINSTERMINAL_H
#define CCONNECTERPLUGINSTERMINAL_H

#include "ConnectThread.h"
#include "Connecter.h"
#include "qtermwidget.h"
#include "ParameterTerminalAppearance.h"

class CConnectThreadTerminal;
class CConnecterPluginsTerminal : public CConnecter
{
    Q_OBJECT
public:
    explicit CConnecterPluginsTerminal(CPluginFactory *parent);
    virtual ~CConnecterPluginsTerminal();

    // Instance connect
    virtual CConnect* InstanceConnect();
    int OnRun();
    
    QWidget* GetViewer() override;
    virtual qint16 Version() override;
    virtual int Load(QDataStream &d) override;
    virtual int Save(QDataStream &d) override;
    /**
     * @brief Open settings dialog
     * @param parent
     * @return DialogCode
     *   QDialog::Accepted
     *   QDialog::Rejected
     *   -1: error
     */
    virtual int OpenDialogSettings(QWidget* parent = nullptr) override;
    virtual QString GetServerName() override;

public Q_SLOTS:
    virtual int Connect() override;
    virtual int DisConnect() override;

private Q_SLOTS:
    void slotTerminalTitleChanged();

protected:
    virtual QDialog* GetDialogSettings(QWidget *parent) = 0;
    virtual int SetParamter();

    virtual int OnConnect();
    virtual int OnDisConnect();
    virtual int OnLoad(QDataStream& d);
    virtual int OnSave(QDataStream& d);
    
    virtual CParameterTerminalAppearance* GetPara() = 0;
    
    QTermWidget* m_pConsole;
    CConnectThreadTerminal* m_pThread;
    bool m_bThread;
    bool m_bExit;
};

#endif // CCONNECTERPLUGINSTERMINAL_H

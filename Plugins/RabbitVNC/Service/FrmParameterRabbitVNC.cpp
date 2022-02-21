#include "FrmParameterRabbitVNC.h"
#include "ui_FrmParameterRabbitVNC.h"

CFrmParameterRabbitVNC::CFrmParameterRabbitVNC(CParameterServiceRabbitVNC *para) :
    QWidget(nullptr),
    ui(new Ui::CFrmParameterRabbitVNC),
    m_pParameters(para)
{
    ui->setupUi(this);
}

CFrmParameterRabbitVNC::~CFrmParameterRabbitVNC()
{
    delete ui;
}

void CFrmParameterRabbitVNC::slotSave()
{
    m_pParameters->setEnable(ui->cbEnable->isChecked());
    m_pParameters->setPort(ui->sbPort->value());
    m_pParameters->setPassword(ui->lePassword->text());
    
    m_pParameters->setIce(ui->cbEnableICE->isChecked());
    
    m_pParameters->setSignalServer(ui->leSignalServer->text());
    m_pParameters->setSignalPort(ui->sbSignalPort->value());
    m_pParameters->setSignalUser(ui->leSignalName->text());
    m_pParameters->setSignalPassword(ui->leSignalPassword->text());
    
    m_pParameters->setStunServer(ui->leStunServer->text());
    m_pParameters->setStunPort(ui->sbStunPort->value());
    
    m_pParameters->setTurnServer(ui->leTurnServer->text());
    m_pParameters->setTurnPassword(ui->leTrunPassword->text());
    m_pParameters->setTurnUser(ui->leTurnUser->text());
    m_pParameters->setTurnPassword(ui->leTrunPassword->text());
}

void CFrmParameterRabbitVNC::showEvent(QShowEvent *event)
{
    ui->cbEnable->setChecked(m_pParameters->getEnable());
    ui->sbPort->setValue(m_pParameters->getPort());
    ui->lePassword->setText(m_pParameters->getPassword());
    
    ui->cbEnableICE->setChecked(m_pParameters->getIce());
    
    ui->leSignalServer->setText(m_pParameters->getSignalServer());
    ui->sbSignalPort->setValue(m_pParameters->getSignalPort());
    ui->leSignalName->setText(m_pParameters->getSignalUser());
    ui->leSignalPassword->setText(m_pParameters->getSignalPassword());
    
    ui->leStunServer->setText(m_pParameters->getStunServer());
    ui->sbStunPort->setValue(m_pParameters->getStunPort());
    
    ui->leTurnServer->setText(m_pParameters->getTurnServer());
    ui->sbTurnPort->setValue(m_pParameters->getTurnPort());
    ui->leTurnUser->setText(m_pParameters->getTurnUser());
    ui->leTrunPassword->setText(m_pParameters->getTurnPassword());
}

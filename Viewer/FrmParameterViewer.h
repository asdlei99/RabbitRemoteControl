#ifndef FRMPARAMETERVIEWER_H
#define FRMPARAMETERVIEWER_H

#include <QWidget>
#include "ParameterViewer.h"

namespace Ui {
class CFrmParameterViewer;
}

class CFrmParameterViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmParameterViewer(CParameterViewer* pParameter,
                                 QWidget *parent = nullptr);
    virtual ~CFrmParameterViewer();

    //! [Accept parameters]
public Q_SLOTS:
    /*!
     * \brief Accept parameters
     */
    void slotAccept();
    //! [Accept parameters]

    void on_cbEnableViewPassword_clicked(bool checked);
    void on_pbEncryptKey_clicked();

private:
    Ui::CFrmParameterViewer *ui;
    CParameterViewer* m_pPara;
};

#endif // FRMPARAMETERVIEWER_H

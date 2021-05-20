#pragma once

#include "models.h"
#include <QLineEdit>
#include <QWidget>

class CaseWidget : public QWidget {
    Q_OBJECT
public:
    CaseWidget(CaseModel* model, QWidget* parent = nullptr);
    void nextCase();
    void setUserName(const QString& user);
    //QString currentCase() { return m_lineEdit->text(); }
    void setRole(const QString& role);
signals:
    void caseChanged(const QString& casename);

protected:
    void copyToClipBoard();

private:
    CaseModel* m_caseModel = nullptr;
    QLineEdit* m_lineEdit = nullptr;
    QString m_username;
    QString m_currentRole;
};

#pragma once

#include "casewidget.h"
#include "models.h"
#include <QWidget>

class InfoWidget : public QWidget {
    Q_OBJECT
public:
    InfoWidget(QuestionModel* questionModel, CaseModel* casemodel, QWidget* parent = nullptr);
    void requestNextCase() { m_caseWidget->nextCase(); };
signals:
    void userNameChanged(const QString& name);
    void roleChanged(const QString& role);
    void caseChanged(const QString& casename);

private:
    QuestionModel* m_questionModel = nullptr;
    CaseWidget* m_caseWidget = nullptr;
};

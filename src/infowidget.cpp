

#include "infowidget.h"

#include "casewidget.h"
#include "rolewidget.h"
#include "userwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

InfoWidget::InfoWidget(QuestionModel* questionModel, CaseModel* caseModel, QWidget* parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout;
    setLayout(mainLayout);
    auto ucLayout = new QVBoxLayout;
    mainLayout->addLayout(ucLayout);

    auto user_widget = new UserWidget(this);
    ucLayout->addWidget(user_widget);
    connect(user_widget, &UserWidget::usernameChanged, this, &InfoWidget::userNameChanged);

    m_caseWidget = new CaseWidget(caseModel, this);
    ucLayout->addWidget(m_caseWidget);
    connect(m_caseWidget, &CaseWidget::caseChanged, this, &InfoWidget::caseChanged);
    connect(user_widget, &UserWidget::usernameChanged, m_caseWidget, &CaseWidget::setUserName);

    auto roleWidget = new RoleWidget(questionModel, this);
    mainLayout->addWidget(roleWidget);
    connect(roleWidget, &RoleWidget::roleChanged, m_caseWidget, &CaseWidget::setRole);
    connect(roleWidget, &RoleWidget::roleChanged, this, &InfoWidget::roleChanged);
}

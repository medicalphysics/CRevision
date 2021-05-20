
#include "userwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

#include <Lmcons.h>
#include <windows.h>

UserWidget::UserWidget(QWidget* parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout;

    auto nameLayout = new QHBoxLayout;
    auto nameLabel = new QLabel(tr("Navn:"), this);
    m_nameEdit = new QLineEdit(this);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(m_nameEdit);
    mainLayout->addLayout(nameLayout);

    connect(m_nameEdit, &QLineEdit::textChanged, this, &UserWidget::usernameChanged);
    setLayout(mainLayout);

    QTimer::singleShot(0, this, &UserWidget::findUsername);
}

QString findWinUsername(void)
{
    char acUserName[256];
    DWORD nUserName = sizeof(acUserName);
    QString name;
    if (GetUserNameA(acUserName, &nUserName))
        name = QString(acUserName);
    return name;
}

void UserWidget::findUsername()
{
    QString uname = findWinUsername();
    m_nameEdit->setText(uname);
}

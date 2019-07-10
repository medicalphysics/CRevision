
#include "userinfowidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>

#include <windows.h>
#include <Lmcons.h>

QString findUsername(void)
{
	char acUserName[256];
	DWORD nUserName = sizeof(acUserName);
	QString name;
	if (GetUserNameA(acUserName, &nUserName))
		name = QString(acUserName);
	return name;
}



UserInfoWidget::UserInfoWidget(QWidget* parent)
	:QWidget(parent)
{
	auto mainLayout = new QVBoxLayout;

	auto nameLayout = new QHBoxLayout;
	auto nameLabel = new QLabel(tr("Name:"), this);
	auto nameEdit = new QLineEdit(this);
	nameEdit->setText(findUsername());
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameEdit);
	mainLayout->addLayout(nameLayout);


	setLayout(mainLayout);
}


#pragma once

#include <QWidget>

class UserInfoWidget :public QWidget
{
	Q_OBJECT
public:
	UserInfoWidget(QWidget* parent = nullptr);
signals:
	void usernameChanged(const QString& username);
};
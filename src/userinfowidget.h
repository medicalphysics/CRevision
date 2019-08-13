

#pragma once

#include <QWidget>
#include <QString>

class UserInfoWidget :public QWidget
{
	Q_OBJECT
public:
	UserInfoWidget(QWidget* parent = nullptr);
	const QString& currentUsername() { return m_currentUsername; }
signals:
	void usernameChanged(const QString& username);
private:
	QString m_currentUsername;
};
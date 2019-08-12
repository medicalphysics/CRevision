#pragma once

#include <QWidget>
#include <QLineEdit>
#include "models.h"


class CaseWidget :public QWidget
{
	Q_OBJECT
public:
	CaseWidget(QWidget* parent = nullptr);
	void nextCase();
	void setUserName(const QString& user) { m_username = user; }
protected:
	void copyToClipBoard();

private:
	CaseModel* m_caseModel = nullptr;
	QLineEdit* m_lineEdit = nullptr;
	QString m_username;
};
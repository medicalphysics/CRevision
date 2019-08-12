
#pragma once

#include <QtSql>
#include <QSqlTableModel>

#include <QString>
#include <QStringList>

class Question
{
public:
	QString questionGroup;
	QString question;
	QStringList answers;
	bool comment = true;
	bool valid = false;
};

class QuestionModel :public QSqlTableModel
{
	Q_OBJECT
public:
	QuestionModel(QObject* parent=nullptr, QSqlDatabase db = QSqlDatabase());
	int questionCount() { return this->rowCount(); }
	Question question(int index);
protected:
	void readQuestions(const QString& filepath);
};

class AnswerModel :public QSqlTableModel
{
	Q_OBJECT
public:
	AnswerModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
};

class CaseModel :public QSqlTableModel
{
	Q_OBJECT
public:
	CaseModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
protected:
	void readCases(const QString& filepath);
};

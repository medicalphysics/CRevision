
#pragma once

#include <QtSql>
#include <QSqlTableModel>

#include <QString>
#include <QStringList>
#include <QDateTime>

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


class Answer
{
public:
	QString questionGroup;
	QString question;
	QString answer;
	QString answers;
	QString comment;
	QString accessionName;
	QString username;
	QString date;
	bool isValid=false;
	Answer() { date = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"); }
};

class AnswerModel :public QSqlTableModel
{
	Q_OBJECT
public:
	AnswerModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
	void saveAnswer(const Answer& answer);
};

class CaseModel :public QSqlTableModel
{
	Q_OBJECT
public:
	CaseModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
protected:
	void readCases(const QString& filepath);
};

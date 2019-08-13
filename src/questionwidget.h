#pragma once

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QRadioButton>
#include <QVector>
class QuestionWidget : public QWidget
{
	Q_OBJECT
public:
	QuestionWidget(QWidget* parent = nullptr,
				   const QString& questionGroup = QString(),
		           const QString& question = QString(),
				   const QStringList& answers=QStringList(),
				   bool comment = true);
	const QString& question() const { return m_question; };
	const QString& questionGroup() const { return m_questionGroup; };
	const QString& answer() const { return m_answer; };
	const QString& comment() const { return m_comment; };
	bool hasAnswer() {return m_hasAnswer;}
	void clearQuestion();
signals:
	void answerChanged(void);
protected:
	void buttonClicked();
private:
	bool m_hasComment = true;
	QString m_questionGroup = "";
	QString m_question = "";
	QString m_comment = "";
	QString m_answer = "";
	QStringList m_answers;
	bool m_hasAnswer = false;
	QVector<QRadioButton*> m_buttons;


};
#pragma once

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QRadioButton>
#include <QVector>
#include <QTextEdit>

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
	QString answers() const { return m_answers.join(','); }
	bool hasAnswer() {return m_hasAnswer;}
	void clearQuestion();
signals:
	void answerChanged(void);
	void cleared(void);
protected:
	void buttonClicked();
	void setComment(const QString& txt) { m_comment = txt; }
private:
	bool m_hasComment = true;
	QTextEdit* m_commentEdit = nullptr;
	QString m_questionGroup = "";
	QString m_question = "";
	QString m_comment = "";
	QString m_answer = "";
	QStringList m_answers;
	bool m_hasAnswer = false;
	QVector<QRadioButton*> m_buttons;


};
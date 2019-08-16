#include "questioncollectionwidget.h"
#include "questionwidget.h"

#include <QVBoxLayout>
#include <QString>
#include <QGridLayout>
#include <QGroupBox>

QuestionCollectionWidget::QuestionCollectionWidget(QWidget* parent)
	:QWidget(parent)
{
	QSqlDatabase db = QSqlDatabase::database();
	m_questionModel = new QuestionModel(this, db);

	auto mainLayout = new QVBoxLayout(this);

	for (int i = 0; i < m_questionModel->questionCount(); ++i)
	{
		auto q = m_questionModel->question(i);
		if (q.valid)
		{
			auto w = new QuestionWidget(this, q.questionGroup, q.question, q.answers, q.comment);
			m_questionWidgets.append(w);
			connect(w, &QuestionWidget::answerChanged, this, &QuestionCollectionWidget::answerChanged);
			//mainLayout->addWidget(w);
		}
	}
	QMap<QString, QGridLayout*> layouts;

	const int nColumns = 4;

	for (auto w : m_questionWidgets)
	{
		auto group = w->questionGroup();
		if (!layouts.contains(group))
		{
			auto box = new QGroupBox(group);
			layouts[group] = new QGridLayout(this);
			box->setLayout(layouts[group]);
			//mainLayout->addLayout(layouts[group]);
			mainLayout->addWidget(box);
		}
		auto layout = layouts[group];
		const int n_elements = layout->count();

		const int row = n_elements / nColumns;
		const int column = n_elements % nColumns;
		layout->addWidget(w, row, column);
	}

	setLayout(mainLayout);
}

QVector<Answer> QuestionCollectionWidget::getAnswers()
{
	QVector<Answer> answers(m_questionWidgets.size());
	for (int i = 0; i < m_questionWidgets.size(); ++i)
	{
		answers[i].questionGroup = m_questionWidgets[i]->questionGroup();
		answers[i].question = m_questionWidgets[i]->question();
		answers[i].answer = m_questionWidgets[i]->answer();
		answers[i].comment = m_questionWidgets[i]->comment();
		answers[i].isValid = m_questionWidgets[i]->hasAnswer();
		answers[i].answers = m_questionWidgets[i]->answers();
	}
	return answers;
}

void QuestionCollectionWidget::clearAllQuestions()
{
	for (auto w : m_questionWidgets)
	{
		w->clearQuestion();
	}
	emit answerReady(false);
}

void QuestionCollectionWidget::answerChanged(void)
{
	bool allAnswered = true;
	for (auto w : m_questionWidgets)
	{
		allAnswered = w->hasAnswer() && allAnswered;
	}
	emit answerReady(allAnswered);
}

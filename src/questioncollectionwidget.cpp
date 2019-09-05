#include "questioncollectionwidget.h"
#include "questionwidget.h"

#include <QVBoxLayout>
#include <QString>
#include <QGridLayout>
#include <QGroupBox>
#include <QFont>

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

	const int maxColumns = 1;

	for (auto w : m_questionWidgets)
	{
		auto group = w->questionGroup();
		if (!layouts.contains(group))
		{
			auto box = new QGroupBox(group);

			QFont font = box->font();
			font.setPointSize(font.pointSize() + 4);
			font.setBold(true);
			box->setFont(font);

			layouts[group] = new QGridLayout(this);
			box->setLayout(layouts[group]);
			mainLayout->addWidget(box);
		}
		auto layout = layouts[group];
		layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		const int n_elements = layout->count();

		const int row = n_elements / maxColumns;
		const int column = n_elements % maxColumns;
		layout->addWidget(w, row, column);
	}

	mainLayout->addStretch();
	
	auto vStrechLayout = new QHBoxLayout(this);
	vStrechLayout->setContentsMargins(0, 0, 0, 0);
	vStrechLayout->addLayout(mainLayout);
	auto spacerItem = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
	vStrechLayout->addSpacerItem(spacerItem);


	setLayout(vStrechLayout);
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

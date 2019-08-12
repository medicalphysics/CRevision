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
			//mainLayout->addWidget(w);
		}
	}
	QMap<QString, QGridLayout*> layouts;
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
		int n_elements = layout->count();
		int row = n_elements / 2;
		int column = n_elements % 2 ? 1 : 0;
		layout->addWidget(w, row, column);
	}

	setLayout(mainLayout);
}
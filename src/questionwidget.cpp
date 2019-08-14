
#include "questionwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>



QuestionWidget::QuestionWidget(QWidget* parent, const QString& questionGroup, const QString& question, const QStringList& answers, bool comment)
	: QWidget(parent), m_questionGroup(questionGroup), m_question(question), m_answers(answers), m_hasComment(comment)
{
	auto mainLayout = new QVBoxLayout(this);
	//question
	auto questionLabel = new QLabel(m_question, this);
	mainLayout->addWidget(questionLabel);
	//answers
	auto groupBox = new QGroupBox();
	auto groupBoxLayout = new QVBoxLayout(groupBox);
	for (const QString& a : m_answers)
	{
		QRadioButton* r = new QRadioButton(a, groupBox);
		groupBoxLayout->addWidget(r);
		connect(r, &QRadioButton::toggled, this, &QuestionWidget::buttonClicked);
		m_buttons.append(r);
	}
	groupBox->setLayout(groupBoxLayout);
	groupBox->setFlat(true); // no borders
	mainLayout->addWidget(groupBox);
	//comment
	auto commentLayout = new QHBoxLayout(this);
	auto commentLabel = new QLabel(tr("Comment"), this);
	commentLayout->addWidget(commentLabel);
	auto commentLineEdit = new QLineEdit(this);
	connect(commentLineEdit, &QLineEdit::textChanged, this, &QuestionWidget::setComment);
	connect(this, &QuestionWidget::cleared, commentLineEdit, &QLineEdit::clear);
	commentLineEdit->setEnabled(m_hasComment);
	commentLayout->addWidget(commentLineEdit);
	mainLayout->addLayout(commentLayout);
	
	this->setLayout(mainLayout);
}


void QuestionWidget::clearQuestion()
{
	for (auto r : m_buttons)
	{
		r->setAutoExclusive(false);
		r->setChecked(false);
		r->setAutoExclusive(true);
	}
	m_hasAnswer = false;
	m_comment = "";
	emit cleared();
}

void QuestionWidget::buttonClicked()
{
	for (auto r : m_buttons)
	{
		if (r->isChecked())
		{
			m_answer = r->text();
			m_hasAnswer = true;
			emit answerChanged();
		}
	}
}

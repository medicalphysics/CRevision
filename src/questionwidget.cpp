
#include "questionwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>

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
		auto r = new QRadioButton(a, groupBox);
		groupBoxLayout->addWidget(r);
	}
	groupBox->setLayout(groupBoxLayout);
	groupBox->setFlat(true); // no borders
	mainLayout->addWidget(groupBox);
	//comment
	auto commentLayout = new QHBoxLayout(this);
	auto commentLabel = new QLabel(tr("Comment"), this);
	commentLayout->addWidget(commentLabel);
	auto commentLineEdit = new QLineEdit(this);
	commentLineEdit->setEnabled(m_hasComment);
	commentLayout->addWidget(commentLineEdit);
	mainLayout->addLayout(commentLayout);
	
	this->setLayout(mainLayout);
}

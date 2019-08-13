#include "nextwidget.h"

#include <QPushButton>
#include <QHBoxLayout>

NextWidget::NextWidget(QWidget* parent)
	:QWidget(parent)
{
	auto layout = new QHBoxLayout(this);

	m_saveNextButton = new QPushButton(tr("Save and Next"), this);
	m_saveNextButton->setDisabled(true);
	auto discardNextButton = new QPushButton(tr("Discard and Next"), this);

	connect(m_saveNextButton, &QPushButton::clicked, [=]() {emit this->saveAndNextCase(); });
	connect(discardNextButton, &QPushButton::clicked, [=]() {emit this->discardAndNextCase(); });

	layout->addWidget(discardNextButton);
	layout->addStretch();
	layout->addWidget(m_saveNextButton);
	setLayout(layout);
}
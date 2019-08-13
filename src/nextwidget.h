#pragma once

#include "models.h"

#include <QWidget>
#include <QPushButton>

class NextWidget : public QWidget
{
	Q_OBJECT
public:
	NextWidget(QWidget* parent = nullptr);
	void answersReady(bool ready) { m_saveNextButton->setEnabled(ready); }
signals:
	void discardAndNextCase();
	void saveAndNextCase();
private:
	AnswerModel* m_answermodel = nullptr;
	QPushButton* m_saveNextButton = nullptr;
};
#pragma once

#include <QWidget>
#include <QVector>

#include "models.h"
#include "questionwidget.h"

class QuestionCollectionWidget :public QWidget
{
	Q_OBJECT
public:
	QuestionCollectionWidget(QWidget* parent = nullptr);
private:
	QuestionModel* m_questionModel=nullptr;
	QVector<QuestionWidget*> m_questionWidgets;
};
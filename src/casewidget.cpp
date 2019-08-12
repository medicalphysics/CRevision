

#include "casewidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QClipBoard>
#include <QGuiApplication>
#include <QSet>
#include < QRandomGenerator>

#include <qdebug.h>

CaseWidget::CaseWidget(QWidget* parent)
	:QWidget(parent)
{
	QSqlDatabase db = QSqlDatabase::database();
	m_caseModel = new CaseModel(this, db);
	
	auto layout = new QHBoxLayout();

	m_lineEdit = new QLineEdit(this);
	auto label = new QLabel(tr("Accession number:"), this);
	auto copyButton = new QPushButton(tr("Copy"), this);
	connect(copyButton, &QPushButton::clicked, this, &CaseWidget::copyToClipBoard);
	layout->addWidget(label);
	layout->addWidget(m_lineEdit);
	layout->addWidget(copyButton);
	setLayout(layout);
	nextCase();
}

void CaseWidget::nextCase()
{
	QSqlDatabase db = QSqlDatabase::database();

	QSqlQuery query(db);
	//finding used accession names
	query.bindValue(":username", m_username);
	query.exec("SELECT DISTINCT accession_name FROM answers WHERE username = :username");

	QSet<QString> usedAC;
	while (query.next())
	{
		auto ac = query.value(0).toString();
		usedAC.insert(ac);
	}
	//finding all accession names
	query.clear();
	query.exec("SELECT accession_name FROM cases");
	QSet<QString> allAC;
	while (query.next())
	{
		auto ac = query.value(0).toString();
		allAC.insert(ac);
	}
	allAC.subtract(usedAC);
	int count = allAC.count();
	if (count == 0)
		return;
	qDebug() << allAC;
	int n = QRandomGenerator::global()->bounded(count);
	auto pos = allAC.begin();
	auto accession_number = *(pos+n);
	m_lineEdit->setText(accession_number);
	//finding random 


}

void CaseWidget::copyToClipBoard()
{
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_lineEdit->text());
}
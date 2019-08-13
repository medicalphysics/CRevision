

#include "casewidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QClipBoard>
#include <QGuiApplication>
#include <QSet>
#include <QRandomGenerator>
#include <QTimer>
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
	QTimer::singleShot(0, [=]() {this->nextCase(); });
	
}

void CaseWidget::nextCase()
{
	QSqlDatabase db = QSqlDatabase::database();

	QSqlQuery query(db);
	//finding used accession names
	qDebug() << "username: " << m_username;
	
	//query.exec("SELECT DISTINCT accession_name FROM answers WHERE username = :username");

	//finding case that has not been answered by username
	QString queryTxt = "SELECT DISTINCT accession_name FROM cases WHERE accession_name NOT IN (SELECT accession_name FROM answers WHERE username = '";
	queryTxt += m_username + "')";
	//query.prepare("SELECT DISTINCT accession_name FROM cases WHERE accession_name NOT IN (SELECT accession_name FROM answers WHERE username = ?)");
	//query.addBindValue(m_username, QSql::Out);
	query.exec(queryTxt);



	qDebug() << "Executed query: " << query.executedQuery();
	QSet<QString> availableCases;
	while (query.next())
	{
		auto ac = query.value(0).toString();
		availableCases.insert(ac);
	}
	
	int count = availableCases.count();
	if (count == 0) // if no available cases return empty string
	{
		m_lineEdit->setText("");
		return;
	}
	//finding random available case
	int n = QRandomGenerator::global()->bounded(count);
	auto pos = availableCases.begin();
	auto accession_number = *(pos+n);
	m_lineEdit->setText(accession_number);
}

void CaseWidget::copyToClipBoard()
{
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_lineEdit->text());
}
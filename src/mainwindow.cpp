/*This file is part of OpenDXMC.

OpenDXMC is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenDXMC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenDXMC. If not, see < https://www.gnu.org/licenses/>.

Copyright 2019 Erlend Andersen
*/

#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QMenuBar>
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QScrollArea>
#include <QTimer>

#include "nextwidget.h"



MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{

	auto menu = this->menuBar();
	auto fileMenu = menu->addMenu(tr("Fil"));
	auto exportAction = fileMenu->addAction(tr("Eksporter svar"));
	fileMenu->addSeparator();
	auto quitAction = fileMenu->addAction(tr("Avslutt"));
	connect(exportAction, &QAction::triggered, this, &MainWindow::exportData);
	connect(quitAction, &QAction::triggered, this, &MainWindow::close);
	exportAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	quitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));



	//setting up database
	if (!setupDataBase())
		return;
	QSqlDatabase db = QSqlDatabase::database();
	m_answerModel = new AnswerModel(this, db);

	//creating layout
	auto mainLayout = new QVBoxLayout;

	//userinfo
	m_userInfo = new UserInfoWidget(this);
	mainLayout->addWidget(m_userInfo);
	//casewidget
	m_caseWidget = new CaseWidget(this);
	connect(m_userInfo, &UserInfoWidget::usernameChanged, m_caseWidget, &CaseWidget::setUserName);
	mainLayout->addWidget(m_caseWidget);
	//questions
	auto scrollArea = new QScrollArea(this);
	m_questionsWidget = new QuestionCollectionWidget(scrollArea);
	scrollArea->setWidget(m_questionsWidget);
	scrollArea->setWidgetResizable(true);
	scrollArea->setAlignment(Qt::AlignLeft);
	//scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	auto scrollView = scrollArea->viewport();
	//scrollView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	mainLayout->addWidget(scrollArea);
	//nextcase
	auto nextWidget = new NextWidget(this);
	mainLayout->addWidget(nextWidget);


	auto mainWidget = new QWidget(this);
	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);



	//logic
	connect(m_userInfo, &UserInfoWidget::usernameChanged, m_caseWidget, &CaseWidget::setUserName);
	connect(nextWidget, &NextWidget::discardAndNextCase, m_questionsWidget, &QuestionCollectionWidget::clearAllQuestions);
	connect(nextWidget, &NextWidget::discardAndNextCase, m_caseWidget, &CaseWidget::nextCase);
	connect(nextWidget, &NextWidget::saveAndNextCase, this, &MainWindow::saveAnswers);
	connect(nextWidget, &NextWidget::saveAndNextCase, m_questionsWidget, &QuestionCollectionWidget::clearAllQuestions);
	connect(nextWidget, &NextWidget::saveAndNextCase, m_caseWidget, &CaseWidget::nextCase);
	connect(m_questionsWidget, &QuestionCollectionWidget::answerReady, nextWidget, &NextWidget::answersReady);

	QTimer::singleShot(0, [=](void) {this->setMinimumFixedSize(scrollArea); });
}


void MainWindow::setMinimumFixedSize(QWidget* wid)
{

	auto size = m_questionsWidget->sizeHint();
	qDebug() << "Width: " << size;

	int w = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
	
	wid->setMinimumWidth(size.width() + w);

	
}

bool MainWindow::setupDataBase()
{
	auto db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("database.sqlite3");
	if (!db.open())
	{
		qDebug() << "Error: connection with database fail";
		return false;
	}
	else
	{
		qDebug() << "Database: connection ok";
	}

	//creating tables if needed 
	if (!db.tables().contains("cases"))
	{
		QSqlQuery query("CREATE TABLE cases (id INTEGER PRIMARY KEY, accession_name TEXT)");
		if (!query.isActive())
			qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
		qDebug() << query.executedQuery();
	}
	
	if (!db.tables().contains("questions"))
	{
		QSqlQuery query("CREATE TABLE questions (id INTEGER PRIMARY KEY, question_group TEXT, question TEXT, answers TEXT, comment INTEGER DEFAULT 0)");
		if (!query.isActive())
			qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
		qDebug() << query.executedQuery();
	}
	if (!db.tables().contains("answers"))
	{
		QSqlQuery query("CREATE TABLE answers (id INTEGER PRIMARY KEY, question_group TEXT, question TEXT, answers TEXT, answer TEXT, comment TEXT, accession_name TEXT, username TEXT, date TEXT)");
		if (!query.isActive())
			qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
		qDebug() << query.executedQuery();
	}
	return true;
}

void MainWindow::saveAnswers()
{
	auto  record = m_answerModel->record();
	QString caseName = m_caseWidget->currentCase();
	QString username = m_userInfo->currentUsername();
	record.setValue("accession_name", caseName);
	record.setValue("username", username);

	auto answers = m_questionsWidget->getAnswers();
	for (const auto& a : answers)
	{
		auto r = QSqlRecord(record);
		r.setValue("question_group", a.questionGroup.trimmed());
		r.setValue("question", a.question.trimmed());
		r.setValue("answer", a.answer.trimmed());
		r.setValue("answers", a.answers.trimmed());
		r.setValue("comment", a.comment.trimmed());
		r.setValue("date", a.date.trimmed());
		auto success = m_answerModel->insertRecord(-1, r);
		qDebug() << "Insert answer " << success;
	}
	m_answerModel->submitAll();
}

void MainWindow::exportData()
{
	auto savepath = QFileDialog::getSaveFileName(this, tr("Eksporter svar"), "", "*.txt");
	if (savepath.isNull())
	{
		return;
	}

	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery query(db);
	query.exec("SELECT * FROM answers");

	QFile file(savepath);
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		auto record = db.record("answers");
		QStringList fieldNames;
		for (int i = 0; i < record.count(); ++i)
		{
			fieldNames.append(record.fieldName(i));
			stream << record.fieldName(i) << ";";
		}
		stream << "\n";
		while (query.next())
		{
			for (const auto& fn : fieldNames)
			{ 
				stream << query.value(fn).toString() << ";";
			}
			stream << "\n";
		}
		file.close();
	}

}

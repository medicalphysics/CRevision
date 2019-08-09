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
#include "userinfowidget.h"
#include "models.h"

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSqlDatabase>

#include <QTableView>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent) 
	: QMainWindow(parent)
{


	//setting up database
	if (!setupDataBase())
		return;


	//creating layout
    auto mainLayout = new QVBoxLayout;

	auto userInfo = new UserInfoWidget(this);
	mainLayout->addWidget(userInfo);

	auto mainWidget = new QWidget(this);
	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);


	//test
	QSqlDatabase db = QSqlDatabase::database();
	
	auto v = new QTableView(this);
	mainLayout->addWidget(v);
	auto testmodel = new QuestionModel(this, db);
	v->setModel(testmodel);

	auto w = new QTableView(this);
	mainLayout->addWidget(w);
	auto testcasemodel = new CaseModel(this, db);
	w->setModel(testcasemodel);


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
		QSqlQuery query("CREATE TABLE cases (id INTEGER PRIMARY KEY, case_name TEXT)");
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
		QSqlQuery query("CREATE TABLE answers (id INTEGER PRIMARY KEY, question TEXT, answer TEXT, comment TEXT, accession_name TEXT, username TEXT, date TEXT)");
		if (!query.isActive())
			qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
		qDebug() << query.executedQuery();
	}
	return true;
}

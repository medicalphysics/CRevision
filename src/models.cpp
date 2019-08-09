

#include "models.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSqlRecord>


QuestionModel::QuestionModel(QObject* parent, QSqlDatabase db)
	:QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("questions");
	select();
	readQuestions("resources/data/sampleQuestions.txt");
}
void QuestionModel::readQuestions(const QString& filepath)
{
	QFile file(filepath);
	if (!file.open(QFile::ReadOnly |
		QFile::Text))
	{
		qDebug() << " Could not open the file for reading";
		file.close();
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QStringList line = in.readLine().split(';');
		bool ignore = false;
		if (line.length() == 0)
			ignore = true;
		if (auto first_element = line.at(0).trimmed(); first_element.at(0) == '#')
			ignore = true;
		auto  element= this->record();
		
		if (line.count() < element.count() - 1)
			ignore = true;

		if (!ignore)
		{
			element.setValue("question_group", line.at(0).trimmed());
			element.setValue("question", line.at(1).trimmed());
			element.setValue("answers", line.at(2).trimmed());
			int comment = line.at(3).trimmed().toLower().at(0) != 'n';
			element.setValue("comment", comment);
			auto success = insertRecord(-1, element);
			if (!success)
				qDebug() << "Failed to insert question row";
			submitAll();
		}
	}
	file.close();	
}


AnswerModel::AnswerModel(QObject* parent, QSqlDatabase db)
	:QSqlTableModel(parent, db)
{

}

CaseModel::CaseModel(QObject* parent, QSqlDatabase db)
	: QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("cases");
	select();
	readCases("resources/data/sampleCases.txt");
}

void CaseModel::readCases(const QString& filepath)
{
	QFile file(filepath);
	if (!file.open(QFile::ReadOnly |
		QFile::Text))
	{
		qDebug() << " Could not open the file for reading";
		file.close();
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QStringList line = in.readLine().split(';');
		bool ignore = false;
		if (line.length() == 0)
			ignore = true;
		if (auto first_element = line.at(0).trimmed(); first_element.at(0) == '#')
			ignore = true;
		auto  element = this->record();

		if (line.count() < element.count() - 1)
			ignore = true;
		if (!ignore)
		{
			element.setValue("case_name", line.at(0).trimmed());
			auto success = insertRecord(-1, element);
			if (!success)
				qDebug() << "Failed to insert case row";
			submitAll();
		}
	}
	file.close();
}
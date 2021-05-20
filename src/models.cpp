

#include "models.h"

#include <QDateTime>
#include <QFile>
#include <QSqlRecord>
#include <QStringList>
#include <QTextStream>

QString cleanStringList(const QString& txt, const QString insep = ",", const QString outsep = ",")
{

    auto list = txt.split(insep);
    for (int i = 0; i < list.size(); ++i) {
        list[i] = list[i].trimmed();
        if (outsep != insep) {
            list[i] = list[i].replace(outsep, " ");
        }
    }
    return list.join(outsep);
}

QuestionModel::QuestionModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    if (!db.tables().contains("questions")) {
        db.exec("CREATE TABLE questions (id INTEGER PRIMARY KEY, question_group TEXT, roles TEXT, question TEXT, answers TEXT, comment INTEGER DEFAULT 0, criteria TEXT)");
    }

    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("questions");
    select();
    readQuestions("resources/data/sampleQuestions.txt");
}

Question QuestionModel::question(int index)
{
    Question q;
    if (index < 0 || index >= rowCount())
        return q;
    auto r = record(index);
    q.questionGroup = r.field("question_group").value().toString();
    q.question = r.field("question").value().toString();
    q.comment = r.field("comment").value().toInt() > 0;

    auto answers = r.field("answers").value().toString();
    if (!answers.isEmpty()) {
        auto answersList = answers.split(',');
        q.answers = answersList;
    }
    auto roles = r.field("roles").value().toString();
    if (!roles.isEmpty()) {
        auto rolesList = roles.split(',');
        q.roles = rolesList;
    }
    auto criteria = r.field("criteria").value().toString();
    if (!criteria.isEmpty()) {
        q.criteria = criteria.split(";");
    }
    q.valid = true;
    return q;
}

void QuestionModel::readQuestions(const QString& filepath)
{
    QFile file(filepath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << " Could not open the file for reading";
        file.close();
        return;
    }

    //deletes questions
    removeRows(0, rowCount());
    submitAll();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList line = in.readLine().split(';');
        if (line.size() > 0) {
            if (line.at(0).size() > 0) {
                if (const auto first_element = line.at(0).trimmed(); first_element.at(0) != '#') {
                    auto element = this->record();
                    if (line.size() >= element.count() - 2) {
                        element.setValue("question_group", line.at(0).trimmed());
                        element.setValue("roles", cleanStringList(line.at(1)));
                        element.setValue("question", line.at(2).trimmed());
                        element.setValue("answers", cleanStringList(line.at(3)));
                        int comment = line.at(4).trimmed().toLower().at(0) != 'n';
                        element.setValue("comment", comment);

                        QStringList citeria;
                        for (int i = 5; i < line.size(); ++i) {
                            auto crit = line[i].trimmed();
                            if (!crit.isEmpty()) {
                                citeria.append(line[i].trimmed());
                            }
                        }
                        element.setValue("criteria", citeria.join(";"));

                        auto success = insertRecord(-1, element);
                        if (!success)
                            qDebug() << "Failed to insert question row";
                        submitAll();
                    }
                }
            }
        }
    }
    file.close();
}

AnswerModel::AnswerModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    if (!db.tables().contains("answers")) {
        db.exec("CREATE TABLE answers (id INTEGER PRIMARY KEY, question_group TEXT, role TEXT, roles TEXT, question TEXT, answers TEXT, answer TEXT, comment TEXT, accession_name TEXT, username TEXT, date TEXT, criteria TEXT, criteriaSelected TEXT)");
    }
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("answers");
    select();
}

void AnswerModel::saveAnswer(const Answer& answer)
{
    if (answer.isValid) {
        auto element = this->record();
        element.setValue("question_group", answer.questionGroup);
        element.setValue("question", answer.question);
        element.setValue("answer", answer.answer);
        element.setValue("answers", answer.answers.join(","));
        element.setValue("role", answer.role);
        element.setValue("roles", answer.roles.join(","));
        element.setValue("comment", answer.comment);
        element.setValue("accession_name", answer.accessionName);
        element.setValue("username", answer.username);
        element.setValue("date", answer.date);
        element.setValue("criteria", answer.criteria.join(";"));
        element.setValue("criteriaSelected", answer.criteriaSelectedText());
        insertRecord(-1, element);
        submitAll();
    }
}

CaseModel::CaseModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    if (!db.tables().contains("cases")) {
        db.exec("CREATE TABLE cases (id INTEGER PRIMARY KEY, accession_name TEXT, roles TEXT)");
    }

    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("cases");
    select();
    readCases("resources/data/sampleCases.txt");
}

void CaseModel::readCases(const QString& filepath)
{

    //deletes all cases
    removeRows(0, rowCount());
    submitAll();

    QFile file(filepath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << " Could not open the file for reading";
        file.close();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        const auto line = in.readLine().split(';');

        QString anum, roles;

        if (line.size() > 0) {
            anum = line.at(0).trimmed();
        }
        if (line.size() > 1) {
            roles = cleanStringList(line.at(1));
        }

        bool skip = anum.size() == 0;
        if (!skip)
            skip = anum.at(0) == '#';

        if (!skip) {
            auto element = this->record();
            element.setValue("accession_name", anum);
            element.setValue("roles", roles);
            auto success = insertRecord(-1, element);
            if (!success)
                qDebug() << "Failed to insert case row";
            submitAll();
        }
    }
    file.close();

    //deleting duplucates
    auto db = database();
    db.exec("DELETE FROM cases WHERE id NOT IN (SELECT MAX(id) FROM cases GROUP BY accession_name)");
}

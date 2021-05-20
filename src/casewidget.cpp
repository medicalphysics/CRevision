

#include "casewidget.h"

#include <QClipBoard>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSet>
#include <QTimer>
#include <qdebug.h>

CaseWidget::CaseWidget(CaseModel* model, QWidget* parent)
    : QWidget(parent)
{

    m_caseModel = model;

    auto layout = new QHBoxLayout();

    m_lineEdit = new QLineEdit(this);
    auto label = new QLabel(QString::fromLatin1("Undersøkelse:"), this);
    auto copyButton = new QPushButton(tr("Kopier"), this);
    connect(copyButton, &QPushButton::clicked, this, &CaseWidget::copyToClipBoard);
    layout->addWidget(label);
    layout->addWidget(m_lineEdit);
    layout->addWidget(copyButton);
    setLayout(layout);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &CaseWidget::caseChanged);
}

void CaseWidget::nextCase()
{
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);

    //finding case that has not been answered by username
    QString queryTxt = "SELECT * FROM cases WHERE accession_name NOT IN (SELECT accession_name FROM answers WHERE username = '";
    queryTxt += m_username + "')";    
    query.exec(queryTxt);

    qDebug() << "Executed query: " << query.executedQuery();
    QStringList availableCases;
    while (query.next()) {
        auto record = query.record();
        auto ac = record.field("accession_name").value().toString();
        auto roles_r = record.field("roles").value().toString();        
        if (roles_r.isEmpty()) {
            availableCases.append(ac);
        } else {
            auto roles = roles_r.split(',');
            if (roles.contains(m_currentRole)) {
                availableCases.append(ac);
            }
        }
    }

    const int count = availableCases.count();
    if (count == 0) // if no available cases return empty string
    {
        m_lineEdit->setText("");
    } else {
        //finding random available case
        const int n = QRandomGenerator::global()->bounded(count);
        const auto accession_number = availableCases[n];
        m_lineEdit->setText(accession_number);
    }
}

void CaseWidget::setUserName(const QString& user)
{
    m_username = user;
}

void CaseWidget::setRole(const QString& role)
{
    m_currentRole = role;
    nextCase();
}

void CaseWidget::copyToClipBoard()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(m_lineEdit->text());
}

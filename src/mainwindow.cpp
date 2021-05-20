
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QScrollArea>
#include <QSqlDatabase>
#include <QStyle>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    setupMenu();

    //setting up database
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqlite3");
    if (!db.open()) {
        return;
    } else {
        qDebug() << "Database: connection ok";
    }

    m_answerModel = new AnswerModel(this, db);
    m_questionModel = new QuestionModel(this, db);
    m_caseModel = new CaseModel(this, db);

    //creating layout
    auto mainLayout = new QVBoxLayout;
    auto mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    // infowidget
    m_infoWidget = new InfoWidget(m_questionModel, m_caseModel, this);
    mainLayout->addWidget(m_infoWidget);

    //questions
    auto scrollArea = new QScrollArea();
    m_questionsWidget = new QuestionCollectionWidget(m_questionModel, m_answerModel, scrollArea);
    connect(m_questionsWidget, &QuestionCollectionWidget::nextCase, m_infoWidget, &InfoWidget::requestNextCase);
    connect(m_infoWidget, &InfoWidget::caseChanged, m_questionsWidget, &QuestionCollectionWidget::setCase);
    connect(m_infoWidget, &InfoWidget::roleChanged, m_questionsWidget, &QuestionCollectionWidget::setRole);
    connect(m_infoWidget, &InfoWidget::userNameChanged, m_questionsWidget, &QuestionCollectionWidget::setUserName);
    scrollArea->setWidget(m_questionsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea);

    QTimer::singleShot(0, [=](void) { this->setMinimumFixedSize(scrollArea); });
}

void MainWindow::setupMenu()
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
}

void MainWindow::setMinimumFixedSize(QWidget* wid)
{
    auto size = m_questionsWidget->sizeHint();
    qDebug() << "Width: " << size;

    int w = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);

    wid->setMinimumWidth(size.width() + w);
}

void MainWindow::exportData()
{
    auto savepath = QFileDialog::getSaveFileName(this, tr("Eksporter svar"), "", "*.txt");
    if (savepath.isNull()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec("SELECT * FROM answers");

    QFile file(savepath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        auto record = db.record("answers");
        QStringList fieldNames;
        for (int i = 0; i < record.count(); ++i) {
            fieldNames.append(record.fieldName(i));
            stream << record.fieldName(i) << ";";
        }
        stream << "\n";
        while (query.next()) {
            for (const auto& fn : fieldNames) {
                auto value = query.value(fn).toString();
                value.replace(QChar(';'), QChar(' '));
                value.replace(QChar('\n'), QChar(' '));
                stream << value << ";";
            }
            stream << "\n";
        }
        file.close();
    }
}

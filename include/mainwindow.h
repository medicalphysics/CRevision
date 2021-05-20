
#pragma once

#include "infowidget.h"
#include "models.h"
#include "questioncollectionwidget.h"

#include <QMainWindow>
#include <QScrollArea>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

protected:
    void setupMenu();
    void setMinimumFixedSize(QWidget*);
    bool setupDataBase();    
    void exportData();

private:
    AnswerModel* m_answerModel = nullptr;
    QuestionModel* m_questionModel = nullptr;
    CaseModel* m_caseModel = nullptr;
    InfoWidget* m_infoWidget = nullptr;
    QuestionCollectionWidget* m_questionsWidget = nullptr;
};

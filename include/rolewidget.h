

#pragma once

#include "models.h"

#include <QButtonGroup>
#include <QMap>
#include <QRadioButton>
#include <QStringList>
#include <QWidget>

class RoleWidget : public QWidget {
    Q_OBJECT
public:
    RoleWidget(QuestionModel* model, QWidget* parent = nullptr);
    void setRoles(const QStringList& roles);

signals:
    void roleChanged(const QString& role);

protected:
    void setupWidget();
    void updateRole(int role);

private:
    QButtonGroup* m_buttonGroup = nullptr;
    QList<QRadioButton*> m_buttons;
    QMap<int, QString> m_roles;
};

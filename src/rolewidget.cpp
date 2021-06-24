

#include "rolewidget.h"

#include <QHBoxLayout>

RoleWidget::RoleWidget(QuestionModel* model, QWidget* parent)
    : QWidget(parent)
{
    QStringList unique_roles;
    const QString field_name("roles");

    for (int i = 0; i < model->rowCount(); ++i) {
        auto record = model->record(i);
        if (!record.isEmpty()) {
            auto field = record.field(field_name);
            const auto roles = field.value().toString().split(',');
            for (const auto& role_r : roles) {
                const auto role = role_r.trimmed();

                if (!(unique_roles.contains(role)) && !role.isEmpty()) {

                    unique_roles.append(role);
                }
            }
        }
    }

    if (unique_roles.size() == 0) {
        unique_roles.append(tr("Alle"));
    }

    setupWidget();
    setRoles(unique_roles);
    QTimer::singleShot(0, [=]() { this->updateRole(-1); });
}

void RoleWidget::setRoles(const QStringList& roles)
{
    auto blayout = layout();

    for (int i = 0; i < roles.size(); ++i) {
        if (m_buttons.size() <= i) {
            auto button = new QRadioButton(this);
            blayout->addWidget(button);
            m_buttonGroup->addButton(button, i);
            m_buttons.append(button);
        }
        auto button = m_buttons.at(i);
        m_roles[i] = roles[i];
        button->setText(roles[i]);
        button->setHidden(false);
        button->setChecked(false);
    }

    for (int i = roles.size(); i < m_buttons.size(); ++i) {
        auto button = m_buttons[i];
        button->setChecked(false);
        button->setHidden(true);
    }

    auto dummy = QString();
    emit roleChanged(dummy);
}

void RoleWidget::setupWidget()
{
    auto layout = new QVBoxLayout;
    setLayout(layout);

    m_buttonGroup = new QButtonGroup(this);

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &RoleWidget::updateRole);
}
void RoleWidget::updateRole(int roleID)
{
    if (m_roles.contains(roleID)) {
        emit roleChanged(m_roles[roleID]);
    } else if (m_roles.size() == 1) {
        const auto key = m_roles.firstKey();
        emit roleChanged(m_roles[key]);
        m_buttons[0]->setChecked(true);
    } else {
        auto dummy = QString();
        emit roleChanged(dummy);
    }
}

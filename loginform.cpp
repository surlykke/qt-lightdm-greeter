/*
* Copyright (c) 2012-2015 Christian Surlykke
*
* This file is part of qt-lightdm-greeter 
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#include <QDebug>
#include <QCompleter>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QPixmap>
#include <QMessageBox>
#include <QMenu>
#include <QProcess>

#include "loginform.h"
#include "ui_loginform.h"
#include "settings.h"

int rows(QAbstractItemModel& model) {
    return model.rowCount(QModelIndex());
}

QString displayData(QAbstractItemModel& model, int row, int role)
{
    QModelIndex modelIndex = model.index(row, 0);
    return model.data(modelIndex, role).toString();
}

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent), 
    ui(new Ui::LoginForm),
    m_Greeter(),
    power(this),
    usersModel(*this),
    sessionsModel(),
    sessionComboTouched(false)
{
    if (!m_Greeter.connectSync())
    {
        close();
    }

    ui->setupUi(this);
    initialize();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setFocus(Qt::FocusReason reason)
{
    if (ui->userCombo->currentIndex() == -1)
    {
        ui->userCombo->setFocus(reason);
    }
    else
    {
        ui->passwordInput->setFocus(reason);
    }
}



QString LoginForm::otherUserName()
{
    return ui->otherUserInput->text();
}

void LoginForm::initialize()
{
    // Appearence
    QPixmap icon(":/resources/rqt-2.png");
    ui->iconLabel->setPixmap(icon.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->hostnameLabel->setText(m_Greeter.hostname());

    // Fill in data
    ui->userCombo->setModel(&usersModel);
    ui->sessionCombo->setModel(&sessionsModel);
    ui->leaveButton->setMenu(buildLeaveMenu());

    // Connect
    ui->userCombo->setCurrentIndex(-1);
    connect(ui->userCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(userChanged()));
    connect(ui->otherUserInput, SIGNAL(editingFinished()), this, SLOT(userChanged()));
    connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginClicked()));
    connect(&m_Greeter, SIGNAL(showPrompt(QString,QLightDM::Greeter::PromptType)),
            this,       SLOT(onPrompt(QString,QLightDM::Greeter::PromptType)));
    connect(&m_Greeter, SIGNAL(authenticationComplete()), this, SLOT(authenticationComplete()));

    setSuggestedUser();

    ui->userCombo->setVisible(! m_Greeter.hideUsersHint());

    ui->passwordInput->setEnabled(false);
    ui->passwordInput->clear();
}

void LoginForm::userChanged()
{
    qDebug() << "User changed";
    setSuggestedSession();

    if (m_Greeter.inAuthentication())
    {
        m_Greeter.cancelAuthentication();
    }
    if (! currentUser().isEmpty()) {
        m_Greeter.authenticate(currentUser());
    }
}

void LoginForm::loginClicked()
{
    m_Greeter.respond(ui->passwordInput->text().trimmed());
    ui->passwordInput->clear();
    ui->passwordInput->setEnabled(false);
}

void LoginForm::onPrompt(QString prompt, QLightDM::Greeter::PromptType promptType)
{
    ui->passwordInput->setEnabled(true);
    ui->passwordInput->setFocus();
}


QMenu *LoginForm::buildLeaveMenu()
{
    QMenu* leaveMenu = new QMenu(this);
    if (power.canShutdown()) {
        QAction* action = leaveMenu->addAction(QIcon::fromTheme("system-shutdown"), tr("Shutdown"));
        connect(action, SIGNAL(triggered(bool)), &power, SLOT(shutdown()));
    }
    if (power.canRestart()) {
        QAction* action = leaveMenu->addAction(QIcon::fromTheme("system-reboot"), tr("Restart"));
        connect(action, SIGNAL(triggered(bool)), &power, SLOT(restart()));
    }
    if (power.canHibernate()) {
        QAction* action = leaveMenu->addAction(QIcon::fromTheme("system-suspend-hibernate"), tr("Hibernate"));
        connect(action, SIGNAL(triggered(bool)), &power, SLOT(hibernate()));
    }
    if (power.canSuspend()) {
        QAction* action = leaveMenu->addAction(QIcon::fromTheme("system-suspend"), tr("Suspend"));
        connect(action, SIGNAL(triggered(bool)), &power, SLOT(suspend()));
    }

    return leaveMenu;
}

QString LoginForm::currentUser()
{
    QModelIndex index = usersModel.index(ui->userCombo->currentIndex(), 0, QModelIndex());
    return usersModel.data(index, QLightDM::UsersModel::NameRole).toString();
}

QString LoginForm::currentSession()
{
    QModelIndex index = sessionsModel.index(ui->sessionCombo->currentIndex(), 0, QModelIndex());
    return sessionsModel.data(index, QLightDM::SessionsModel::KeyRole).toString();
}

void LoginForm::setSuggestedUser()
{
    if (m_Greeter.showManualLoginHint()) {
        ui->userCombo->setCurrentIndex(usersModel.rowCount(QModelIndex()) - 1); // 'other..'
    }
    else {
        ui->userCombo->setCurrentIndex(0);
        QString suggestedUser = m_Greeter.selectUserHint();
        if (suggestedUser.isEmpty()) {
            suggestedUser = Settings().getLastUser();
        }
        if (!suggestedUser.isEmpty()) {
            for (int i = 0; i < usersModel.rowCount(QModelIndex()) - 1; i++) {
                if (suggestedUser == usersModel.data(usersModel.index(i, 0), QLightDM::UsersModel::NameRole)) {
                    ui->userCombo->setCurrentIndex(i);
                }
            }
        }
    }
}

void LoginForm::setSuggestedSession()
{
    qDebug() << "setSuggestedSession";
    ui->sessionCombo->setCurrentIndex(0);
    QString sessionHint = m_Greeter.defaultSessionHint();
    qDebug() << "sessionHint" << sessionHint;
    if (!sessionHint.isEmpty()) {
        for (int i = 0; i < sessionsModel.rowCount(QModelIndex()); i++) {
            if (sessionHint == sessionsModel.data(sessionsModel.index(i, 0), QLightDM::SessionsModel::KeyRole)) {
                ui->sessionCombo->setCurrentIndex(i);
            }
        }
    }
}

void LoginForm::authenticationComplete()
{
    if (m_Greeter.isAuthenticated()) {
        Settings settings;
        settings.setLastUser(currentUser());
        m_Greeter.startSessionSync(currentSession());
    }
    else 
    {
        ui->passwordInput->clear();
    }
}

void LoginForm::paintEvent(QPaintEvent *event)
{
    ui->otherUserInput->setVisible(ui->userCombo->currentIndex() == usersModel.rowCount(QModelIndex()) - 1);
    adjustSize();
    QWidget::paintEvent(event);
}


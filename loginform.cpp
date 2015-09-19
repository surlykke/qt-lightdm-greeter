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

const int NameRole = QLightDM::UsersModel::NameRole;
const int KeyRole = QLightDM::SessionsModel::KeyRole;

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
    sessionsModel()
{
    if (!m_Greeter.connectSync()) {
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
    if (ui->userCombo->currentIndex() == -1) {
        ui->userCombo->setFocus(reason);
    }
    else if (ui->userCombo->currentIndex() == ui->userCombo->count() - 1) {
        ui->otherUserInput->setFocus(reason);
    } else {
        ui->passwordInput->setFocus(reason);
    }
}


void LoginForm::initialize()
{
    QPixmap icon(":/resources/rqt-2.png"); // This project came from Razor-qt
    ui->iconLabel->setPixmap(icon.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->hostnameLabel->setText(m_Greeter.hostname());

    ui->userCombo->setModel(&usersModel);
    ui->sessionCombo->setModel(&sessionsModel);
    ui->leaveButton->setMenu(new QMenu(this));
    if (power.canShutdown()) {
        addLeaveEntry("system-shutdown", tr("Shutdown"), SLOT(shutdown()));
    }
    if (power.canRestart()) {
        addLeaveEntry("system-reboot", tr("Restart"), SLOT(restart()));
    }
    if (power.canHibernate()) {
        addLeaveEntry("system-suspend-hibernate", tr("Hibernate"), SLOT(hibernate()));
    }
    if (power.canSuspend()) {
        addLeaveEntry("system-suspend", tr("Suspend"), SLOT(suspend()));
    }

    ui->userCombo->setCurrentIndex(-1);
    connect(ui->userCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(userChanged()));
    connect(ui->otherUserInput, SIGNAL(editingFinished()), this, SLOT(userChanged()));
    connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginClicked()));
    connect(&m_Greeter, SIGNAL(showPrompt(QString, QLightDM::Greeter::PromptType)), this, SLOT(onPrompt(QString, QLightDM::Greeter::PromptType)));
    connect(&m_Greeter, SIGNAL(authenticationComplete()), this, SLOT(authenticationComplete()));

    int userIndex = 0;
    if (m_Greeter.showManualLoginHint()) {
        userIndex = (usersModel.rowCount(QModelIndex()) - 1); // 'other..'
    }
    else {
        QString user = Settings().getLastUser().isEmpty() ?
                    m_Greeter.selectUserHint() : Settings().getLastUser();

        for (int i = 0; i < usersModel.rowCount(QModelIndex()) - 1; i++) {
            if (user == usersModel.data(usersModel.index(i, 0), QLightDM::UsersModel::NameRole)) {
                ui->userCombo->setCurrentIndex(i);
            }
        }
    }
    ui->userCombo->setCurrentIndex(userIndex);

    ui->passwordInput->setEnabled(false);
    ui->passwordInput->clear();
}

void LoginForm::userChanged()
{
    ui->sessionCombo->setCurrentIndex(0);
    QString sessionHint = m_Greeter.defaultSessionHint();
    for (int i = 0; i < sessionsModel.rowCount(QModelIndex()); i++) {
        if (sessionHint == sessionsModel.data(sessionsModel.index(i, 0), QLightDM::SessionsModel::KeyRole)) {
            ui->sessionCombo->setCurrentIndex(i);
        }
    }

    if (m_Greeter.inAuthentication()) {
        m_Greeter.cancelAuthentication();
    }
    if (! currentUser().isEmpty()) {
        m_Greeter.authenticate(currentUser());
        ui->passwordInput->setFocus();
    }
    else if (ui->userCombo->currentIndex() == ui->userCombo->count() - 1) {
        ui->otherUserInput->setFocus();
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


void LoginForm::addLeaveEntry(QString iconName, QString text, const char* slot)
{
    QMenu* menu = ui->leaveButton->menu();
    QAction *action = menu->addAction(QIcon::fromTheme(iconName), text);
    connect(action, SIGNAL(triggered(bool)), &power, slot);
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


void LoginForm::authenticationComplete()
{
    if (m_Greeter.isAuthenticated()) {
        Settings settings;
        settings.setLastUser(currentUser());
        m_Greeter.startSessionSync(currentSession());
    }
    else  {
        ui->passwordInput->clear();
    }
}

void LoginForm::paintEvent(QPaintEvent *event)
{
    ui->userCombo->setVisible(! m_Greeter.hideUsersHint());
    ui->otherUserInput->setVisible(ui->userCombo->currentIndex() == usersModel.rowCount(QModelIndex()) - 1);
    adjustSize();
    QWidget::paintEvent(event);
}


/*
* Copyright (c) 2012-2015 Christian Surlykke, Petr Vanek
*
* This file is part of qt-lightdm-greeter 
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <QMap>
#include <QtGlobal>
#include <QtDebug>
#include <QFile>
#include <QSettings>
#include "constants.h"
#include "mainwindow.h"

QFile logfile;
QTextStream ts;

void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    ts << type << ": " << msg << "\n";
    ts.flush();
}

void setupLoggin() 
{
    QSettings greeterSettings(CONFIG_FILE, QSettings::IniFormat);
	qDebug() << "opening settings at:" << CONFIG_FILE;
	qDebug() << "keys:" << greeterSettings.allKeys();
	foreach (QString key , greeterSettings.allKeys()) {
		qDebug() << key << "->" << greeterSettings.value(key);
	}

    if (greeterSettings.contains(LOGFILE_PATH_KEY))
    {
	QString fileName = greeterSettings.value(LOGFILE_PATH_KEY).toString();
	logfile.setFileName(fileName);
	if (logfile.open(QIODevice::WriteOnly | QIODevice::Append)) 
	{
	    ts.setDevice(&logfile);
	    qInstallMessageHandler(messageHandler);	
	}
	else 
	{
	    qWarning() << "Could not open" << fileName; 
	}
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setupLoggin();
    QFile styleFile(":/resources/qt-lightdm-greeter.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = styleFile.readAll();
    qDebug() << "Setting styleSheet:" << styleSheet;
    a.setStyleSheet(styleSheet);

    MainWindow *focusWindow = 0;
    for (int i = 0; i < QApplication::desktop()->screenCount(); ++i)
    {
        MainWindow *w = new MainWindow(i);
        w->show();
        if (w->showLoginForm())
            focusWindow = w;
    }

    // Ensure we set the primary screen's widget as active when there
    // are more screens
    if (focusWindow)
    {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return a.exec();
}

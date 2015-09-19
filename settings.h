#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

bool qt_lightdm_greeter_prepare();


class Settings : public QSettings
{
public:
    Settings();
    QString getLastUser() { return value("last-user").toString(); }
    void setLastUser(QString userId) { setValue("last-user", userId); }
    QString getLastSession(QString userId) { return value(userId + "/last-session").toString(); }
    void setLastSession(QString userId, QString session) { setValue(userId + "/last-session", session); }
};

#endif // SETTINGS_H

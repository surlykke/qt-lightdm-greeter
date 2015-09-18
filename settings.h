#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

/*
#define LAST_USER_FILE "/var/lib/qt-lightdm-greeter/last-user"
#define LAST_USER_KEY "last-user"
#define CONFIG_FILE "/etc/lightdm/qt-lightdm-greeter.conf"
#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGINFORM_OFFSETX_KEY "loginform-offset-x"
#define LOGINFORM_OFFSETY_KEY "loginform-offset-y"
#define LOGFILE_PATH_KEY "logfile-path"
*/


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

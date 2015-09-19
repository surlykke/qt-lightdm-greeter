#include <QDebug>
#include <QDir>
#include "settings.h"

const QString GREETER_DATA_DIR = "/var/lib/lightdm/qt-lightdm-greeter";
const QString GREETER_DATA_FILE = GREETER_DATA_DIR + "/state";
#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGINFORM_OFFSETX_KEY "loginform-offset-x"
#define LOGINFORM_OFFSETY_KEY "loginform-offset-y"
#define LOGFILE_PATH_KEY "logfile-path"

bool qt_lightdm_greeter_prepare()
{
    QDir dir(GREETER_DATA_DIR);
    if (!dir.exists()) {
        if (!dir.mkpath(GREETER_DATA_DIR)) {
            qWarning() << "Unable to create dir" << GREETER_DATA_DIR;
        }
    }
}

Settings::Settings() : QSettings(QString(GREETER_DATA_FILE), QSettings::NativeFormat)
{
}

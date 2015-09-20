#include <QDebug>
#include <QDir>
#include "settings.h"

#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGINFORM_OFFSETX_KEY "loginform-offset-x"
#define LOGINFORM_OFFSETY_KEY "loginform-offset-y"
#define LOGFILE_PATH_KEY "logfile-path"

const QString Cache::GREETER_DATA_DIR_PATH = "/var/lib/lightdm/qt-lightdm-greeter";

void Cache::prepare()
{
    QDir dir(GREETER_DATA_DIR_PATH);
    if (!dir.exists()) {
        if (!dir.mkpath(GREETER_DATA_DIR_PATH)) {
            qWarning() << "Unable to create dir" << GREETER_DATA_DIR_PATH;
        }
    }
}


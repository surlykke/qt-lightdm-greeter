#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define DATA_DIR "qt-lightdm-greeter"

#define LAST_USER_FILE "/var/lib/qt-lightdm-greeter/last-user"
#define LAST_USER_KEY "last-user"
#define CONFIG_FILE "/etc/lightdm/qt-lightdm-greeter.conf"
#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGINFORM_OFFSETX_KEY "loginform-offset-x"
#define LOGINFORM_OFFSETY_KEY "loginform-offset-y"
#define LOGFILE_PATH_KEY "logfile-path"

namespace LXQt {
    namespace LightDmGreeter {
        const QString DATADIR = "qt-lightdm-greeter"; 
    }
}

#endif	/* CONSTANTS_H */


#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define DATA_DIR "lxqt-lightdm-greeter"

#define LAST_USER_FILE "/var/lib/lxqt-lightdm-greeter/last-user"
#define LAST_USER_KEY "last-user"
#define CONFIG_FILE "/etc/lightdm/lxqt-lightdm-greeter.conf"
#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGINFORM_OFFSETX_KEY "loginform-offset-x"
#define LOGINFORM_OFFSETY_KEY "loginform-offset-y"

namespace LxQt {
    namespace LightDmGreeter {
        const QString DATADIR = "lxqt-lightdm-greeter"; 
    }
}

#endif	/* CONSTANTS_H */


# qt-lightdm-greeter

qt-lightdm-greeter is a simple frontend for the lightdm displaymanager, written in c++ and qt5.

It has it's roots in the razor project, where it was developed under the name razor-lightdm-greeter. When razor was merged into LXQt it was renamed to lxqt-lightdm-greeter.

As of september 2015 I have renamed it to qt-lightdm-greeter and cut it's ties to LXQt. This was primarily because LXQt recommends SDDM as its displaymanager and because the application had very little integration with LXQt. 

qt-lightdm-greeter sports a razor-qt logo as a reminder of it's origin.

## Installing

Make sure you have cmake, gcc, qt5 and liblightdm-qt5 (and lightdm) installed on your system

Do
    
```shell
git clone https://github.com/surlykke/qt-lightdm-greeter.git
cd qt-lightdm-greeter
mkdir build
cd build
cmake ..
make 
sudo make install
```

## How to enable qt-lightdm greeter

Update or insert in(to) your `/etc/lightdm/lightdm.conf`, in the `SeatDefaults` section, this line:

    greeter-session=qt-lightdm-greeter

## Configure qt-lightdm-greeter

The file `/etc/lightdm/qt-lightdm-greeter.conf` allows for a 
few configurations of qt-lightdm-greeter (background-image, positioning of loginform). 
The configuration options are documented in that file.


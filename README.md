## SMART HOME EQUIPMENT CONTROL SYSTEM WITH BEAGLEBONE BLACK AND YOCTO
### Flowchart
<picture>
  <img alt="yocto" height="70%" width="70%" src="https://i.imgur.com/i9Jna1w.jpeg">
</picture>

### Hardware requirements
- [Beaglebone Black](https://www.thegioiic.com/beaglebone-black-rev-c-)
- [PID Driver for DC motor](http://www.roboconshop.com/San-Pham/%C4%90ien-tu/Driver-and-controller/PID-Driver-for-DC-motor.aspx)
- [STM32 F103C8T6](https://hshop.vn/products/kit-ra-chon-stm32f103c8t6)
- [RS555](https://icdayroi.com/dong-co-rs555-12-24v)

### Connect

### Setup the environment
```
cd poky/
source oe-init-build-env
```
### Setup in conf/
Example:
#### For BeagleBone Black

MACHINE ?= "beaglebone-yocto"

#### And add your custom layer then add in bblayers.conf file

IMAGE_INSTALL:append = " hello-mod"

IMAGE_INSTALL:append = " myqtapp"

IMAGE_INSTALL:append = " console"

DISTRO_FEATURES:append = " wayland "

IMAGE_INSTALL:append = " qtbase qtwayland qtbase-tools qtbase-plugins "

CORE_IMAGE_EXTRA_INSTALL += " wayland weston "

```
cd poky/build/conf/
git clone https://github.com/XuanThucNgo/Yocto_Custom.git -b my-conf
```
### Building a custom layer for your project
```
cd poky/
git clone https://github.com/XuanThucNgo/Yocto_Custom.git -b meta-mylayer
git clone https://github.com/XuanThucNgo/Yocto_Custom.git -b meta-myApp
```

SUMMARY = "This is a Qt Application"
DESCRIPTION = "This is a Qt Application"
LICENSE = "CLOSED"

#LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=d7bfc32a4337317666d20f2c8c6a8ae1"

DEPENDS += " qtbase wayland "

SRC_URI = "file://dc_driver.cpp \
            file://dc_driver.h \
            file://main.cpp \
            file://mainwindow.cpp \
            file://mainwindow.h \
            file://mainwindow.ui \
            file://MyApp.pro"



S = "${WORKDIR}"

do_install:append () {
	install -d ${D}${bindir}
	install -m 0775 MyApp ${D}${bindir}/
}

FILES_${PN} += "${bindir}/MyApp"

inherit qmake5

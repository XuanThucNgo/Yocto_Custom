DESCRIPTION = "This is a console program"
LICENSE = "CLOSED"

SRC_URI = "file://dc_app.c \
           file://usr_app.c"

S = "${WORKDIR}"

# Ensure LDFLAGS includes the necessary GNU hash style
LDFLAGS:append = " -Wl,--hash-style=gnu"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o dc_app dc_app.c -lm
    ${CC} ${CFLAGS} ${LDFLAGS} -o led_app usr_app.c -lm
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 dc_app ${D}${bindir}/
    install -m 0755 led_app ${D}${bindir}/
}


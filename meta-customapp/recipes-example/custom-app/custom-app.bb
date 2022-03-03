DESCRIPTION = "Custom simple application for project"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://custom-app.c"

S = "${WORKDIR}"

do_compile() {
	${CC} custom-app.c ${LDFLAGS} -o customapp
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 customapp ${D}${bindir}
}

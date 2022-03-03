DESCRIPTION = "Application which implement pthread library"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://concurrent-app.c"

S = "${WORKDIR}"

do_compile() {
	${CC} concurrent-app.c ${LDFLAGS} -o concurrentapp -pthread
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 concurrentapp ${D}${bindir}
}

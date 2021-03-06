require findutils.inc

# GPLv2+ (<< 4.2.32), GPLv3+ (>= 4.2.32)
LICENSE = "GPLv3+"
LIC_FILES_CHKSUM = "file://COPYING;md5=1ebbd3e34237af26da5dc08a4e440464"

DEPENDS = "bison-native"

SRC_URI[sha256sum] = "57127b7e97d91282c6ace556378d5455a9509898297e46e10443016ea1387164"

PACKAGECONFIG[selinux] = "--with-selinux,--without-selinux,libselinux"
# http://savannah.gnu.org/bugs/?27299
CACHED_CONFIGUREVARS += "gl_cv_func_wcwidth_works=yes"

EXTRA_OECONF += "ac_cv_path_SORT=${bindir}/sort"

RDEPENDS:${PN}-ptest += "bash sed grep"

do_install_ptest:class-target() {
	mkdir -p ${D}${PTEST_PATH}/tests/
	cp -r ${S}/tests/* ${D}${PTEST_PATH}/tests/

	# substitute value in run-ptest with actual version
	sed -i -e 's/__run_ptest_version__/${PV}/' ${D}${PTEST_PATH}/run-ptest

}

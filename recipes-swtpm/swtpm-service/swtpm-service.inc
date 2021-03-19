SUMMARY = "Creates a software tpm service"
LICENSE = "Apache-2.0"

inherit systemd

TOOLCHAIN = "clang"

SRC_URI += "file://swtpm.service \
            file://swtpm.sh \
            file://LICENSE \
           "

BPN = "swtpm-service"

INITSCRIPT_NAME = "swtpm"
INITSCRIPT_PARAMS = "defaults"
SYSTEMD_SERVICE_${PN} = "swtpm.service"

do_install_append () {
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm.service ${D}${systemd_unitdir}/system
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/swtpm.sh ${D}${bindir}/swtpm.sh
}

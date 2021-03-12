SUMMARY = "Creates a software tpm service"
LICENSE = "Apache-2.0"

inherit systemd

TOOLCHAIN = "clang"

SRC_URI += "file://swtpm.service \
            file://swtpm_tpm2_startup.service \
            file://swtpm_tpm2_changeauth.service \
            file://LICENSE \
           "

BPN = "swtpm-service"

INITSCRIPT_NAME = "swtpm"
INITSCRIPT_PARAMS = "defaults"
SYSTEMD_SERVICE_${PN} = "swtpm.service swtpm_tpm2_startup.service swtpm_tpm2_changeauth.service"

do_install_append () {
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm.service ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm_tpm2_startup.service ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm_tpm2_changeauth.service ${D}${systemd_unitdir}/system
}

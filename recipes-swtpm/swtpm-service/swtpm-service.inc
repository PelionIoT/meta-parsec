SUMMARY = "A language-agnostic API to secure services in a platform-agnostic way"
HOMEPAGE = "https://github.com/parallaxsecond/parsec"
LICENSE = "Apache-2.0"

inherit systemd

TOOLCHAIN = "clang"

SRC_URI += "file://swtpm_init \
            file://swtpm.service \
            file://swtpm_tpm2_startup.service \
            file://swtpm_tpm2_changeauth.service \
            file://LICENSE \
           "

BPN = "swtpm-service"

INITSCRIPT_NAME = "swtpm"
INITSCRIPT_PARAMS = "defaults"
SYSTEMD_SERVICE_${PN} = "swtpm.service swtpm_tpm2_startup.service swtpm_tpm2_changeauth.service"

do_install_append () {
    install -d ${D}${sysconfdir}/swtpm
    install -m 0600 ${WORKDIR}/swtpm_init ${D}${sysconfdir}/swtpm
    chmod -R 755 ${D}${sysconfdir}/swtpm

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm.service ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm_tpm2_startup.service ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/swtpm_tpm2_changeauth.service ${D}${systemd_unitdir}/system
}

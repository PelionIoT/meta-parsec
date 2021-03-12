SUMMARY = "A language-agnostic API to secure services in a platform-agnostic way"
HOMEPAGE = "https://github.com/parallaxsecond/parsec"
LICENSE = "Apache-2.0"

inherit cargo update-rc.d systemd

TOOLCHAIN = "clang"

SRC_URI += "crate://crates.io/parsec-service/${PV} \
            file://config.toml \
           "

BPN = "parsec-service"

CARGO_BUILD_FLAGS += "${CARGO_EXTRA_FLAGS}"

INITSCRIPT_NAME = "parsec"
INITSCRIPT_PARAMS = "defaults"
SYSTEMD_SERVICE_${PN} = "parsec.service"

do_install_append () {
    install -d ${D}${sysconfdir}/parsec
    install -m 0600 ${WORKDIR}/config.toml ${D}${sysconfdir}/parsec

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${S}/systemd-daemon/parsec.service ${D}${systemd_unitdir}/system
    sed -i -e 's,ExecStart=/usr/libexec/parsec/parsec,ExecStart=/bin/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
    sed -i -e 's,WorkingDirectory=/home/parsec/,WorkingDirectory=/userdata,g' ${D}${systemd_unitdir}/system/parsec.service
    sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=/bin/sleep 10,g' ${D}${systemd_unitdir}/system/parsec.service

    parsec_configure ${D}${sysconfdir}/parsec/config.toml
}

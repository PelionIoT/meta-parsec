SUMMARY = "A language-agnostic API to secure services in a platform-agnostic way"
HOMEPAGE = "https://github.com/parallaxsecond/parsec"
LICENSE = "Apache-2.0"

inherit cargo update-rc.d systemd

TOOLCHAIN = "clang"

SRC_URI += "crate://crates.io/parsec-service/${PV} \
            ${@bb.utils.contains('PARSEC_PROVIDER','SOFTWARE_TPM','file://config-software-tpm.toml','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','HARDWARE_TPM','file://config-hardware-tpm.toml','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','HARDWARE_TPM','file://tpm-udev.rules','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','PKCS11','file://config-pkcs11.toml','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','PKCS11','file://init_pkcs11_slots.sh','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','PKCS11','file://01-tee.rules','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','SOFTHSM','file://config-softhsm.toml','',d)} \
            ${@bb.utils.contains('PARSEC_PROVIDER','SOFTHSM','file://init_pkcs11_slots.sh','',d)} \
           "

BPN = "parsec-service"

CARGO_BUILD_FLAGS += "${CARGO_EXTRA_FLAGS} --features cryptoki/generate-bindings \
                      ${@bb.utils.contains('PARSEC_PROVIDER','SOFTWARE_TPM','--features tss-esapi/generate-bindings','',d)} \
                      ${@bb.utils.contains('PARSEC_PROVIDER','HARDWARE_TPM','--features tss-esapi/generate-bindings','',d)} \
                     "

INITSCRIPT_NAME = "parsec"
INITSCRIPT_PARAMS = "defaults"
SYSTEMD_SERVICE_${PN} = "parsec.service"
FILES_${PN} = "/usr \
               /usr/bin \
               /etc/parsec/config.toml \
               ${@bb.utils.contains('PARSEC_PROVIDER','PKCS11','/etc/udev/rules.d/01-tee.rules','',d)} \
               ${@bb.utils.contains('PARSEC_PROVIDER','HARDWARE_TPM','/etc/udev/rules.d/tpm-udev.rules','',d)} \
               "

cargo_do_install_append () {
    install -d ${D}${bindir}
    install -d -m 0700 ${D}/usr/libexec/parsec
    mv ${D}${bindir}/parsec ${D}/usr/libexec/parsec/parsec
}

do_install_append () {

    if [ "${PARSEC_PROVIDER}" = "SOFTWARE_TPM" ]; then
        install -d ${D}${sysconfdir}/parsec
        install -m 0600 ${WORKDIR}/config-software-tpm.toml ${D}${sysconfdir}/parsec/config.toml
    fi

    if [ "${PARSEC_PROVIDER}" = "HARDWARE_TPM" ]; then
        install -d ${D}${sysconfdir}/parsec
        install -m 0600 ${WORKDIR}/config-hardware-tpm.toml ${D}${sysconfdir}/parsec/config.toml
        install -d ${D}${sysconfdir}/udev/rules.d
        install -m 0644 ${WORKDIR}/tpm-udev.rules ${D}${sysconfdir}/udev/rules.d
    fi

    if [ "${PARSEC_PROVIDER}" = "PKCS11" ]; then
        install -d ${D}${sysconfdir}/parsec
        install -m 0600 ${WORKDIR}/config-pkcs11.toml ${D}${sysconfdir}/parsec/config.toml

        install -d ${D}${bindir}
        install -m 0700 ${WORKDIR}/init_pkcs11_slots.sh ${D}${bindir}/init_pkcs11_slots.sh

        install -d ${D}${sysconfdir}/udev/rules.d
        install -m 0644 ${WORKDIR}/01-tee.rules ${D}${sysconfdir}/udev/rules.d
    fi

    if [ "${PARSEC_PROVIDER}" = "SOFTHSM" ]; then
        install -d ${D}${sysconfdir}/parsec
        install -m 0600 ${WORKDIR}/config-softhsm.toml ${D}${sysconfdir}/parsec/config.toml

        install -d ${D}${bindir}
        install -m 0700 ${WORKDIR}/init_pkcs11_slots.sh ${D}${bindir}/init_pkcs11_slots.sh

    fi

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${S}/systemd-daemon/parsec.service ${D}${systemd_unitdir}/system

    sed -i -e 's,WorkingDirectory=/home/parsec/,WorkingDirectory=/userdata,g' ${D}${systemd_unitdir}/system/parsec.service
    sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=mkdir -m 0750 -p /run/parsec,g' ${D}${systemd_unitdir}/system/parsec.service

    if [ "${PARSEC_PROVIDER}" = "PKCS11" ]; then
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+chown parsec /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+mkdir -m 0700 -p /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+/usr/bin/init_pkcs11_slots.sh,g' ${D}${systemd_unitdir}/system/parsec.service
    fi
    if [ "${PARSEC_PROVIDER}" = "SOFTHSM" ]; then
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+chown parsec /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+mkdir -m 0700 -p /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=/usr/bin/init_pkcs11_slots.sh,g' ${D}${systemd_unitdir}/system/parsec.service
    fi
    if [ "${PARSEC_PROVIDER}" = "HARDWARE_TPM" ]; then
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+chown parsec /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
        sed -i -e 's,\[Service\],\[Service\]\nExecStartPre=+mkdir -m 0700 -p /userdata/parsec,g' ${D}${systemd_unitdir}/system/parsec.service
    fi
    sed -i -e 's,\[Service\],\[Service\]\nRestartSec=5s,g' ${D}${systemd_unitdir}/system/parsec.service
    sed -i -e 's,\[Service\],\[Service\]\nRestart=always,g' ${D}${systemd_unitdir}/system/parsec.service
    sed -i -e 's,\[Service\],\[Service\]\nUser=parsec,g' ${D}${systemd_unitdir}/system/parsec.service

}

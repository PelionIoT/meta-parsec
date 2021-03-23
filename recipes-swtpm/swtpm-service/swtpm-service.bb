# SW TPM service

LICENSE="Apache-2.0"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=1dece7821bf3fd70fe1309eaa37d52a2"

DEPENDS += "ibmswtpm2"
RDEPENDS_${PN} += " ibmswtpm2 tpm2-tools libtss2 libtss2-tcti-mssim"
require swtpm-service.inc



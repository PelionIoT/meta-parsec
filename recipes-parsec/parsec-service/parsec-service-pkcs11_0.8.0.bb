# Parsec service with PKCS11 provider

CARGO_EXTRA_FLAGS = "--features=pkcs11-provider"

# Only 2.3.2 is available in the meta-security layer atm
DEPENDS += "tpm2-tss"
RDEPENDS_${PN} += " optee-sks opensc"

require parsec-service.inc
require parsec-service_${PV}.inc

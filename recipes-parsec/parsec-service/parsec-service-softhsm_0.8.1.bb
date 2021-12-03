# Parsec service with PKCS11 provider

CARGO_EXTRA_FLAGS = "--features=pkcs11-provider"

RDEPENDS_${PN} += " softhsm opensc"

require parsec-service.inc
require parsec-service_${PV}.inc

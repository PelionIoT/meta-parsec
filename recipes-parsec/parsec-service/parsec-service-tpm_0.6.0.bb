# Parsec service with TPM provider

PARSEC_PROVIDER = "tpm"

CARGO_EXTRA_FLAGS = "--features=tpm-provider"

# Only 2.3.2 is available in the meta-security layer atm
DEPENDS += "tpm2-tss"

require parsec-service.inc
require parsec-service_${PV}.inc

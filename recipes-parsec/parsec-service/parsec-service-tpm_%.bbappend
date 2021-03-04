# These paremeters can/should be defined in build/local.conf
PARSEC_TPM_TCTI ?= ""
PARSEC_TPM_AUTH ?= ""

# Change values in the default Parsec config file
parsec_configure() {
    cfg_file=$1

    if [ ! -z ${PARSEC_TPM_TCTI} ]; then
        sed -i -e 's,^.*tcti = .*$,tcti = "${PARSEC_TPM_TCTI}",' ${cfg_file}
    fi
    if [ ! -z ${PARSEC_TPM_AUTH} ]; then
        sed -i -e 's,^.*owner_hierarchy_auth = .*$,owner_hierarchy_auth = "${PARSEC_TPM_AUTH}",' ${cfg_file}
    fi
}

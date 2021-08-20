#!/bin/sh

# SWTPM startup script, to be called from the systemd service.

echo "SWTPM Startup"

cd /userdata/parsec

if [ ! -f .HWTPM_Initialised ]; then
    # No HWTPM_Initialised file found, so treat this as the first run
    echo "No HWTPM_Initialised found"
    set -x
    /bin/tpm2_startup -c -T device:/dev/tpm0
    set +x
    date > ./.HWTPM_Initialised
else
    echo "HWTPM_Initialised found"
    set -x
    /bin/tpm2_startup -T device:/dev/tpm0
    set +x
fi

echo "SWTPM Startup Complete"


#!/bin/sh

# SWTPM startup script, to be called from the systemd service.

echo "SWTPM Startup"

cd /userdata/parsec

if [ ! -f NVChip ]; then
    # No NVChip file found, so treat this as the first run
    echo "No NVChip found"
    set -x
    /bin/tpm_server &
    /bin/sleep 5
    /bin/tpm2_startup -c -T mssim
    /bin/tpm2_changeauth -c owner tpm_pass
else
    echo "NVChip found"
    set -x
    /bin/tpm_server &
    /bin/sleep 5
    /bin/tpm2_startup -T mssim
fi
set +x
echo "SWTPM Startup Complete"

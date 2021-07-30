#! /bin/sh

# Work out how many slots are present, and how many are uninitialised.
# If none of them are initialised then got ahead and initialise them.

MODULE=/usr/lib/libsks.so.0
SO_PIN=12345678
PIN=87654321

num_slots=$(pkcs11-tool --module $MODULE --list-slots | grep -c "^Slot")
num_unintialized_slots=$(pkcs11-tool --module $MODULE --list-slots | grep -c "uninitialized")

if [ $num_slots -gt 0 ]; then
    if [ $num_unintialized_slots -eq $num_slots ]; then
        pkcs11-tool --module $MODULE --init-token --label "parsec" --so-pin ${SO_PIN}
        pkcs11-tool --module $MODULE --init-pin --so-pin ${SO_PIN} --pin ${PIN}
    fi
fi



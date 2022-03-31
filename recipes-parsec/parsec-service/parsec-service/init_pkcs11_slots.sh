#! /bin/sh

# Work out how many slots are present, and how many are uninitialised.
# If none of them are initialised then got ahead and initialise them.

# Select a module file, either for optee-sks or softhsm
if [ -e /usr/lib/libsks.so.0 ]; then
    MODULE=/usr/lib/libsks.so.0
elif [ -e /usr/lib/softhsm/libsofthsm2.so ]; then
    MODULE=/usr/lib/softhsm/libsofthsm2.so
else
    echo "Cannot find a MODULE library"
    exit 2
fi

SO_PIN=12345678
PIN=87654321

num_slots=$(pkcs11-tool --module $MODULE --list-slots | grep -c "^Slot")
num_unintialized_slots=$(pkcs11-tool --module $MODULE --list-slots | grep -c "uninitialized")

if [ "$num_slots" -gt 0 ]; then
    if [ "$num_unintialized_slots" -eq "$num_slots" ]; then
        pkcs11-tool --module $MODULE --init-token --label "parsec" --so-pin ${SO_PIN}
        pkcs11-tool --module $MODULE --init-pin --so-pin ${SO_PIN} --pin ${PIN}
    fi
fi

if [ "$MODULE" = /usr/lib/softhsm/libsofthsm2.so ]; then
    if [ ! -f /userdata/parsec/config.toml ]; then
        # For the softhsm, the slot number is allocated at random during the init.
        # Update the parsec config file with the new value.
        slot=$(softhsm2-util --show-slots |   grep -m 1 ^Slot | cut -d" " -f2)
        sed -e "s,slot_number = [0-9]*,slot_number = $slot,g" /etc/parsec/config.toml > /userdata/parsec/config.toml
    fi
fi



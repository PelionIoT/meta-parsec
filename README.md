# meta-parsec layer


This layer contains recipes for the Parsec service with either a hardware TPM or a software TPM or a PKCS11 provider service.

## Hardware TPM Provider

### Hardware TPM Password

By default Parsec and hardware TPM are configured to use an empty password.

To change this perform the following steps:

1. Stop the Parsec service `sudo systemctl stop parsec`.
1. Change the owner_hierarchy_auth `sudo tpm2_changeauth -c owner new_pass`.
1. Edit the Parsec configuration file in `/etc/parsec/config.toml` and change the owner_hierarchy_auth line to your new_pass.
1. Restart the Parsec service `sudo systemctl start parsec`.

## PKCS11 Provider

There are two different possible PKCS11 providers that use different technologies to store secrets. Parsec configuration file in `/etc/parsec/config.toml` selects the provider by selecting a library file.

- Uses the OP-TEE trusted application.
  - This is used when **PARSEC_PROVIDER** is set to **PKCS11**.
  - To use this the library to be used is `/usr/lib/libsks.so.0`.
- Uses a software HSM.
  - This is used when **PARSEC_PROVIDER** is set to **SOFTHSM**.
  - To use this the library to be used is `/usr/lib/softhsm/libsofthsm2.so`.

### PKCS11 Pin

By default both the Parsec and PKCS11 are configured to use the pins **12345678** and **87654321**.
This **MUST** be changed as part of the factory setup process.

To change this perform the following steps:

1. Use the appropriate library file depending on the **PARSEC_PROVIDER** as described above
1. Stop the Parsec service `sudo systemctl stop parsec`
1. Change the User pin:
   - `sudo pkcs11-tool --module /usr/lib/libsks.so.0 --change-pin --pin 87654321 --new-pin new_user_pin`  
   OR  
   - `sudo pkcs11-tool --module /usr/lib/softhsm/libsofthsm2.so --change-pin --pin 87654321 --new-pin new_user_pin`

1. Change the Security Officer pin:
   - `sudo pkcs11-tool --module /usr/lib/libsks.so.0 --login --login-type so --so-pin 12345678 --change-pin --new-pin new_so_pin`  
   OR  
   - `sudo pkcs11-tool --module /usr/lib/softhsm/libsofthsm2.so --login --login-type so --so-pin 12345678 --change-pin --new-pin new_so_pin`
1. Edit the Parsec configuration file in `/etc/parsec/config.toml` and change the user_pin line to your new_user_pin.
1. Restart the Parsec service `sudo systemctl start parsec`.

## Software TPM Provider

### Software TPM Password

By default both the Parsec and software TPM are configured to use the password **tpm_pass**.
This **MUST** be changed as part of the factory setup process.

To change this perform the following steps:

1. Stop the Parsec service `sudo systemctl stop parsec`.
1. Change the owner_hierarchy_auth `sudo tpm2_changeauth -c -p tpm_pass new_auth_pass`.
1. Edit the Parsec configuration file in `/etc/parsec/config.toml` and change the owner_hierarchy_auth line to your new_auth_pass.
1. Restart the Parsec service `sudo systemctl start parsec`.

### Stopping and restarting the Services

The services must be shutdown in the correct order.

1. Stop the Parsec service `sudo systemctl stop parsec`.
1. Stop the SW TPM service `sudo systemctl stop swtpm`.

To restart them:

1. Start the SW TPM service `sudo systemctl start swtpm`.
1. Start the Parsec service `sudo systemctl start parsec`.


## Dependencies

This layer depends on:

    URI: git://git.openembedded.org/openembedded-core
    branch: master
    revision: HEAD
    prio: default

    URI: git://git.openembedded.org/meta-openembedded/meta-oe
    branch: master
    revision: HEAD
    prio: default

    URI git://git.yoctoproject.org/meta-security
    branch: master
    revision: HEAD
    prio: default

## Adding the meta-parsec layer to your build

In order to use this layer, you need to make the build system aware of
it.

Assuming this layer exists at the top-level of your
Yocto build tree, you can add it to the build system by adding the
location of the meta-parsec layer to `bblayers.conf`, along with any
other layers needed. e.g.:

```
    BBLAYERS ?= " \
      /path/to/yocto/meta \
      /path/to/yocto/meta-yocto \
      /path/to/yocto/meta-yocto-bsp \
      /path/to/yocto/meta-openembedded/meta-oe \
      /path/to/yocto/meta-parsec \
      "
```

meta-parsec layer
==============

This layer contains recipes for the Parsec service with a software TPM provider.

Dependencies
============

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

    URI https://github.com/meta-rust/meta-rust.git
    branch: master
    revision: HEAD
    prio: default

    URI https://github.com/kraj/meta-clang.git
    branch: master
    revision: HEAD
    prio: default


Adding the meta-parsec layer to your build
========================================

In order to use this layer, you need to make the build system aware of
it.

Assuming this layer exists at the top-level of your
yocto build tree, you can add it to the build system by adding the
location of the meta-parsec layer to bblayers.conf, along with any
other layers needed. e.g.:

    BBLAYERS ?= " \
      /path/to/yocto/meta \
      /path/to/yocto/meta-yocto \
      /path/to/yocto/meta-yocto-bsp \
      /path/to/yocto/meta-openembedded/meta-oe \
      /path/to/yocto/meta-rust \
      /path/to/yocto/meta-clang \
      /path/to/yocto/meta-security/meta-tpm \
      /path/to/yocto/meta-security/meta-parsec \
      "

To include the Parsec service into your image add the following into the
local.conf:

    IMAGE_INSTALL_append = " parsec-service-tpm"
    IMAGE_INSTALL_append = " swtpm-service"
    IMAGE_INSTALL_append = " ibmswtpm2"
    IMAGE_INSTALL_append = " tpm2-tools"
    IMAGE_INSTALL_append = " tpm2-tss"
    IMAGE_INSTALL_append = " libtss2"
    IMAGE_INSTALL_append = " libtss2-tcti-mssim"
    
The Parsec service will be deployed into the image with a config file for
the requested provider.


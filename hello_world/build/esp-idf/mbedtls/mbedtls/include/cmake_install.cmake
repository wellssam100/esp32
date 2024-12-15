# Install script for directory: /mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/swubu/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/build_info.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/mnt/c/Users/wells/Workspace/ESP32/ESP-IDF/esp-idf-v5.2.3/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

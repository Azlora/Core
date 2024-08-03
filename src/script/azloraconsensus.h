// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2024 The Azlora Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef AZLORA_SCRIPT_AZLORACONSENSUS_H
#define AZLORA_SCRIPT_AZLORACONSENSUS_H

#include <stdint.h>

#if defined(BUILD_AZLORA_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/azlora-config.h>
  #if defined(_WIN32)
    #if defined(HAVE_DLLEXPORT_ATTRIBUTE)
      #define EXPORT_SYMBOL __declspec(dllexport)
    #else
      #define EXPORT_SYMBOL
    #endif
  #elif defined(HAVE_DEFAULT_VISIBILITY_ATTRIBUTE)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBAZLORACONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define AZLORACONSENSUS_API_VER 1

typedef enum azloraconsensus_error_t
{
    azloraconsensus_ERR_OK = 0,
    azloraconsensus_ERR_TX_INDEX,
    azloraconsensus_ERR_TX_SIZE_MISMATCH,
    azloraconsensus_ERR_TX_DESERIALIZE,
    azloraconsensus_ERR_AMOUNT_REQUIRED,
    azloraconsensus_ERR_INVALID_FLAGS,
} azloraconsensus_error;

/** Script verification flags */
enum
{
    azloraconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    azloraconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    azloraconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    azloraconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    azloraconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    azloraconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    azloraconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    azloraconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = azloraconsensus_SCRIPT_FLAGS_VERIFY_P2SH | azloraconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               azloraconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | azloraconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               azloraconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | azloraconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int azloraconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, azloraconsensus_error* err);

EXPORT_SYMBOL int azloraconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, azloraconsensus_error* err);

EXPORT_SYMBOL unsigned int azloraconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // AZLORA_SCRIPT_AZLORACONSENSUS_H

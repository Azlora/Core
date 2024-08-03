// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2024 The Azlora Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef AZLORA_WALLET_SALVAGE_H
#define AZLORA_WALLET_SALVAGE_H

#include <streams.h>
#include <util/fs.h>

class ArgsManager;
struct bilingual_str;

namespace wallet {
bool RecoverDatabaseFile(const ArgsManager& args, const fs::path& file_path, bilingual_str& error, std::vector<bilingual_str>& warnings);
} // namespace wallet

#endif // AZLORA_WALLET_SALVAGE_H

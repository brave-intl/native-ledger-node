/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_C_NODE_ADDON_API_NODE_LEDGER_URL_LOADER_H_
#define SRC_C_NODE_ADDON_API_NODE_LEDGER_URL_LOADER_H_

#include <string>
#include "bat/ledger/ledger_url_loader.h"

#include "bat/ledger/export.h"

using ledger::LedgerURLLoader;

class LEDGER_EXPORT NodeLedgerURLLoader : public LedgerURLLoader {
 public:
  NodeLedgerURLLoader();
  void Start() override;
  uint64_t request_id() override;
};

#endif  // SRC_C_NODE_ADDON_API_NODE_LEDGER_URL_LOADER_H_

/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <napi.h>

#include "node_ledger_wrap.h"  // NOLINT(build/include)

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  NodeLedgerWrap::Init(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
//  NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

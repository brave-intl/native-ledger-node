/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_C_NODE_ADDON_API_NODE_LEDGER_WRAP_H_
#define SRC_C_NODE_ADDON_API_NODE_LEDGER_WRAP_H_

#include "node_ledger_client_proxy.h"  // NOLINT(build/include)

#include <napi.h>
#include <memory>

#include "bat/ledger/ledger.h"

using ledger::Ledger;
using ledger::LedgerClient;
using ledger::LedgerCallbackHandler;

class NodeLedgerClientProxy;

class NodeLedgerWrap : public Napi::ObjectWrap<NodeLedgerWrap> {
 public:
  static Napi::FunctionReference constructor;
  explicit NodeLedgerWrap(const Napi::CallbackInfo &info);
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  void CreateInstance(const Napi::CallbackInfo& info);
  void CreateWallet(const Napi::CallbackInfo& info);
  void Reconcile(const Napi::CallbackInfo& info);
  void OnLoad(const Napi::CallbackInfo& info);
  void OnUnload(const Napi::CallbackInfo& info);
  void OnShow(const Napi::CallbackInfo& info);
  void OnHide(const Napi::CallbackInfo& info);
  void OnForeground(const Napi::CallbackInfo& info);
  void OnBackground(const Napi::CallbackInfo& info);
  void OnMediaStart(const Napi::CallbackInfo& info);
  void OnMediaStop(const Napi::CallbackInfo& info);
  void OnXHRLoad(const Napi::CallbackInfo& info);
  void SetPublisherInfo(const Napi::CallbackInfo& info);
  void GetPublisherInfo(const Napi::CallbackInfo& info);
  void GetPublisherInfoList(const Napi::CallbackInfo& info);
  void SetPublisherMinVisitTime(const Napi::CallbackInfo& info);
  void SetPublisherMinVisits(const Napi::CallbackInfo& info);
  void SetPublisherAllowNonVerified(const Napi::CallbackInfo& info);
  void SetContributionAmount(const Napi::CallbackInfo& info);
  Napi::Value GetBATAddress(const Napi::CallbackInfo& info);
  Napi::Value GetBTCAddress(const Napi::CallbackInfo& info);
  Napi::Value GetETHAddress(const Napi::CallbackInfo& info);
  Napi::Value GetLTCAddress(const Napi::CallbackInfo& info);
  Napi::Value GetPublisherMinVisitTime(const Napi::CallbackInfo& info);
  Napi::Value GetPublisherMinVisits(const Napi::CallbackInfo& info);
  Napi::Value GetPublisherAllowNonVerified(const Napi::CallbackInfo& info);
  Napi::Value GetContributionAmount(const Napi::CallbackInfo& info);
  void GetWalletProperties(const Napi::CallbackInfo& info);
  void OnSavePublisherInfo(Napi::Function cb, ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> publisher_info);
  Napi::ObjectReference* GetJSClientReference() { return &ref; }

 private:
  NodeLedgerClientProxy* clientProxy_;
  Ledger* ledger;
  Napi::ObjectReference ref;
};

#endif  // SRC_C_NODE_ADDON_API_NODE_LEDGER_WRAP_H_

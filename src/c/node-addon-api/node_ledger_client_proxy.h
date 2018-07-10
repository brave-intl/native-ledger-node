/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SRC_C_NODE_ADDON_API_NODE_LEDGER_CLIENT_PROXY_H_
#define SRC_C_NODE_ADDON_API_NODE_LEDGER_CLIENT_PROXY_H_

#include <napi.h>
#include <memory>
#include <string>
#include <vector>

#include "node_ledger_wrap.h"  // NOLINT(build/include)
#include "bat/ledger/ledger.h"
#include "bat/ledger/ledger_client.h"
#include "bat/ledger/ledger_task_runner.h"

#include "bat/ledger/publisher_info.h"

using ledger::LedgerClient;
using ledger::Result;
using ledger::LedgerCallbackHandler;
using ledger::LedgerURLLoader;
using ledger::LedgerTaskRunner;
using ledger::PublisherInfoFilter;
using ledger::PublisherInfo;
using ledger::PublisherInfoList;

using PublisherInfoCallback = std::function<void(Result,
    std::unique_ptr<PublisherInfo>)>;
using GetPublisherInfoListCallback =
    std::function<void(const PublisherInfoList&, uint32_t /* next_record */)>;

class NodeLedgerWrap;

class NodeLedgerClientProxy : public LedgerClient {
 public:
  explicit NodeLedgerClientProxy(NodeLedgerWrap* ledgerWrap);
  ~NodeLedgerClientProxy() {}

  // overrides
  std::string GenerateGUID() const override;
  void OnWalletInitialized(Result result) override;
  void GetWalletProperties() override;
  void OnWalletProperties(Result result, std::unique_ptr<ledger::WalletInfo>
    walletInfo) override;
  void OnReconcileComplete(Result result,
                           const std::string& viewing_id) override;
  void LoadLedgerState(LedgerCallbackHandler* handler) override;
  void SaveLedgerState(const std::string& ledger_state,
                       LedgerCallbackHandler* handler) override;
  void LoadPublisherState(LedgerCallbackHandler* handler) override;
  void SavePublisherState(const std::string& publisher_state,
                          LedgerCallbackHandler* handler) override;
  void SavePublisherInfo(std::unique_ptr<PublisherInfo> publisher_info,
                         PublisherInfoCallback callback) override;
  void LoadPublisherInfo(const PublisherInfo::id_type& publisher_id,
                         PublisherInfoCallback callback) override;
  void LoadPublisherInfoList(uint32_t start, uint32_t limit,
                             PublisherInfoFilter filter,
                             GetPublisherInfoListCallback callback) override;
  std::unique_ptr<LedgerURLLoader> LoadURL(const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    const ledger::URL_METHOD& method,
    LedgerCallbackHandler* handler) override;
  void RunIOTask(std::unique_ptr<LedgerTaskRunner> task) override;
  void RunTask(std::unique_ptr<LedgerTaskRunner> task) override;

  void GetPromotion(const std::string& lang,
    const std::string& paymentId) override;
  void OnPromotion(ledger::Promo) override;
  void GetPromotionCaptcha() override;
  void OnPromotionCaptcha(const std::string& image) override;
  void OnPromotionFinish(Result result,
    unsigned int statusCode,
    uint64_t expirationDate) override;
  void OnRecoverWallet(Result result, double balance) override;
  std::string URIEncode(const std::string& value) override;

  void OnSavePublisherInfo(Napi::Function cb, ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> publisher_info);

 private:
  Napi::ObjectReference* jsClient;
};

#endif  // SRC_C_NODE_ADDON_API_NODE_LEDGER_CLIENT_PROXY_H_

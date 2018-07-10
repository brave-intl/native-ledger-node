/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "node_ledger_client_proxy.h"  // NOLINT(build/include)

#include "node_ledger_url_loader.h"  // NOLINT(build/include)

#include <napi.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <map>

using ledger::Ledger;
using ledger::WalletInfo;
using ledger::PublisherInfoCallback;

LedgerCallbackHandler* callbackHandler;
PublisherInfoCallback publisherInfoCallback;
std::unique_ptr<PublisherInfo> publisherInfoRef;

NodeLedgerClientProxy::NodeLedgerClientProxy(NodeLedgerWrap* ledgerWrap) {
  jsClient = ledgerWrap->GetJSClientReference();
}

std::string NodeLedgerClientProxy::GenerateGUID() const {
  Napi::Function generateGUID =
    jsClient->Get("generateGUID").As<Napi::Function>();
  return generateGUID.Call({}).ToString();
}

void NodeLedgerClientProxy::OnWalletInitialized(Result result) {
  Napi::Function onWalletCreated =
    jsClient->Get("onWalletCreated").As<Napi::Function>();
  onWalletCreated.Call({});
}


std::string NodeLedgerClientProxy::URIEncode(const std::string& value) {
  return "";
}

void NodeLedgerClientProxy::GetWalletProperties() {
  Napi::Function getWalletProperties =
    jsClient->Get("getWalletProperties").As<Napi::Function>();
  getWalletProperties.Call({});
}

void NodeLedgerClientProxy::OnWalletProperties(Result result,
  std::unique_ptr<ledger::WalletInfo> info) {
  Napi::Function onWalletProperties =
    jsClient->Get("onWalletProperties").As<Napi::Function>();
  Napi::Object walletInfo = Napi::Object::New(jsClient->Env());

  walletInfo.Set(Napi::String::New(jsClient->Env(), "altCurrency"),
    info->altcurrency_);
  walletInfo.Set(Napi::String::New(jsClient->Env(), "probi"), info->probi_);
  walletInfo.Set(Napi::String::New(jsClient->Env(), "balance"), info->balance_);

  Napi::Array rates = Napi::Array::New(jsClient->Env(), info->rates_.size());
  for (std::map<std::string, double>::iterator iter = info->rates_.begin();
    iter != info->rates_.end();
    ++iter) {
    std::string key = iter->first;
    double value = iter->second;
    rates.Set(Napi::String::New(jsClient->Env(), key), value);
  }
  walletInfo.Set(Napi::String::New(jsClient->Env(), "rates"), rates);

  Napi::Array choices = Napi::Array::New(jsClient->Env(),
    info->parameters_choices_.size());
  for (size_t ix = 0; ix < info->parameters_choices_.size() ; ix++) {
    choices.Set(ix, Napi::Number::New(jsClient->Env(),
      info->parameters_choices_.at(ix)));
  }
  walletInfo.Set(Napi::String::New(jsClient->Env(), "parametersChoices"),
    choices);

  Napi::Array range = Napi::Array::New(jsClient->Env(),
    info->parameters_range_.size());
  for (size_t ix = 0; ix < info->parameters_range_.size(); ix++) {
    range.Set(ix, Napi::Number::New(jsClient->Env(),
      info->parameters_range_.at(ix)));
  }
  walletInfo.Set(Napi::String::New(jsClient->Env(), "parametersRange"), range);

  walletInfo.Set(Napi::String::New(jsClient->Env(), "parametersDays"),
    info->parameters_days_);

  Napi::Array grants = Napi::Array::New(jsClient->Env(), info->grants_.size());
  for (size_t ix = 0; ix < info->grants_.size(); ix++) {
    Napi::Object grant = Napi::Object::New(jsClient->Env());
    grant.Set(Napi::String::New(jsClient->Env(), "altCurrency"),
      info->grants_.at(ix).altcurrency);
    grant.Set(Napi::String::New(jsClient->Env(), "probi"),
      info->grants_.at(ix).probi);
    grant.Set(Napi::String::New(jsClient->Env(), "expiryTime"),
      info->grants_.at(ix).expiryTime);
    grants.Set(Napi::Object::New(jsClient->Env()), grant);
  }
  walletInfo.Set(Napi::String::New(jsClient->Env(), "grants"), grants);
  onWalletProperties.Call({ walletInfo });
}

void NodeLedgerClientProxy::OnReconcileComplete(Result result,
  const std::string& viewing_id) {
  Napi::Function onReconcileComplete =
    jsClient->Get("onReconcileComplete").As<Napi::Function>();
  Napi::Object results = Napi::Object::New(jsClient->Env());
  Napi::Object jsResult = Napi::Object::New(jsClient->Env());
  jsResult.Set(Napi::String::New(jsClient->Env(), "OK"), 0);
  jsResult.Set(Napi::String::New(jsClient->Env(), "ERROR"), 1);
  results.Set(Napi::String::New(jsClient->Env(), "result"), jsResult);
  Napi::String viewingId = Napi::String::New(jsClient->Env(), viewing_id);
  onReconcileComplete.Call({results, viewingId});
}

void LoadLedgerStateCallback
  (const Napi::CallbackInfo& info) {
}

void NodeLedgerClientProxy::LoadLedgerState(LedgerCallbackHandler* handler) {
  Napi::Function loadLedgerState =
    jsClient->Get("loadLedgerState").As<Napi::Function>();
  loadLedgerState.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), LoadLedgerStateCallback));
  loadLedgerState.Call({ loadLedgerState.Get("cb").As<Napi::Function>() });
}

void SaveLedgerStateCallback
  (const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    std::cout << "back in native SaveLedgerStateCallback" << std::endl;
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env,
        "expected numerical argument to callback")
        .ThrowAsJavaScriptException();
    }
    Result result = *(info[0].As<Napi::External<ledger::Result>>().Data());
    callbackHandler->OnLedgerStateSaved(result);
}

void NodeLedgerClientProxy::SaveLedgerState(const std::string& ledger_state,
  LedgerCallbackHandler* handler) {
  callbackHandler = handler;
  Napi::Function saveLedgerState =
    jsClient->Get("saveLedgerState").As<Napi::Function>();
  Napi::String ledgerState = Napi::String::New(jsClient->Env(), ledger_state);
  saveLedgerState.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), SaveLedgerStateCallback));

  saveLedgerState.Call({ ledgerState, saveLedgerState.Get
    ("cb").As<Napi::Function>() });
}

void LoadPublisherStateCallback
  (const Napi::CallbackInfo& info) {
    std::cout << "back in native LoadPublisherStateCallback" << std::endl;
}

void NodeLedgerClientProxy::LoadPublisherState
  (LedgerCallbackHandler* handler) {
  Napi::Function loadPublisherState =
    jsClient->Get("loadPublisherState").As<Napi::Function>();
  loadPublisherState.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), LoadPublisherStateCallback));
  loadPublisherState.Call({ loadPublisherState.Get
    ("cb").As<Napi::Function>() });
}

void SavePublisherStateCallback
  (const Napi::CallbackInfo& info) {
    std::cout << "back in native SavePublisherStateCallback" << std::endl;
}

void NodeLedgerClientProxy::SavePublisherState
  (const std::string& publisher_state,
  LedgerCallbackHandler* handler) {
  Napi::Function savePublisherState =
    jsClient->Get("savePublisherState").As<Napi::Function>();
  Napi::String publisherState = Napi::String::New(jsClient->Env(),
    publisher_state);
  savePublisherState.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), SavePublisherStateCallback));
  savePublisherState.Call({ publisherState, savePublisherState.Get
    ("cb").As<Napi::Function>() });
}

void SavePublisherInfoCallback
  (const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    std::cout << "back in native SavePublisherInfoCallback" << std::endl;
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env,
        "expected numerical argument to callback")
        .ThrowAsJavaScriptException();
    }
    PublisherInfoCallback callback = publisherInfoCallback;
    Result cbResult = static_cast<Result>(info[0].As<Napi::Number>()
      .Int32Value());
    callback(cbResult, std::move(publisherInfoRef));
}

void NodeLedgerClientProxy::OnSavePublisherInfo(Napi::Function callback,
  ledger::Result result,
  std::unique_ptr<ledger::PublisherInfo> publisher_info) {
  std::cout << "<<<OnSavePublisherInfo>>>" << std::endl;
  Napi::Number results = Napi::Number::New(jsClient->Env(), result);

  Napi::Object publisherInfo = Napi::Object::New(jsClient->Env());
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "id"),
    publisher_info->id);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "duration"),
    publisher_info->duration);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "score"),
    publisher_info->score);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "visits"),
    publisher_info->visits);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "pinned"),
    publisher_info->pinned);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "percent"),
    publisher_info->percent);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "weight"),
    publisher_info->weight);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "excluded"),
    publisher_info->excluded);

  callback.Call({ results, publisherInfo });
}

void NodeLedgerClientProxy::SavePublisherInfo
  (std::unique_ptr<PublisherInfo> publisher_info,
  PublisherInfoCallback callback) {
  publisherInfoCallback = callback;
  Napi::Function savePublisherInfo =
    jsClient->Get("savePublisherInfo").As<Napi::Function>();
  Napi::Object publisherInfo = Napi::Object::New(jsClient->Env());
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "id"),
    publisher_info->id);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "duration"),
    publisher_info->duration);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "score"),
    publisher_info->score);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "visits"),
    publisher_info->visits);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "pinned"),
    publisher_info->pinned);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "percent"),
    publisher_info->percent);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "weight"),
    publisher_info->weight);
  publisherInfo.Set(Napi::String::New(jsClient->Env(), "excluded"),
    publisher_info->excluded);

  publisherInfoRef = std::move(publisher_info);

  savePublisherInfo.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), SavePublisherInfoCallback));
  savePublisherInfo.Call({ publisherInfo, savePublisherInfo.Get
    ("cb").As<Napi::Function>() });
}

void NodeLedgerClientProxy::LoadPublisherInfo
  (const PublisherInfo::id_type& publisher_id,
  PublisherInfoCallback callback) {
  Napi::Function loadPublisherInfo =
    jsClient->Get("loadPublisherInfo").As<Napi::Function>();
  loadPublisherInfo.Call({});
}

void NodeLedgerClientProxy::LoadPublisherInfoList
  (uint32_t start, uint32_t limit,
  PublisherInfoFilter filter,
  GetPublisherInfoListCallback callback) {
  Napi::Function loadPublisherInfoList =
    jsClient->Get("loadPublisherInfoList").As<Napi::Function>();
  loadPublisherInfoList.Call({});
}


void OnURLRequestResponseCallback(const Napi::CallbackInfo& info) {
  std::cout << "<<<back in native OnURLRequestResponseCallback>>>"
    << std::endl;
  // callbackHandler->OnURLRequestResponse(request_id, response_code, response);
}

std::unique_ptr<LedgerURLLoader> NodeLedgerClientProxy::LoadURL
  (const std::string& url,
  const std::vector<std::string>& headers,
  const std::string& content,
  const std::string& contentType,
  const ledger::URL_METHOD& method,
  LedgerCallbackHandler* handler) {
  callbackHandler = handler;
  Napi::Function loadURL =
    jsClient->Get("loadURL").As<Napi::Function>();
  loadURL.Set(Napi::String::New(jsClient->Env(), "url"), url);

  Napi::Array headerArray = Napi::Array::New(jsClient->Env(), headers.size());
  for (uint64_t ix = 0; ix < headers.size(); ix++) {
    headerArray[ix] = headers[ix];
  }
  loadURL.Set(Napi::String::New(jsClient->Env(), "cb"),
    Napi::Function::New(jsClient->Env(), OnURLRequestResponseCallback));
  Napi::Value urlResult = loadURL.Call({ Napi::String::New
    (jsClient->Env(), url), headerArray, Napi::String::New
    (jsClient->Env(), content), Napi::String::New
    (jsClient->Env(), contentType),
    Napi::Number::New(jsClient->Env(), method), loadURL.Get("cb")
    .As<Napi::Function>() });

  std::unique_ptr<NodeLedgerURLLoader> ldrPtr;
  ldrPtr.reset(new NodeLedgerURLLoader());

  return std::move(ldrPtr);
}

void NodeLedgerClientProxy::GetPromotion
  (const std::string& lang, const std::string& paymentId) {
}

void NodeLedgerClientProxy::OnPromotion(ledger::Promo) {
}

void NodeLedgerClientProxy::GetPromotionCaptcha() {
}

void NodeLedgerClientProxy::OnPromotionCaptcha(const std::string& image) {
}

void NodeLedgerClientProxy::RunIOTask(std::unique_ptr<LedgerTaskRunner> task) {
}

void NodeLedgerClientProxy::RunTask(std::unique_ptr<LedgerTaskRunner> task) {
}

void NodeLedgerClientProxy::OnRecoverWallet(Result result, double balance) {
}

void NodeLedgerClientProxy::OnPromotionFinish(Result result,
  unsigned int statusCode,
    uint64_t expirationDate) {
}

/* Copyright (c) 2018 Brave Software. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "node_ledger_client_proxy.h"  // NOLINT(build/include)
#include "node_ledger_wrap.h"          // NOLINT(build/include)

#include <napi.h>

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <map>

using ledger::Ledger;
using ledger::VisitData;
using ledger::LedgerCallbackHandler;

using PublisherInfoCallback = std::function<void(Result,
    std::unique_ptr<PublisherInfo>)>;

using namespace std::placeholders;  // NOLINT

Napi::FunctionReference NodeLedgerWrap::constructor;

Napi::Object NodeLedgerWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function tpl = DefineClass(env, "NodeLedger", {
    InstanceMethod("createWallet", &NodeLedgerWrap::CreateWallet),
    InstanceMethod("reconcile", &NodeLedgerWrap::Reconcile),
    InstanceMethod("onLoad", &NodeLedgerWrap::OnLoad),
    InstanceMethod("onUnload", &NodeLedgerWrap::OnUnload),
    InstanceMethod("onShow", &NodeLedgerWrap::OnShow),
    InstanceMethod("onHide", &NodeLedgerWrap::OnHide),
    InstanceMethod("onForeground", &NodeLedgerWrap::OnForeground),
    InstanceMethod("onBackground", &NodeLedgerWrap::OnBackground),
    InstanceMethod("onMediaStart", &NodeLedgerWrap::OnMediaStart),
    InstanceMethod("onMediaStop", &NodeLedgerWrap::OnMediaStop),
    InstanceMethod("onXHRLoad", &NodeLedgerWrap::OnXHRLoad),
    InstanceMethod("getPublisherAllowNonVerified",
      &NodeLedgerWrap::GetPublisherAllowNonVerified),
    InstanceMethod("setPublisherAllowNonVerified",
      &NodeLedgerWrap::SetPublisherAllowNonVerified),
    InstanceMethod("getPublisherMinVisitTime",
      &NodeLedgerWrap::GetPublisherMinVisitTime),
    InstanceMethod("setPublisherMinVisitTime",
      &NodeLedgerWrap::SetPublisherMinVisitTime),
    InstanceMethod("getPublisherMinVisits",
      &NodeLedgerWrap::GetPublisherMinVisits),
    InstanceMethod("setPublisherMinVisits",
      &NodeLedgerWrap::SetPublisherMinVisits),
    InstanceMethod("setContributionAmount",
      &NodeLedgerWrap::SetContributionAmount),
    InstanceMethod("getContributionAmount",
      &NodeLedgerWrap::GetContributionAmount),
    InstanceMethod("getBATAddress", &NodeLedgerWrap::GetBATAddress),
    InstanceMethod("getBTCAddress", &NodeLedgerWrap::GetBTCAddress),
    InstanceMethod("getETHAddress", &NodeLedgerWrap::GetETHAddress),
    InstanceMethod("getLTCAddress", &NodeLedgerWrap::GetLTCAddress),
    InstanceMethod("getWalletProperties",
      &NodeLedgerWrap::GetWalletProperties),
    InstanceMethod("setPublisherInfo", &NodeLedgerWrap::SetPublisherInfo),
  });

  constructor = Napi::Persistent(tpl);
  constructor.SuppressDestruct();

  exports.Set("NodeLedger", tpl);
  return exports;
}

NodeLedgerWrap::NodeLedgerWrap(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<NodeLedgerWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::Object obj = info[0].As<Napi::Object>();
  this->ref = Napi::ObjectReference::New(obj);

  clientProxy_ =
    new NodeLedgerClientProxy(this);
  this->ledger = Ledger::CreateInstance(clientProxy_);
}

void NodeLedgerWrap::CreateWallet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  this->ledger->CreateWallet();
}

void NodeLedgerWrap::Reconcile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  this->ledger->Reconcile();
}

void NodeLedgerWrap::OnLoad(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsObject()) {
    Napi::TypeError::New
      (env, "JS Object argument expected").ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("tld").IsString()) {
    Napi::TypeError::New
      (env, "TLD in object expected to be string type")
      .ThrowAsJavaScriptException();
  }

  if (!info[0].As<Napi::Object>().Get("domain").IsString()) {
    Napi::TypeError::New
      (env, "Domain in object expected to be string type")
      .ThrowAsJavaScriptException();
  }

  if (!info[0].As<Napi::Object>().Get("path").IsString()) {
    Napi::TypeError::New
      (env, "Path in object expected to be string type")
      .ThrowAsJavaScriptException();
  }

  if (!info[0].As<Napi::Object>().Get("tabId").IsNumber()) {
    Napi::TypeError::New
      (env, "Tab ID in object expected to be numerical type")
      .ThrowAsJavaScriptException();
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  std::string tld = info[0].As<Napi::Object>().Get("tld").ToString();
  std::string domain = info[0].As<Napi::Object>().Get("domain").ToString();
  std::string path = info[0].As<Napi::Object>().Get("path").ToString();
  uint32_t tab_id =
    info[0].As<Napi::Object>().Get("tabId").ToNumber().Int32Value();
  const uint64_t currentTime = info[1].ToNumber().Int64Value();
  VisitData* visitData = new VisitData(tld, domain, path, tab_id);
  this->ledger->OnLoad(*visitData, currentTime);
}

void NodeLedgerWrap::OnUnload(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnUnload(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnShow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnShow(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnHide(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnHide(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnForeground(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnForeground(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnBackground(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnBackground(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnMediaStart(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnMediaStart(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnMediaStop(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New
    (env, "current time is expected to be a number")
    .ThrowAsJavaScriptException();
  }
  const uint64_t currentTime = info[1].ToNumber().Int64Value();

  this->ledger->OnMediaStop(info[0].ToNumber().Int32Value(), currentTime);
}

void NodeLedgerWrap::OnXHRLoad(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 1) {
    Napi::Error::New(env, "Expected positive non-zero numerical argument")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "String argument expected")
      .ThrowAsJavaScriptException();
  }
  // TODO(jsadler) check map object
  const std::map<std::string, std::string>& parts = {{"", ""}};
  if (!info[3].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
    .ThrowAsJavaScriptException();
  }
  this->ledger->OnXHRLoad(info[0].ToNumber().Int32Value(),
    info[1].ToString(), parts, info[3].ToNumber().Int64Value());
}

Napi::Value NodeLedgerWrap::GetPublisherAllowNonVerified
  (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  return Napi::Boolean::New(env, this->ledger->GetPublisherAllowNonVerified());
}

void NodeLedgerWrap::SetPublisherAllowNonVerified
  (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsBoolean()) {
    Napi::TypeError::New(env, "Boolean argument expected")
      .ThrowAsJavaScriptException();
  }
  this->ledger->SetPublisherAllowNonVerified(info[0].As<Napi::Boolean>());
}

void NodeLedgerWrap::SetPublisherMinVisitTime(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int64Value() < 0) {
    Napi::Error::New(env, "Expected non-negative numerical argument")
      .ThrowAsJavaScriptException();
  }
  this->ledger->SetPublisherMinVisitTime(info[0].ToNumber().Int64Value());
}

Napi::Value NodeLedgerWrap::GetPublisherMinVisitTime
  (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  return Napi::Number::New(env, this->ledger->GetPublisherMinVisitTime());
}

void NodeLedgerWrap::SetPublisherMinVisits(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  if (info[0].ToNumber().Int32Value() < 0) {
    Napi::Error::New(env, "Expected non-negative numerical argument")
      .ThrowAsJavaScriptException();
  }
  this->ledger->SetPublisherMinVisits(info[0].ToNumber().Int32Value());
}

void NodeLedgerWrap::SetPublisherInfo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsObject()) {
    Napi::TypeError::New
      (env, "JS Object argument expected").ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("id").IsString()) {
    Napi::TypeError::New
      (env, "ID in object expected to be string type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("duration").IsNumber()) {
    Napi::TypeError::New
      (env, "Duration in object expected to be numerical type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("visits").IsNumber()) {
    Napi::TypeError::New
      (env, "Visits in object expected to be numerical type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("pinned").IsBoolean()) {
    Napi::TypeError::New
      (env, "Pinned in object expected to be boolean type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("percent").IsNumber()) {
    Napi::TypeError::New
      (env, "Percent in object expected to be numerical type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("weight").IsNumber()) {
    Napi::TypeError::New
      (env, "Weight in object expected to be numerical type")
      .ThrowAsJavaScriptException();
  }
  if (!info[0].As<Napi::Object>().Get("excluded").IsBoolean()) {
    Napi::TypeError::New
      (env, "Pinned in object expected to be boolean type")
      .ThrowAsJavaScriptException();
  }
  if (!info[1].IsFunction()) {
    Napi::TypeError::New
      (env, "Callback argument expected to be function")
      .ThrowAsJavaScriptException();
  }

  std::unique_ptr<PublisherInfo> publisherInfo;
  publisherInfo.reset(new PublisherInfo(info[0].As<Napi::Object>().Get("id").As
    <Napi::String>()));
  publisherInfo->duration = info[0].As<Napi::Object>().Get
    ("duration").As<Napi::Number>().Int64Value();
  publisherInfo->visits = info[0].As<Napi::Object>().Get
    ("visits").As<Napi::Number>().Int32Value();
  publisherInfo->pinned = info[0].As<Napi::Object>().Get
    ("pinned").As<Napi::Boolean>();
  publisherInfo->percent = info[0].As<Napi::Object>().Get
    ("percent").As<Napi::Number>().Int32Value();
  publisherInfo->weight = info[0].As<Napi::Object>().Get
    ("weight").As<Napi::Number>().DoubleValue();
  publisherInfo->excluded = info[0].As<Napi::Object>().Get
    ("excluded").As<Napi::Boolean>();

  this->ledger->SetPublisherInfo(std::move(publisherInfo),
    std::bind(&NodeLedgerClientProxy::OnSavePublisherInfo,
    clientProxy_, info[1].As<Napi::Function>() , _1, _2));
}

Napi::Value NodeLedgerWrap::GetPublisherMinVisits
  (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  return Napi::Number::New(env, this->ledger->GetPublisherMinVisits());
}

void NodeLedgerWrap::SetContributionAmount(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Numerical argument expected")
      .ThrowAsJavaScriptException();
  }
  this->ledger->SetContributionAmount(info[0].ToNumber().DoubleValue());
}

Napi::Value NodeLedgerWrap::GetContributionAmount
  (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  return Napi::Number::New(env, this->ledger->GetContributionAmount());
}

Napi::Value NodeLedgerWrap::GetBATAddress(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string batAddr = this->ledger->GetBATAddress();
  Napi::String returnValue = Napi::String::New(env, batAddr);
  return returnValue;
}

Napi::Value NodeLedgerWrap::GetBTCAddress(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string batAddr = this->ledger->GetBTCAddress();
  Napi::String returnValue = Napi::String::New(env, batAddr);
  return returnValue;
}

Napi::Value NodeLedgerWrap::GetETHAddress(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string batAddr = this->ledger->GetETHAddress();
  Napi::String returnValue = Napi::String::New(env, batAddr);
  return returnValue;
}

Napi::Value NodeLedgerWrap::GetLTCAddress(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string batAddr = this->ledger->GetLTCAddress();
  Napi::String returnValue = Napi::String::New(env, batAddr);
  return returnValue;
}

void NodeLedgerWrap::GetWalletProperties(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  this->ledger->GetWalletProperties();
}

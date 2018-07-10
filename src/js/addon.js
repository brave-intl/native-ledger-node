/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

const uuid = require('uuid')
const NodeLedger = require('bindings')('addon').NodeLedger

const ledgerResult = require('./ledgerResult')

// const publisherInfo = {
//   id: 'url.com',
//   duration: 250000,
//   visits: 3,
//   pinned: false,
//   percent: 35,
//   weight: 20.4,
//   excluded: false
// }

const ledgerClient = {
  generateGUID: () => {
    console.log('return from native to generateGUID')
    return uuid.v4().toLowerCase()
  },
  onWalletCreated: () => {
    console.log('returned from native to onWalletCreated')
  },
  getWalletProperties: () => {
    console.log('returned from native to getWalletProperties')
  },
  onWalletProperties: (walletInfo) => {
    console.log('return from native to onWalletProperties')
    console.log(walletInfo)
  },
  onReconcileComplete: (result, viewingId) => {
    console.log('return from native to onReconcileComplete')
    console.log(result)
    console.log(viewingId)
  },
  loadLedgerState: (cb) => {
    console.log('return from native to loadLedgerState')
    cb() // <-- back to native
  },
  saveLedgerState: (ledgerState, cb) => {
    console.log('return from native to saveLedgerState')
    console.log(ledgerState)
    cb(ledgerResult.OK) // <-- back to native
  },
  loadPublisherState: (err, cb) => {
    if (err) { console.error(err) }
    console.log('return from native to loadPublisherState')
    const testData = '{min_publisher_duration: 8000, min_visits: 1, allow_non_verified: true}'
    cb(ledgerResult.OK, testData) // <-- back to native
  },
  savePublisherState: (publisherState, cb) => {
    console.log('return from native to savePublisherState')
    console.log(publisherState)
    cb(ledgerResult.OK) // <-- back to native
  },
  savePublisherInfo: (publisherInfo, cb) => {
    console.log('return from native to savePublisherInfo')
    console.log(publisherInfo)
    cb(ledgerResult.OK) // <-- back to native
  },
  loadPublisherInfo: (publisherId, cb) => {
    console.log('return from native to loadPublisherInfo')
    console.log(publisherId)
    cb() // <-- back to native
  },
  loadPublisherInfoList: (start, limit, filter, cb) => {
    console.log('return from native to loadPublisherInfoList')
    console.log(start)
    console.log(limit)
    console.log(filter)
    cb() // <-- back to native
  },
  loadURL: (url, headers, content, contentType, method, cb) => {
    console.log('return from native to loadURL')
    console.log('url:' + url)
    console.log('headers: ' + headers)
    console.log('content: ' + content)
    console.log('contentType: ' + contentType)
    console.log('method: ' + method)
    cb() // <-- back to native
  },
  onSavePublisherInfo: (result, publisherInfo) => {
    console.log('return from native to onSavePublisherInfo')
    console.log(result === ledgerResult.OK)
    console.log(result === ledgerResult.ERROR)
    console.log(publisherInfo)
  }
}

const nativeLedger = new NodeLedger(ledgerClient)

const ledger = {
  createWallet: () => {
    nativeLedger.createWallet()
  },
  reconcile: () => {
    nativeLedger.reconcile()
  },
  onLoad: (tabId) => {
    nativeLedger.onLoad(tabId)
  },
  onUnload: (tabId) => {
    nativeLedger.onUnload(tabId)
  },
  onShow: (tabId) => {
    nativeLedger.onShow(tabId)
  },
  onHide: (tabId) => {
    nativeLedger.onHide(tabId)
  },
  onForeground: (tabId) => {
    nativeLedger.onForeground(tabId)
  },
  onBackground: (tabId) => {
    nativeLedger.onBackground(tabId)
  },
  onMediaStart: (tabId) => {
    nativeLedger.onMediaStart(tabId)
  },
  onMediaStop: (tabId) => {
    nativeLedger.onMediaStop(tabId)
  },
  onXHRLoad: (tabId, url) => {
    nativeLedger.onXHRLoad(tabId, url)
  },
  setPublisherInfo: (publisherInfo, callback) => {
    nativeLedger.setPublisherInfo(publisherInfo, callback)
  },
  getPublisherInfo: (publisherId, callback) => {
    nativeLedger.getPublisherInfo(publisherId, callback)
  },
  getPublisherInfoList: (start, limit, filter, callback) => {
    nativeLedger.getPublisherInfoList(start, limit, filter, callback)
  },
  setPublisherMinVisitTime: (duration) => {
    nativeLedger.setPublisherMinVisitTime(duration)
  },
  setPublisherMinVisits: (visits) => {
    nativeLedger.setPublisherMinVisits(visits)
  },
  setPublisherAllowNonVerified: (allow) => {
    nativeLedger.setPublisherAllowNonVerified(allow)
  },
  setContributionAmount: (amount) => {
    nativeLedger.setContributionAmount(amount)
  },
  getBATAddress: () => {
    return nativeLedger.getBATAddress()
  },
  getBTCAddress: () => {
    return nativeLedger.getBTCAddress()
  },
  getETHAddress: () => {
    return nativeLedger.getETHAddress()
  },
  getLTCAddress: () => {
    return nativeLedger.getLTCAddress()
  },
  getPublisherMinVisitTime: () => {
    return nativeLedger.getPublisherMinVisitTime()
  },
  getPublisherMinVisits: () => {
    return nativeLedger.getPublisherMinVisits()
  },
  getPublisherAllowNonVerified: () => {
    return nativeLedger.getPublisherAllowNonVerified()
  },
  getContributionAmount: () => {
    return nativeLedger.getContributionAmount()
  },
  getWalletProperties: () => {
    nativeLedger.getWalletProperties()
  },
  getPromotion: () => {
    nativeLedger.getPromotion()
  },
  getPromotionCaptcha: () => {
    nativeLedger.getPromotionCaptcha()
  },
  getWalletPassphrase: () => {
    return nativeLedger.getWalletPassphrase()
  },
  recoverWallet: () => {
    nativeLedger.recoverWallet()
  }
}

module.exports = {
  NodeLedger,
  ledger,
  ledgerClient
}

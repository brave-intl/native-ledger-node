/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* global describe, it, before, afterEach */

const sinon = require('sinon')
const assert = require('assert')

const ledgerAddon = require('../src/js/addon')
const NodeLedger = ledgerAddon.NodeLedger
const ledgerClient = ledgerAddon.ledgerClient

describe('node-native-ledger create wallet tests', function () {
  let loadLedgerStateSpy
  before(function () {
    loadLedgerStateSpy = sinon.spy(ledgerClient, 'loadLedgerState')
  })

  afterEach(function () {
    loadLedgerStateSpy.restore()
  })

  it('calls create wallet and calls back from native to loadLedgerState', function () {
    const ledger = new NodeLedger(ledgerClient)
    ledger.createWallet()
    assert(loadLedgerStateSpy.calledOnce)
  })

  it('calls create wallet and ignores arguments', function () {
    const ledger = new NodeLedger(ledgerClient)
    ledger.createWallet('test arg')
    assert(loadLedgerStateSpy.calledOnce)
  })
})

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* global describe, it */

const assert = require('assert')

const ledgerAddon = require('../src/js/addon')
const NodeLedger = ledgerAddon.NodeLedger
const ledgerClient = ledgerAddon.ledgerClient

describe('node-native-ledger get currency addresses tests', function () {
  it('calls getBATAddress and returns string type', function () {
    const ledger = new NodeLedger(ledgerClient)
    assert(typeof ledger.getBATAddress() === 'string', 'Returned address is string type')
  })

  it('calls getBTCAddress and returns string type', function () {
    const ledger = new NodeLedger(ledgerClient)
    assert(typeof ledger.getBTCAddress() === 'string', 'Returned address is string type')
  })

  it('calls getETHAddress and returns string type', function () {
    const ledger = new NodeLedger(ledgerClient)
    assert(typeof ledger.getETHAddress() === 'string', 'Returned address is string type')
  })

  it('calls getLTCAddress and returns string type', function () {
    const ledger = new NodeLedger(ledgerClient)
    assert(typeof ledger.getLTCAddress() === 'string', 'Returned address is string type')
  })
})

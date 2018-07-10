/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* global describe, it */

const assert = require('assert')

const ledgerAddon = require('../src/js/addon')
const NodeLedger = ledgerAddon.NodeLedger
const ledgerClient = ledgerAddon.ledgerClient

let visitData = {
  tld: 'org',
  domain: 'example',
  path: '/dir/file.html',
  tabId: 1
}

describe('node-native-ledger event tests', function () {
  describe('calling onLoad', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onLoad()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Object was expected')
    })

    it('with invalid argument should throw JavaScript exception', function () {
      const ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onLoad('invalid')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Object was expected')
    })

    it('with empty object should throw JavaScript exception', function () {
      const ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onLoad({})
      } catch (e) {
        argException = true
      }
      assert(argException, 'Inner object type mismatch')
    })

    it('with visitData object with wrongly typed members should throw JavaScript exception', function () {
      const ledger = new NodeLedger(ledgerClient)
      const data = Object.assign({}, visitData)
      data.tld = 2
      let argException = false
      try {
        ledger.onLoad(data)
      } catch (e) {
        argException = true
      }
      assert(argException, 'Inner object type mismatch')
    })

    it('with valid visitData, should not throw', function () {
      const ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onLoad(visitData)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'No exception thrown')
    })
  })

  describe('calling onUnload', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onUnload()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onUnload('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onUnload(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onShow', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onShow()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onShow('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onShow(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onHide', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onHide()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onHide('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onHide(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onForeground', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onForeground()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onForeground('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onForeground(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onBackground', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onBackground()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onBackground('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onBackground(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onMediaStart', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStart()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStart('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStart(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onMediaStop', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStop()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStop('test')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with correct argument should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onMediaStop(200)
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })

  describe('calling onXHRLoad', function () {
    it('with no argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onXHRLoad()
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-numerical argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onXHRLoad('test', 'url')
      } catch (e) {
        argException = true
      }
      assert(argException, 'Number was expected')
    })
    it('with non-string argument should throw JavaScript exception', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onXHRLoad(200, 200)
      } catch (e) {
        argException = true
      }
      assert(argException, 'String was expected')
    })
    it('with correct arguments should not throw', function () {
      let ledger = new NodeLedger(ledgerClient)
      let argException = false
      try {
        ledger.onXHRLoad(200, 'url')
      } catch (e) {
        argException = true
      }
      assert(!argException, 'Should not throw')
    })
  })
})

/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* global describe, it */
const assert = require('assert')

const ledgerAddon = require('../src/js/addon')
const NodeLedger = ledgerAddon.NodeLedger
const ledgerClient = ledgerAddon.ledgerClient

describe('node-native-ledger publisher tests', function () {
  describe('getter and setter methods', function () {
    describe('publisher minimum visit time setter and getter', function () {
      it('sets publisher minimum visit time with null argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherMinVisitTime()
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })
      it('sets publisher minimum visit time with wrong type argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherMinVisitTime('20')
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })
      it('sets publisher minimum visit time with correct arguments and is able to retrieve from native', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        const minVisitTime = 20
        try {
          ledger.setPublisherMinVisitTime(minVisitTime)
        } catch (e) {
          argException = true
        }
        assert(!argException && ledger.getPublisherMinVisitTime() === minVisitTime, 'Should not throw')
      })
      it('gets the default minimum visit time if it hasnt been set', function () {
        const ledger = new NodeLedger(ledgerClient)
        assert(ledger.getPublisherMinVisitTime() === 8000, 'Default min visit time should be 8000ms')
      })
    })

    describe('publisher minimum visits setter and getter', function () {
      it('sets publisher minimum visit with null argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherMinVisits()
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })
      it('sets publisher minimum visit with wrong type argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherMinVisits('20')
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })
      it('sets publisher minimum visit with correct arguments and is able to retrieve from native', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        const minVisit = 4
        try {
          ledger.setPublisherMinVisits(minVisit)
        } catch (e) {
          argException = true
        }
        assert(!argException && ledger.getPublisherMinVisits() === minVisit, 'Should not throw')
      })
      it('gets the default minimum visit if it hasnt been set', function () {
        const ledger = new NodeLedger(ledgerClient)
        assert(ledger.getPublisherMinVisits() === 1, 'Default min visit should be 1')
      })
    })

    describe('publisher allow non-verified for contribution setter and getter', function () {
      it('sets publisher allow non-verified with null argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherAllowNonVerified()
        } catch (e) {
          argException = true
        }
        assert(argException, 'Boolean type was expected')
      })
      it('sets publisher allow non-verified with wrong type argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setPublisherAllowNonVerified('true')
        } catch (e) {
          argException = true
        }
        assert(argException, 'Boolean type was expected')
      })
      it('sets publisher allow non-verified with correct arguments and is able to retrieve from native', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        const allowNonVerified = false
        try {
          ledger.getPublisherAllowNonVerified(allowNonVerified)
        } catch (e) {
          argException = true
        }
        assert(!argException && !ledger.getPublisherAllowNonVerified() === allowNonVerified, 'Should not throw')
      })
      it('gets the default allow non-verified if it hasnt been set', function () {
        const ledger = new NodeLedger(ledgerClient)
        assert(ledger.getPublisherAllowNonVerified(), 'Default allow non-verified should be true')
      })
    })

    describe('publisher set amount for contribution setter and getter', function () {
      it('sets publisher contribution with null argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setContributionAmount()
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })
      it('sets publisher contribution with wrong type argument', function () {
        const ledger = new NodeLedger(ledgerClient)
        let argException = false
        try {
          ledger.setContributionAmount('true')
        } catch (e) {
          argException = true
        }
        assert(argException, 'Numeric type was expected')
      })

      // TODO: configure openssl symbols -> the following calls into SaveState
      // it('sets publisher contribution with correct arguments and is able to retrieve from native', function () {
      //   const ledger = new NodeLedger(ledgerClient)
      //   let argException = false
      //   const contributionAmount = 47
      //   try {
      //     ledger.setContributionAmount(contributionAmount)
      //   } catch {
      //     argException = true
      //   }
      //   assert(!argException && ledger.getContributionAmount() === contributionAmount, 'Should not throw')
      // })
      it('gets the default contribution if it hasnt been set', function () {
        const ledger = new NodeLedger(ledgerClient)
        assert(ledger.getContributionAmount() === 0, 'Default contribution should be 0')
      })
    })
  })
})

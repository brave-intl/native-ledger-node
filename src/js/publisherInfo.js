/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

const publisherInfoFilter = require('./publisherInfoFilter')

class PublisherInfo {
  constructor (publisherId) {
    this.id = publisherId
    this.duration = 0
    this.visits = 0
    this.pinned = false
    this.percent = 0
    this.weight = 0
    this.excluded = true
    this.publisherFilter = publisherInfoFilter.ALL
  }
}

module.exports = PublisherInfo

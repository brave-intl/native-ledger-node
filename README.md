# node-native-ledger
Interface between BAT-native-ledger and js

The current implementation is based on the current state of `bat-native-ledger` commit [c9195bd](https://github.com/brave-intl/bat-native-ledger/commit/c9195bde4020d6aa40256fc40c3e623fd2fdcd3c)

The static library `libbat-native-ledger-standalone.a` is based on the same commit [c9195bd](https://github.com/brave-intl/bat-native-ledger/commit/c9195bde4020d6aa40256fc40c3e623fd2fdcd3c)

## Instructions to build
```
npm i
npm run build OR npm run build_dev
```

## instructions to run tests
```
npm run test-js
```

## Usage
In javascript, within the library, include the path to `addon.js` and create a native ledger instance and a ledger client JavaScript instance:
```
const ledgerAddon = require('../src/js/addon')
const NodeLedger = ledgerAddon.NodeLedger
const ledgerClient = ledgerAddon.ledgerClient
```

To get an instance of the native ledger:
```
const ledger = new NodeLedger(ledgerClient)
```

This will create an instance of the native ledger from C++ and return it as JavaScript to `const ledger`. Native functions can then be called on the native ledger

### Examples

Create ledger wallet:
```
ledger.createWallet()
```

Set minimum visit time:
```
const minVisitTime = 20000
ledger.setPublisherMinVisitTime(minVisitTime)
```

Get minimum visit time:
```
console.log(ledger.getPublisherMinVisitTime())

output:
20000
```

## Exceptions

Arguments passed into native functions from javascript are type checked on the native side. Invalid arguments will be thrown back as JavaScript exceptions

### Example
```
const minVisitTime = 'abc'
ledger.setPublisherMinVisitTime(minVisitTime)

exception output
TypeError: Numerical argument expected
```

## Changes

`node-native-ledger` is in development to stay in sync with `bat-native-ledger`. The current state of this module is prototype and should not be considered fully functional.





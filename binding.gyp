{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/c/node-addon-api/node_ledger_url_loader.cc",
        "src/c/node-addon-api/node_ledger_client_proxy.cc",
        "src/c/node-addon-api/node_ledger_wrap.cc",
        "src/c/node-addon-api/addon.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        ".",
        "bat-native-ledger/include",
        "src/c/node-addon-api"
      ],
      "libraries": [
        "<(module_root_dir)/bin/libbat-native-ledger-standalone.a"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "conditions": [
        ['OS=="win"', {
          }, {
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc': [ '-fexceptions' ]
          }
        ]
      ],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-ObjC" ],
        "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-v"],
        "MACOSX_DEPLOYMENT_TARGET": "10.9",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      },
    }
  ]
}

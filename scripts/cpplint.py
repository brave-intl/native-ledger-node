#!/usr/bin/env python

import fnmatch
import os
import sys

from lib.util import execute

IGNORE_FILES = [
  os.path.join('./bat-native-ledger/include/bat/ledger/export.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/ledger_callback_handler.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/ledger_client.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/ledger_task_runner.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/ledger_url_loader.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/ledger.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/promo.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/wallet_info.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/wallet_info.cc'),
  os.path.join('./bat-native-ledger/include/bat/ledger/publisher_info.h'),
  os.path.join('./bat-native-ledger/include/bat/ledger/publisher_info.cc'),
  os.path.join('./bat-native-ledger/src/bat/ledger/ledger.cc'),
  os.path.join('./bat-native-ledger/src/test/mock_ledger_client.cc'),
  os.path.join('./bat-native-ledger/src/test/mock_ledger_client.h'),
  os.path.join('./bat-native-ledger/src/bat_client.cc'),
  os.path.join('./bat-native-ledger/src/bat_client.h'),
  os.path.join('./bat-native-ledger/src/bat_get_media.cc'),
  os.path.join('./bat-native-ledger/src/bat_get_media.h'),
  os.path.join('./bat-native-ledger/src/bat_helper_platform.cc'),
  os.path.join('./bat-native-ledger/src/bat_helper_platform.h'),
  os.path.join('./bat-native-ledger/src/bat_helper.cc'),
  os.path.join('./bat-native-ledger/src/bat_helper.h'),
  os.path.join('./bat-native-ledger/src/bat_publishers.cc'),
  os.path.join('./bat-native-ledger/src/bat_publishers.h'),
  os.path.join('./bat-native-ledger/src/ledger_impl.cc'),
  os.path.join('./bat-native-ledger/src/ledger_impl.h'),
  os.path.join('./bat-native-ledger/src/ledger_task_runner_impl.cc'),
  os.path.join('./bat-native-ledger/src/ledger_task_runner_impl.h'),
  os.path.join('./bat-native-ledger/src/rapidjson_bat_helper.h'),
  os.path.join('./bat-native-ledger/src/static_values.h'),
  os.path.join('./bat-native-ledger/src/url_request_handler.cc'),
  os.path.join('./bat-native-ledger/src/url_request_handler.h')
]

SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def main():
  os.chdir(SOURCE_ROOT)
  files = list_files([''],
                     ['*.cpp', '*.cc', '*.h'])

  node_modules_files = list_files(['node_modules'],
                     ['*.cpp', '*.cc', '*.h'])

  call_cpplint(list(set(files) - set(IGNORE_FILES) - set(node_modules_files)))


def list_files(directories, filters):
  matches = []
  for directory in directories:
    for root, _, filenames, in os.walk(os.path.join('./', directory)):
      for f in filters:
        for filename in fnmatch.filter(filenames, f):
          matches.append(os.path.join(root, filename))
  return matches


def call_cpplint(files):
  cpplint = os.path.join(SOURCE_ROOT, 'vendor', 'depot_tools', 'cpplint.py')
  execute([sys.executable, cpplint] + files)


if __name__ == '__main__':
  sys.exit(main())

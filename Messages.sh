#! /usr/bin/env bash
$XGETTEXT `find libkgapi2 -name \*.cpp -o -name \*.h` -o $podir/libkgapi.pot
rm -f rc.cpp

#! /usr/bin/env bash
$XGETTEXT `find libkgapi -name \*.cpp -o -name \*.h` -o $podir/libkgapi.pot
rm -f rc.cpp

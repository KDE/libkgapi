#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui -o -name \*.kcfg` >> rc.cpp || exit 11
$XGETTEXT `find . -name \*.cpp | grep -v '/tests/'` -o $podir/libkgoogle.pot
rm -f rc.cpp

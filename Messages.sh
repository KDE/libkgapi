#! /usr/bin/env bash
$EXTRACT_TR_STRINGS `find src -name \*.cpp -o -name \*.h` -o $podir/libkgapi6_qt.pot

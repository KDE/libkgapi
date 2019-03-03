# LibKGAPI

LibKGAPI (previously called LibKGoogle) is a C++ library that implements APIs for
various Google services.

Currently supported APIs:
 * Calendar API v3 (https://developers.google.com/google-apps/calendar)
 * Contacts API v3 (https://developers.google.com/google-apps/contacts/v3/)
 * Tasks API v1 (https://developers.google.com/google-apps/tasks)
 * Static Google Maps API v2 (https://developers.google.com/maps/documentation/staticmaps/)
 * Drive API v2 (https://developers.google.com/drive/v2/reference)
 * Blogger API v3 (https://developers.google.com/blogger/docs/3.0/reference/)

Deprecated APIs:
 * Latitude API v1 (https://developers.google.com/latitude/v1/)

## Compiling:

```
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=/usr
make
# As root:
make install
```

To compile example applications, run cmake with `-DKGAPI_BUILD_EXAMPLES=TRUE` argument.
To run automated tests, run `make test`.

## BUGS:

Submit bugs and feature requests to KDE bugzilla, product libkgapi:

https://bugs.kde.org/enter_bug.cgi?product=libkgapi

## API Documentation:

https://api.kde.org/kdepim/libkgapi/html/index.html

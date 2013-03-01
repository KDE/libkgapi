prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libkgapi2
Description: A C++ library for accessing Google services.
URL: http://projects.kde.org/libkgapi
Requires:
Version: ${KGAPI_VERSION}
Libs: -L${LIB_INSTALL_DIR} -lkgapi2
Cflags: -I${INCLUDE_INSTALL_DIR}

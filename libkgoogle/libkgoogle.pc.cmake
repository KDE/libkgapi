prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libkgoogle
Description: A C++ library for accessing Google services.
URL: http://projects.kde.org/akonadi-google
Requires:
Version: ${KGOOGLE_VERSION}
Libs: -L${LIB_INSTALL_DIR} -lkgoogle
Cflags: -I${INCLUDE_INSTALL_DIR}

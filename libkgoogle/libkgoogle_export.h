#ifndef LIBKGOOGLE_EXPORT_H
#define LIBKGOOGLE_EXPORT_H

#include <QtGlobal>

#if defined(LIBKGOOGLE_LIBRARY)
#  define LIBKGOOGLE_EXPORT Q_DECL_EXPORT
#else
#  define LIBKGOOGLE_EXPORT Q_DECL_IMPORT
#endif

#endif

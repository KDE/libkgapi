/*
 *   SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *             (c) 2016 - 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <config.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef macintosh
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include <assert.h>

#include <sasl/sasl.h>
#include <sasl/saslplug.h>
#include <sasl/saslutil.h>

#include "plugin_common.h"

#ifdef WIN32
BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif

SASL_CLIENT_PLUG_INIT(xoauth2)

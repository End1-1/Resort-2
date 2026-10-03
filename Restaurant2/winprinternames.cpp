#include "winprinternames.h"

#ifdef Q_OS_WIN

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <winspool.h>

#include <QByteArray>

QStringList winInstalledPrinterNames()
{
    QStringList names;
    DWORD needed = 0;
    DWORD count = 0;
    EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 1, nullptr, 0, &needed, &count);
    if(needed == 0) {
        return names;
    }
    QByteArray buffer(int(needed), 0);
    if(!EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 1,
                      reinterpret_cast<LPBYTE>(buffer.data()), needed, &needed, &count)) {
        return names;
    }
    const PRINTER_INFO_1W *info = reinterpret_cast<const PRINTER_INFO_1W *>(buffer.constData());
    for(DWORD i = 0; i < count; ++i) {
        if(info[i].pName) {
            names.append(QString::fromWCharArray(info[i].pName));
        }
    }
    return names;
}

#else

QStringList winInstalledPrinterNames()
{
    return QStringList();
}

#endif

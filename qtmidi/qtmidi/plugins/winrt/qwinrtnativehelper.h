#ifndef WINRTNATIVEHELPER_H
#define WINRTNATIVEHELPER_H

#include <windows.h>
#include <QtCore/qstring.h>

HRESULT __stdcall GetActivationFactoryByPCWSTR(void*, ::Platform::Guid&, void**);
namespace __winRT
{
HRESULT __stdcall __getActivationFactoryByPCWSTR(const void *str,
                                                 ::Platform::Guid &pGuid,
                                                 void **ppActivationFactory);
}

QT_BEGIN_NAMESPACE

QString convert(Platform::String ^ str);
Platform::String^ convert(const QString &str);

QT_END_NAMESPACE

#endif // WINRTNATIVEHELPER_H

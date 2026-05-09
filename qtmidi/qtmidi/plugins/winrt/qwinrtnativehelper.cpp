#include "qwinrtnativehelper.h"

HRESULT __stdcall GetActivationFactoryByPCWSTR(void*,::Platform::Guid&, void**);
namespace __winRT
{
HRESULT __stdcall __getActivationFactoryByPCWSTR(const void *str,
                                                 ::Platform::Guid &pGuid,
                                                 void **ppActivationFactory)
{
    return GetActivationFactoryByPCWSTR(const_cast<void *>(str), pGuid, ppActivationFactory);
}
}

QT_BEGIN_NAMESPACE

QString convert(Platform::String ^ str) {
    return QString::fromWCharArray(str->Begin());
}

Platform::String
    ^ convert(const QString &str) { return ref new Platform::String(str.toStdWString().c_str()); }

    QT_END_NAMESPACE

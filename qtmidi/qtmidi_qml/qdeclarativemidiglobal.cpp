#include "qdeclarativemidiglobal_p.h"

#include <qjsengine.h>

QT_BEGIN_NAMESPACE

QDeclarativeMidiGlobal::QDeclarativeMidiGlobal(QJSEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
{
}

QT_END_NAMESPACE

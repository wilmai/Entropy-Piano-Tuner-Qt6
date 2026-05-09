#ifndef QDECLARATIVEMIDIGLOBAL_P_H
#define QDECLARATIVEMIDIGLOBAL_P_H

#include <QtQml/qqml.h>
#include <QtQml/qjsvalue.h>

QT_BEGIN_NAMESPACE

class QDeclarativeMidiGlobal : public QObject
{
    Q_OBJECT

public:
    explicit QDeclarativeMidiGlobal(QJSEngine *engine, QObject *parent = 0);

private:
    QJSEngine *m_engine;
};

QT_END_NAMESPACE

#endif  // QDECLARATIVEMIDIGLOBAL_P_H

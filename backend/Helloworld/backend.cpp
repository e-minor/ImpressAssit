#include <QtQml>
#include <QtQml/QQmlContext>
#include "backend.h"
#include "mytype.h"


void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Helloworld"));

    qmlRegisterType<Impress>(uri, 1, 0, "Impress");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}


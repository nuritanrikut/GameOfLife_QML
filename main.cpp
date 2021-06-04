#include "TableModel.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main( int argc, char *argv[] )
{
    QGuiApplication app( argc, argv );
    QQmlApplicationEngine engine;

    const QUrl url( QStringLiteral( "qrc:/main.qml" ) );

    // Register TableModel class into QML system as MyTableModel so that it can be instantiated from QML
    // Named it MyTableModel because there is a built-in type named TableModel in QML
    qmlRegisterType<TableModel>( "MyTableModel", 0, 1, "MyTableModel" );

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url]( QObject *obj, const QUrl &objUrl ) {
            if( !obj && url == objUrl )
                QCoreApplication::exit( -1 );
        },
        Qt::QueuedConnection );

    engine.load( url );

    return app.exec();
}

#include "embedded/playlistmanager/inc/SortFilterProxyModel.h"
#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtGui/qguiapplication.h>
#include <QtQml/qqml.h>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
	QFileInfo appFile(argv[0]);
	QDir d = appFile.absolutePath();
	QString pluginDir;
	d.cd("plugins");
	pluginDir = d.absolutePath();
	pluginDir = QDir::toNativeSeparators(pluginDir);
	QStringList dirList;
	dirList.append(pluginDir);
	QCoreApplication::setLibraryPaths(dirList);
#endif

	QGuiApplication app(argc, argv);

    if (QCoreApplication::arguments().contains(QLatin1String("--coreprofile"))) {
        QSurfaceFormat fmt;
        fmt.setVersion(4, 4);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(fmt);
    }
    qmlRegisterType<SortFilterProxyModel>("org.qtproject.example", 1, 0, "SortFilterProxyModel");
    QQmlApplicationEngine engine(QUrl("qrc:/Resources/main.qml"));
    return app.exec();
}

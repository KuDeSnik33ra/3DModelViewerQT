#include <QApplication>
#include <QSurfaceFormat>
#include <QLoggingCategory>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
#ifdef _DEBUG
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("3DModelViewer");
    app.setApplicationVersion("1.0");

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    MainWindow window;
    window.show();

    return app.exec();
}

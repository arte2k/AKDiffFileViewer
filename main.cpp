#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "diffviewerwindow.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DiffViewerWindow window;
    window.show();

    return app.exec();
}

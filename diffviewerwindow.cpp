#include "diffviewerwindow.h"
#include <QTextEdit>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include  <QTextStream>
#include "difffilesyntaxhighlighter.h"


DiffViewerWindow::DiffViewerWindow()
{
    openAction = new QAction(tr("&Open"), this);
    exitAction = new QAction(tr("&Exit"), this);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    textView = new QTextEdit;
    textView->setReadOnly(true);
    setCentralWidget(textView);

    diffFileHighlighter = new DiffFileSyntaxHighlighter(textView);

    setWindowTitle(tr("AKDiffViewer"));
}

void DiffViewerWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Diff Files (*.diff)"));
    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        textView->setText(in.readAll());
        file.close();
    }
}

void DiffViewerWindow::quit()
{

}
#include "diffviewerwindow.h"
#include <QPlainTextEdit>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include  <QTextStream>
#include <QKeySequence>
#include <QApplication>
#include "difffilesyntaxhighlighter.h"
#include <iostream>
#include <QPlainTextDocumentLayout>

DiffViewerWindow::DiffViewerWindow()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    onlyAddedCodeAction = new QAction(tr("&Show only added code"), this);
    onlyRemovedCodeAction = new QAction(tr("&Show only removed code"), this);

    connect(onlyAddedCodeAction, SIGNAL(triggered()), this, SLOT(removeRemovedCodeFromView()));
    connect(onlyRemovedCodeAction, SIGNAL(triggered()), this, SLOT(removeAddedCodeFromView()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(onlyAddedCodeAction);
    fileMenu->addAction(onlyRemovedCodeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    textView = new QPlainTextEdit;
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
//        QRegExp regExp("^\\+.*");
//        for(QString line = in.readLine(); !line.isNull(); line = in.readLine())
//        {
//            if(!regExp.exactMatch(line))
//            {
//            }
//        }
        textView->setPlainText(in.readAll());
//        QRegExp regExp("^\\+.*");
//        QTextCursor cursor = textView->document()->find(regExp);
//        while(!cursor.isNull())
//        {
//            cursor.deleteChar();
//            cursor = textView->document()->find(regExp);
//        }
        //cursor.block().setVisible(false);
        file.close();
    }
}

void DiffViewerWindow::quit()
{
    qApp->quit();
}

void DiffViewerWindow::removeAddedCodeFromView()
{
    QRegExp regExp("^\\+[^\n]*");
    removeTextMatchingRegexp(regExp);
}

void DiffViewerWindow::removeRemovedCodeFromView()
{
    QRegExp regExp("^\\-[^\n]*");
    removeTextMatchingRegexp(regExp);
}

void DiffViewerWindow::removeTextMatchingRegexp(const QRegExp& regExp)
{
    QTextDocument* doc= textView->document();
    QPlainTextDocumentLayout* layout  = qobject_cast<QPlainTextDocumentLayout*>(textView->document()->documentLayout());
    QTextBlock block = doc->begin();
    while(block.isValid())
    {
        QTextCursor cursor(block);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        block = block.next();
        if(regExp.exactMatch(cursor.selectedText()))
        {
            cursor.block().setVisible(false);
            cursor.block().setLineCount(0);
        }
    }
    layout->requestUpdate();
}

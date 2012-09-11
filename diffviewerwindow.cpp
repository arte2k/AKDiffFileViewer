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
#include "diffblockuserdata.h"

DiffViewerWindow::DiffViewerWindow():isNewCodeVisible(true), isRemovedCodeVisible(true)
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
        textView->setPlainText(in.readAll());
        file.close();
        setBlockUserData();
    }
}

void DiffViewerWindow::setBlockUserData()
{
    QTextDocument* doc= textView->document();
    QTextBlock block = doc->begin();
    QRegExp regExpNewCode("^\\+[^\n]*");
    QRegExp regExpRemovedCode("^\\-[^\n]*");
    while(block.isValid())
    {
        QTextCursor cursor(block);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        block = block.next();
        if(regExpNewCode.exactMatch(cursor.selectedText()))
        {
            cursor.block().setUserData(new DiffBlockUserData(true,cursor.block().lineCount()));
        } else if(regExpRemovedCode.exactMatch(cursor.selectedText()))
        {
            cursor.block().setUserData(new DiffBlockUserData(false,cursor.block().lineCount()));
        }
    }
}

void DiffViewerWindow::quit()
{
    qApp->quit();
}

void DiffViewerWindow::removeAddedCodeFromView()
{
    if(isNewCodeVisible)
        removeText(true, true);
    else
        removeText(true, false);
    isNewCodeVisible = !isNewCodeVisible;
}

void DiffViewerWindow::removeRemovedCodeFromView()
{
    if(isRemovedCodeVisible)
        removeText(false, true);
    else
        removeText(false, false);
    isRemovedCodeVisible = !isRemovedCodeVisible;
}

void DiffViewerWindow::removeText(bool removeNewCode, bool isHiding)
{
    QTextDocument* doc= textView->document();
    QPlainTextDocumentLayout* layout  = qobject_cast<QPlainTextDocumentLayout*>(textView->document()->documentLayout());
    QTextBlock block = doc->begin();
    while(block.isValid())
    {
        DiffBlockUserData* userData = static_cast<DiffBlockUserData*>(block.userData());
        if(userData != NULL)
        {
            if((userData->isNewCode() && removeNewCode) || (!userData->isNewCode() && !removeNewCode))
            {
                if(isHiding)
                {
                    block.setVisible(false);
                    block.setLineCount(0);
                }
                else
                {
                    block.setVisible(true);
                    block.setLineCount(userData->getLineCount());

                }
            }
        }
        block = block.next();

    }
    layout->requestUpdate();
}

#ifndef DIFFVIEWERWINDOW_H
#define DIFFVIEWERWINDOW_H

#include <QMainWindow>
#include <QApplication>
class QPlainTextEdit;
class QAction;
class DiffFileSyntaxHighlighter;


class DiffViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DiffViewerWindow();
private slots:
    void open();
    void removeAddedCodeFromView();
    void removeRemovedCodeFromView();
    void quit();

signals:
    
public slots:
private:
    QPlainTextEdit* textView;
    QAction* onlyRemovedCodeAction;
    QAction* onlyAddedCodeAction;
    QAction* openAction;
    QAction* exitAction;
    QMenu*   fileMenu;

    DiffFileSyntaxHighlighter* diffFileHighlighter;
    bool isNewCodeVisible;
    bool isRemovedCodeVisible;

    void removeText(bool removeNewCode,  bool isHiding);
    void setBlockUserData();
};

#endif // DIFFVIEWERWINDOW_H

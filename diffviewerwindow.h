#ifndef DIFFVIEWERWINDOW_H
#define DIFFVIEWERWINDOW_H

#include <QMainWindow>
class QTextEdit;
class QAction;
class DiffFileSyntaxHighlighter;

class DiffViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DiffViewerWindow();
private slots:
    void open();
    void quit();

signals:
    
public slots:
private:
    QTextEdit* textView;

    QAction* openAction;
    QAction* exitAction;
    QMenu*   fileMenu;

    DiffFileSyntaxHighlighter* diffFileHighlighter;
};

#endif // DIFFVIEWERWINDOW_H

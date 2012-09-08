#ifndef DIFFFILESYNTAXHIGHLIGHTER_H
#define DIFFFILESYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <vector>
class DiffFileSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    DiffFileSyntaxHighlighter(QTextEdit *parent);
protected:
    void highlightBlock(const QString& text);
private:
    struct HighlightingRule
         {
             QRegExp pattern;
             QTextCharFormat format;
         };
    std::vector<HighlightingRule> highlightingRules;
};

#endif // DIFFFILESYNTAXHIGHLIGHTER_H

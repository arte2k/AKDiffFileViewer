#include "difffilesyntaxhighlighter.h"
#include <QSyntaxHighlighter>
#include <QRegExp>
#include <QString>

DiffFileSyntaxHighlighter::DiffFileSyntaxHighlighter(QTextEdit * parent):QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    rule.format.setBackground(QColor(148,185, 121));
    rule.pattern = QRegExp("^\\+[^\n]*");
    highlightingRules.push_back(rule);

    rule.format.setBackground(QColor(245,166, 61));
    rule.pattern = QRegExp("^\\-[^\n]*");
    highlightingRules.push_back(rule);
}


void DiffFileSyntaxHighlighter::highlightBlock(const QString& text)
{
    foreach(const HighlightingRule& rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

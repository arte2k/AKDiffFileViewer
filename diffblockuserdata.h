#ifndef DIFFBLOCKUSERDATA_H
#define DIFFBLOCKUSERDATA_H

#include <QTextBlockUserData>

class DiffBlockUserData : public QTextBlockUserData
{
public:
    DiffBlockUserData(bool isNewCode, int linesOfCode):QTextBlockUserData(),bIsNewCode(isNewCode), iLinesOfCode(linesOfCode) {}
    ~DiffBlockUserData() {}

    bool isNewCode() {return bIsNewCode;}
    int getLineCount() {return iLinesOfCode;}
private:
    bool bIsNewCode;
    int iLinesOfCode;
};

#endif // DIFFBLOCKUSERDATA_H

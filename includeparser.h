#ifndef INCLUDEPARSER_H
#define INCLUDEPARSER_H

#include "node.h"

#include <QString>
#include <QFile>

class IncludeParser
{
public:
    IncludeParser();

    Node* parse(const QString &filePath);
    void parseFileForIncludes(Node *node);
};

#endif // INCLUDEPARSER_H

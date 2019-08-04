#include "includeparser.h"
#include <QTextStream>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>

static const QRegularExpression regexprInclude( "@[Ii]nclude *\\( *\"(?<file>.*)\" *\\)" );

/*
 * Konstruktor
 */
IncludeParser::IncludeParser()
{

}

/*
 * Parse Files recursiv nach includes.
 */
Node* IncludeParser::parse(const QString &filePath)
{
    Node *node = new Node(filePath);
    parseFileForIncludes(node);

    return node;
}

/*
 * Finde alles Includes in einen File und gib eine Liste der Include-Files zurück.
 */
void IncludeParser::parseFileForIncludes(Node *node)
{
    QFile srcFile(node->fileName());
    if(!srcFile.open(QFile::ReadOnly))
        node->setError(srcFile.errorString());
    QTextStream in(&srcFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        int pos = line.indexOf('#');
        line = line.left(pos);
        QRegularExpressionMatch match = regexprInclude.match(line);
        if (match.hasMatch())
        {
            QString filepath = QFileInfo(srcFile).absolutePath() + '/' + match.captured("file");
            node->addChild(filepath);
        }
    }
    for (Node *n : node->childList())
    {
        parseFileForIncludes(n);
    }
}

#include <QTextStream>
#include <QFileInfo>
#include "includeparser.h"

void printIncludeList(const Node *node, const int deepth, QTextStream &out);

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    // --------------------------------------------
    // Program arguments
    if (argc < 2)
    {
        out << "Sie müssen eine Datei angeben!" << endl;
        return 0;
    }
    QString filename(argv[1]);
    IncludeParser p;
    Node *root = p.parse(filename);
    printIncludeList(root, -1, out);
    out << endl;

    return 0;
}

/*!
 * \brief Einfache Ausgabe aller Dateien die in includiert werden.
 * \param node
 * \param deepth
 * \param out
 */
void printIncludeList(const Node *node, const int deepth, QTextStream &out)
{
    if (deepth < 0)
    {
        out << "-------------------------------------------------------------------------\n";
        out << "Starte mit Fle: " << node->fileName() << '\n';
        out << "-------------------------------------------------------------------------\n";
    }
    else
    {
        QString space(deepth*2, ' ');
        out << space << QFileInfo(node->fileName()).fileName();
        if (!node->error().isEmpty())
            out << "   (" << node->error() << ')';
        out << '\n';
    }
    for (const Node *n : node->childList())
    {
        printIncludeList(n, deepth+1, out);
    }
}

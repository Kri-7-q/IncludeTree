#include <QTextStream>
#include <QFileInfo>
#include <QList>
#include <QException>
#include <QDir>

class ReadFileException : public QException
{
public:
    ReadFileException(const QString &msg) { msg_ = msg; }

    QString message() const { return msg_; }

private:
    QString msg_;
};

void parseFile(const QString &filepath, const QString &filename, const QStringList &patternList, int depth, QTextStream &out);

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
    QStringList patternList;
    QString filepath(argv[1]);
    if (!filepath.startsWith(QChar('/')))
    {
        filepath = QDir::currentPath() + filepath;
    }
    QFileInfo fileInfo(filepath);
    filepath = fileInfo.absolutePath();
    QString filename = fileInfo.fileName();
    // Such pattern
    for (int index=1; index<argc; ++index)
    {
        patternList << QString(argv[index]);
    }
    try {
        parseFile(filepath, filename, patternList, 0, out);
    } catch (const ReadFileException &e) {
        out << "Fehler: " << e.message() << '\n';
        return 1;
    }

    return 0;
}

/*!
 * \brief parseFile
 * \param filepath
 * \param patternList
 * \param depth
 * \param out
 */
void parseFile(const QString &filepath, const QString &filename, const QStringList &patternList, int depth, QTextStream &out)
{
    // print filename
    out << "\033[34m" << depth << ": " << QString(depth*2, QChar(' ')) << filename << "\033[0m\n";
    // Search in file
    QFile file(filepath + '/' + filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QFileInfo fileInfo(file);
        throw ReadFileException(QString("Konnte Datei '%1' nicht öffnen!").arg(fileInfo.filePath()));
    }
    QTextStream inStream(&file);
    QStringList content;
    while (!inStream.atEnd())
    {
        QString line = inStream.readLine();
        int index = line.indexOf(QChar('#'));
        if (index == 0)
            continue;
        if (index > 0 && index < line.length()-1)
            line = line.left(index);
        // Search for include
        int pos = line.indexOf(QStringLiteral("@INCLUDE"));
        if (pos >= 0)
        {
            pos+=8;
            pos = line.indexOf(QChar('('), pos);
            int end = line.indexOf(")", pos);
            if (line.at(pos+1) == QChar('\"')) pos+=1;
            if (line.at(end-1) == QChar('\"')) end-=1;
            QString filename = line.mid(pos+1, end-pos-1);
            parseFile(filepath, filename, patternList, depth+1, out);
        }
        for (const QString &pattern : patternList)
        {
            if (line.indexOf(pattern) >= 0)
            {
                out << depth << ": " << QString(depth*2, QChar(' ')) << line.trimmed() << '\n';
            }
        }
    }
    file.close();
}

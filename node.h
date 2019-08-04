#ifndef NODE_H
#define NODE_H

#include <QVector>

class Node
{
public:
    Node() {}
    Node(const QString &fileName);

    QString fileName() const;
    void setFileName(const QString &fileName);
    void addChild(const QString &fileName);
    QVector<Node*> childList() const;
    QString error() const { return m_error; }
    void setError(const QString &err) { m_error = err; }
    QString toString() const;

private:
    QString m_fileName;
    QVector<Node*> m_childList;
    QString m_error;
};

#endif // NODE_H

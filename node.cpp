#include "node.h"

Node::Node(const QString &fileName)
    : m_fileName(fileName)
{

}

QString Node::fileName() const
{
    return m_fileName;
}

void Node::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Node::addChild(const QString &fileName)
{
    m_childList << new Node(fileName);
}

QVector<Node *> Node::childList() const
{
    return m_childList;
}

QString Node::toString() const
{
    return m_fileName + "  (" + m_error + ')';
}

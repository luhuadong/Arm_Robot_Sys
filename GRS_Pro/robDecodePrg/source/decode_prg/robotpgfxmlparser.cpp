#include "robotpgfxmlparser.h"
#include <QFile>


RobotPgfXmlParser::RobotPgfXmlParser()
{
}

bool RobotPgfXmlParser::readFile(const QString &fileName)
{
    QFile file(fileName);
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    QDomElement root;

    if (!file.open(QFile::ReadOnly|QFile::Text)) {
        return false;
    }

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        return false;
    }

    root = doc.documentElement();

    if(root.tagName()!= "program") {
        return false;
    }

    parseProgramElement(root);
    return true;
}

void RobotPgfXmlParser::parseProgramElement(const QDomElement &element)
{
    QDomNode child = element.firstChild();

    while (!child.isNull()) {
        if (child.toElement().tagName() == "program") {
            parseModuleElement(child.toElement());
        }
        child = child.nextSibling();
    }

}

void RobotPgfXmlParser::parseModuleElement(const QDomElement &element)
{
    QDomNode child = element.firstChild();

}





























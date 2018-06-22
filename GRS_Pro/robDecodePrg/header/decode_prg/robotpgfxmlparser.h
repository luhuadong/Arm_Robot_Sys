#ifndef ROBOTPGFXMLPARSER_H
#define ROBOTPGFXMLPARSER_H

//#include <QApplication>
#include <QDomDocument>

class RobotPgfXmlParser
{
public:
    RobotPgfXmlParser();
    bool readFile(const QString &fileName);
private:
    void parseProgramElement(const QDomElement &element);
    void parseModuleElement(const QDomElement &element);
};

#endif // ROBOTPGFXMLPARSER_H

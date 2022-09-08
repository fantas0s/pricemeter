#pragma once

#include <QList>
#include <QPair>
#include <QString>
#include <QStack>

class QXmlStreamReader;

class HtmlTableExtractor
{
public:
    HtmlTableExtractor();
    QList<QPair<QString, QString>> extractTable(const QString &htmlSource);
private:
    void html2xml();
    QString readTag(const QString& source, int& startPosition) const;
    QString readTagEnding(const QString& source, int& startPosition, const QString tagName) const;
    void readTableBody(QXmlStreamReader& reader);
    QString m_source;
    QList<QPair<QString, QString>> m_result;
};

#include "htmltableextractor.h"
#include <QXmlStreamReader>

#ifdef DEBUG_NETWORK_ACCESS
#include <QDebug>
#endif

HtmlTableExtractor::HtmlTableExtractor()
{}

QList<QPair<QString, QString>> HtmlTableExtractor::extractTable(const QString &htmlSource)
{
    /* Should return a 24-item list of pairs like:
     * QPair<QString, QString>("00:00", "139.78")
     * QPair<QString, QString>("01:00", "199,92")
     */
    m_source = htmlSource;
    html2xml();
    QXmlStreamReader reader(m_source);

    if (reader.readNext() == QXmlStreamReader::TokenType::StartDocument && !reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name().compare(QLatin1String("table"), Qt::CaseSensitivity::CaseInsensitive) == 0) {
                while (reader.readNextStartElement()) {
                    if (reader.name().compare(QLatin1String("tbody"), Qt::CaseSensitivity::CaseInsensitive) == 0) {
                        readTableBody(reader);
                        break;
                    } else {
                        reader.skipCurrentElement();
                    }
                }
            }
        }
    }
#ifdef DEBUG_NETWORK_ACCESS
    for (auto iter = m_result.constBegin() ; iter != m_result.constEnd() ; iter++)
    {
        qDebug() << "From HTML:" << iter->first << ":" << iter->second;
    }
#endif
    return m_result;
}

void HtmlTableExtractor::html2xml()
{
    QString output;
    int startPosition = m_source.indexOf(QLatin1String("<table"), 0, Qt::CaseSensitivity::CaseInsensitive);
    while (startPosition >= 0)
    {
        /* take stuff like this:
         * <td><li><li><br/></td>
         * And convert it to:
         * <td><li><li><br></br></li></li></td>
         */
        /* read tag <table> and everything in it */
        output = readTag(m_source, startPosition);
        if (output.contains(QLatin1String("data-view-table"))) {
            output.prepend("<?xml version=\"1.0\"?>\n");
            break;
        } else {
            output.clear();
            startPosition = m_source.indexOf(QLatin1String("<table"), startPosition, Qt::CaseSensitivity::CaseInsensitive);
        }
    }
    m_source = output;
}

QString HtmlTableExtractor::readTag(const QString &source, int &startPosition) const
{
    /* The string starts with <xxxxx... */
    QString tagContent;
    tagContent += source[startPosition];
    startPosition++;
    QChar currentChar = source.at(startPosition);
    startPosition++;
    QString tagName;
    while (currentChar != '/' &&
           currentChar != ' ' &&
           currentChar != '>' &&
           startPosition < source.size())
    {
        tagName += currentChar;
        tagContent += currentChar;
        currentChar = source.at(startPosition);
        startPosition++;
    }
    tagName += ">";
    if (currentChar == ' ') {
        /* Tag name complete but extra in tag start element. */
        while (currentChar != '/' &&
               currentChar != '>' &&
               startPosition < source.size())
        {
            tagContent += currentChar;
            currentChar = source.at(startPosition);
            startPosition++;
        }
    }
    if (startPosition >= source.size() ||
        currentChar == '/') {
        /* This is a <br/> tag or end of string. Close the tag. */
        tagContent += "></";
        tagContent += tagName;
        /* Move reading position beyond the ">", if not at end of string. */
        while (currentChar != '>' &&
               startPosition < source.size()) {
            currentChar = source.at(startPosition);
            startPosition++;
        }
    } else {
        /* we're here only if currentChar == '>'
         * So time to read Content.
         */
        tagContent += '>';
        tagContent += readTagEnding(source, startPosition, tagName);
        /* At end of some tag, but is it this tag? */
        if ((startPosition + 2) < source.size()) {
            startPosition += 2;
            if (source.right(source.size() - startPosition).left(tagName.size()).compare(tagName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                /* It's this tag. */
                tagContent += "</";
                tagContent += tagName;
                startPosition += tagName.size();
            } else {
                /* End of some other tag. (case "<list><li></list>")
                 * Close this tag and return. decrease the startPosition to point to '<' */
                tagContent += "</";
                tagContent += tagName;
                startPosition += tagName.size();
            }
        } else {
            /* End of string. Close the tag. */
            tagContent += "</";
            tagContent += tagName;
            startPosition += tagName.size() + 1;
        }
    }
    return tagContent;
}

QString HtmlTableExtractor::readTagEnding(const QString &source, int &startPosition, const QString tagName) const
{
    QString tagContent;
    QChar currentChar = source.at(startPosition);
    startPosition++;
    while (currentChar != '<' &&
           (startPosition + 2) < source.size()) /* Need to peek 2 characters */
    {
        tagContent += currentChar;
        currentChar = source.at(startPosition);
        startPosition++;
    }
    if ((startPosition + 2) >= source.size()) {
        /* End of string. Close the tag. */
        tagContent += "</";
        tagContent += tagName;
    } else {
        /* currentChar == '<' */
        if (source.at(startPosition) != '/') {
            /* Start of a new tag. */
            startPosition--;
            tagContent += readTag(source, startPosition);
            /* We have now received a complete tag into the content, search for end tag again. */
            tagContent += readTagEnding(source, startPosition, tagName);
        } else {
            /* At end of some tag. Set pointer to beginning of the end tag. */
            startPosition--;
        }
    }
    return tagContent;
}

void HtmlTableExtractor::readTableBody(QXmlStreamReader &reader)
{
    /* Inside <tbody>
     * Rest is:
     * <tr><td>00:00 - 01:00</td><td><span>139.78</span></td></tr>
     * <tr><td>01:00 - 02:00</td><td><span>199.92</span></td></tr>
     * etc.
     */
    while (reader.readNextStartElement()) {
        if (reader.name().compare(QLatin1String("tr"), Qt::CaseSensitivity::CaseInsensitive) == 0) {
            QString time;
            QString value;
            if (reader.readNextStartElement()) {
                if (reader.name().compare(QLatin1String("td"), Qt::CaseSensitivity::CaseInsensitive) == 0) {
                    time = reader.readElementText().left(5);
                    if (reader.readNextStartElement()) {
                        if (reader.name().compare(QLatin1String("td"), Qt::CaseSensitivity::CaseInsensitive) == 0) {                    if (reader.readNextStartElement()) {
                                if (reader.name().compare(QLatin1String("span"), Qt::CaseSensitivity::CaseInsensitive) == 0) {
                                    value = reader.readElementText();
                                }
                            }
                        }
                        reader.skipCurrentElement(); /* Break out of "td" */
                    }
                }
                reader.skipCurrentElement(); /* Break out of "tr" */
            }
            if (!time.isEmpty() && !value.isEmpty()) {
                m_result.append(QPair<QString, QString>(time, value));
            }
        } else {
            reader.skipCurrentElement();
        }
    }
}

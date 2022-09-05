#include "xmlfilereader.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

const QLatin1String s_rootElementName("devicelist");
const QLatin1String s_listElementName("device");
const QLatin1String s_titleName("title");
const QLatin1String s_imageName("image");
const QLatin1String s_operationName("operation");
const QLatin1String s_consumptionName("consumption");
const QLatin1String s_secondsName("seconds");
const QLatin1String s_minutesName("minutes");
const QLatin1String s_hoursName("hours");
const QLatin1String s_kWName("kw");
const QLatin1String s_kWhName("kwh");

XmlFileReader::XmlFileReader()
{}

QList<DeviceListItem> XmlFileReader::createItems()
{
    m_items.clear();
    /* Open the XML file and populate model from it. */
    QFile xmlFile(":/appdata/devicelist.xml");
    if (xmlFile.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader;
        reader.setDevice(&xmlFile);
        bool readError = true;
        if (reader.readNext() == QXmlStreamReader::TokenType::StartDocument && !reader.atEnd()) {
            reader.readNextStartElement();
            if (reader.name().compare(s_rootElementName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                readError = parseDeviceList(reader);
            }
        }
        if (readError) {
            qWarning() << "XML File corrupted!";
        }
    } else {
        qWarning() << "File not opened:" << xmlFile.errorString();
    }
    xmlFile.close();
    return m_items;
}

bool XmlFileReader::parseDeviceList(QXmlStreamReader &reader)
{
    bool readError = false;
    while (!reader.atEnd() && !readError && reader.readNextStartElement()) {
        if (reader.name().compare(s_listElementName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            parseDevice(reader);
        } else {
            qWarning() << "Unidentified element in device list:" << reader.name();
            readError = true;
        }
    }
    return readError;
}

void XmlFileReader::parseDevice(QXmlStreamReader &reader)
{
    DeviceListItem newItem;
    QList<OperationListItem> operations;
    while (!reader.atEnd()) {
        bool elementFound = reader.readNextStartElement();
        if (elementFound) {
            if (reader.name().compare(s_operationName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                OperationListItem newOperation = parseOperation(reader);
                if (newOperation.isValid()) {
                    operations.append(newOperation);
                } else {
                    qWarning() << "Found invalid Operation of title" << newOperation.title() << "and Consumption count of" << newOperation.consumptions().size();
                }
            } else {
                if (reader.name().compare(s_titleName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                    newItem.setTitle(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements).trimmed());
                } else if (reader.name().compare(s_imageName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                    const QString textRead = reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements).trimmed();
                    if (!textRead.isEmpty())
                    {
                        newItem.setImageFile(QString("qrc:/images/")+textRead);
                    }
                }
            }
        } else {
            /* Device parsed. */
            break;
        }
    }
    newItem.addOperations(operations);
    if (newItem.isValid()) {
        m_items.append(newItem);
    } else {
        qWarning() << "Found invalid Device of title" << newItem.title();
    }
}

OperationListItem XmlFileReader::parseOperation(QXmlStreamReader &reader)
{
    OperationListItem newItem;
    while (!reader.atEnd() && reader.readNextStartElement()) {
        if (reader.name().compare(s_consumptionName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            const Consumption newConsumption = parseConsumption(reader);
            if (newConsumption.isValid()) {
                newItem.addConsumption(newConsumption);
            } else {
                qWarning() << "Found invalid Consumption of time" << newConsumption.time() << "and kW" << newConsumption.kW();
            }
        } else {
            if (reader.name().compare(s_titleName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
                newItem.setTitle(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements).trimmed());
            }
        }
    }
    return newItem;
}

Consumption XmlFileReader::parseConsumption(QXmlStreamReader &reader)
{
    Consumption newItem;
    int minutes = 0;
    int hours = 0;
    int seconds = 0;
    qreal kW = 0.0;
    qreal kWh = 0.0;
    while (!reader.atEnd() && reader.readNextStartElement()) {
        if (reader.name().compare(s_secondsName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            seconds = toInt(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        } else if (reader.name().compare(s_minutesName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            minutes = toInt(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        } else if (reader.name().compare(s_hoursName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            hours = toInt(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        } else if (reader.name().compare(s_kWName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            kW = toReal(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        } else if (reader.name().compare(s_kWhName, Qt::CaseSensitivity::CaseInsensitive) == 0) {
            kWh = toReal(reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        }
    }
    newItem.setKW(kW);
    newItem.setKWh(kWh);
    if (seconds > 59) {
        minutes += (seconds / 60);
        seconds = seconds % 60;
    }
    if (minutes > 59) {
        hours += (minutes / 60);
        minutes = minutes % 60;
    }
    if (hours > 23) {
        /* 24:00:00 and up are all just "no time" */
        hours = 0;
        minutes = 0;
        seconds = 0;
    }
    newItem.setTime(QTime(hours, minutes, seconds));
    return newItem;
}

int XmlFileReader::toInt(const QString &text) const
{
    bool ok = false;
    QString tempText = text;
    qreal returnValue = tempText.replace(QChar(','), QChar('.')).toInt(&ok);
    if (ok)
        return returnValue;
    else
        return 0;
}

qreal XmlFileReader::toReal(const QString &text) const
{
    bool ok = false;
    QString tempText = text;
    qreal returnValue = tempText.replace(QChar(','), QChar('.')).toFloat(&ok);
    if (ok)
        return returnValue;
    else
        return 0.0;
}

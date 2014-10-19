#include "File.hpp"

#include <QMimeDatabase>
#include <QTextStream>





File::File(const QFileInfo& fileInfo,
           const Config& config,
           QObject* parent) :
    QObject(parent),
    m_path(fileInfo.filePath()),
    m_name(fileInfo.fileName()),
    m_format(initFormat()),
    m_sf2Type(initSf2Type()),
    m_widget(new FileWidget(m_path, config.getLexer(m_format))),
    m_needsToBeSaved(false)
{
    connect(m_widget, &FileWidget::modificationChanged, this, &File::handleModification);
    connect(m_widget, &FileWidget::saveRequiered, this, &File::saveContent);
}





File::File(QObject* parent) :
    QObject(parent),
    m_path(),
    m_name(),
    m_format(FileFormat::Unknown),
    m_sf2Type(Sf2FileType::Unknown),
    m_widget(0),
    m_needsToBeSaved(false)
{
    
}





const QString& File::getPath() const
{
    return m_path;
}





const QString& File::getName() const
{
    return m_name;
}





QString File::getDisplayableName() const
{
    if (m_needsToBeSaved)
    {
        return m_name + "*";
    }
    
    return m_name;
}





FileFormat::Enum File::getFormat() const
{
    return m_format;
}





Sf2FileType::Enum File::getSf2Type() const
{
    return m_sf2Type;
}





FileWidget* File::getWidget() const
{
    return m_widget;
}





bool File::doesNeedToBeSaved() const
{
    return m_needsToBeSaved;
}





void File::save(QXmlStreamWriter& outputStream) const
{
    outputStream.writeStartElement(QStringLiteral("file"));
    outputStream.writeAttribute(QStringLiteral("path"), m_path);
    outputStream.writeAttribute(QStringLiteral("format"), getFormatName());
    outputStream.writeAttribute(QStringLiteral("sf2type"), getSf2TypeName());
    
    outputStream.writeEndElement();// file
}





bool File::load(QXmlStreamReader& inputStream,
                const Config& config)
{
    if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "file")
    {
        QXmlStreamAttributes attributes(inputStream.attributes());
        if (attributes.hasAttribute(QStringLiteral("path")))
        {
            m_path = attributes.value(QStringLiteral("path")).toString();
            QFileInfo fileInfo(m_path);
            m_name = fileInfo.fileName();
        }
        else
        {
            return false;
        }
        if (attributes.hasAttribute(QStringLiteral("format")))
        {
            setFormatFromName(attributes.value(QStringLiteral("format")).toString());
        }
        else
        {
            return false;
        }
        if (attributes.hasAttribute(QStringLiteral("sf2type")))
        {
            setSf2TypeFromName(attributes.value(QStringLiteral("sf2type")).toString());
        }
        else
        {
            return false;
        }
        
        if (m_widget)
        {
            delete m_widget;
        }
        m_widget = new FileWidget(m_path, config.getLexer(m_format));
        connect(m_widget, &FileWidget::modificationChanged, this, &File::handleModification);
        connect(m_widget, &FileWidget::saveRequiered, this, &File::saveContent);
        
        // We go to the end of the file tag.
        inputStream.readNextStartElement();
        while (!inputStream.atEnd() && inputStream.name() != "file")
        {
            inputStream.readNextStartElement();
        }
        
        return true;
    }
    
    return false;
}





void File::handleModification(bool hasModification)
{
    if (m_needsToBeSaved != hasModification)
    {
        m_needsToBeSaved = hasModification;
        emit displayableNameChanged(getDisplayableName());
    }
}





void File::saveContent()
{
    if (m_needsToBeSaved)
    {
        QFile file(m_path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QTextStream outputStream(&file);
            outputStream << m_widget->text();
            
            file.close();
            
            m_needsToBeSaved = false;
            m_widget->setModified(false);
            emit displayableNameChanged(getDisplayableName());
        }
        else
        {
            // Error.
        }
    }
}





FileFormat::Enum File::initFormat() const
{
    QMimeDatabase mimeDatabase;
    QMimeType mimeType(mimeDatabase.mimeTypeForFile(m_path));
    
    if (mimeType.name() == "text/css")
    {
        return FileFormat::CSS;
    }
    else if (mimeType.name() == "text/html")
    {
        return FileFormat::HTML;
    }
    else if (mimeType.name() == "application/javascript")
    {
        return FileFormat::JS;
    }
    else if (mimeType.name() == "application/x-php")
    {
        return FileFormat::PHP;
    }
    else if (mimeType.name() == "application/xml")
    {
        return FileFormat::XML;
    }
    else if (mimeType.name() == "application/x-yaml")
    {
        return FileFormat::YAML;
    }
    else if (mimeType.name() == "text/plain")
    {
        if (m_path.endsWith(".html.twig"))
        {
            return FileFormat::HTML;
        }
    }
    
    return FileFormat::Unknown;
}





Sf2FileType::Enum File::initSf2Type() const
{
    if (m_format == FileFormat::PHP)
    {
        if (m_path.endsWith("Controller.php"))
        {
            return Sf2FileType::Controller;
        }
        else if (m_path.contains("/Entity/"))
        {
            return Sf2FileType::Entity;
        }
        else if (m_path.endsWith("Type.php") || m_path.endsWith("Form.php"))
        {
            return Sf2FileType::FormType;
        }
        else if (m_path.endsWith("Repository.php"))
        {
            return Sf2FileType::Repository;
        }
    }
    else if (m_format == FileFormat::XML || m_format == FileFormat::YAML)
    {
        return Sf2FileType::Configuration;
    }
    else if (m_format == FileFormat::HTML)
    {
        return Sf2FileType::View;
    }
    
    return Sf2FileType::Unknown;
}





QString File::getFormatName() const
{
    switch (m_format)
    {
        case FileFormat::Unknown:
            return QStringLiteral("Unknown");
            
        case FileFormat::CSS:
            return QStringLiteral("CSS");
            
        case FileFormat::HTML:
            return QStringLiteral("HTML");
            
        case FileFormat::JS:
            return QStringLiteral("JS");
            
        case FileFormat::PHP:
            return QStringLiteral("PHP");
            
        case FileFormat::XML:
            return QStringLiteral("XML");
            
        case FileFormat::YAML:
            return QStringLiteral("YAML");
    }
    
    return QString();
}





void File::setFormatFromName(const QString& format)
{
    if (format == "Unknown")
    {
        m_format = FileFormat::Unknown;
    }
    else if (format == "CSS")
    {
        m_format = FileFormat::CSS;
    }
    else if (format == "HTML")
    {
        m_format = FileFormat::HTML;
    }
    else if (format == "JS")
    {
        m_format = FileFormat::JS;
    }
    else if (format == "PHP")
    {
        m_format = FileFormat::PHP;
    }
    else if (format == "XML")
    {
        m_format = FileFormat::XML;
    }
    else if (format == "YAML")
    {
        m_format = FileFormat::YAML;
    }
}





QString File::getSf2TypeName() const
{
    switch (m_sf2Type)
    {
        case Sf2FileType::Unknown:
            return QStringLiteral("Unknown");
            
        case Sf2FileType::Configuration:
            return QStringLiteral("Configuration");
            
        case Sf2FileType::Controller:
            return QStringLiteral("Controller");
            
        case Sf2FileType::Entity:
            return QStringLiteral("Entity");
            
        case Sf2FileType::FormType:
            return QStringLiteral("FormType");
            
        case Sf2FileType::Repository:
            return QStringLiteral("Repository");
            
        case Sf2FileType::Service:
            return QStringLiteral("Service");
            
        case Sf2FileType::View:
            return QStringLiteral("View");
    }
    
    return QString();
}





void File::setSf2TypeFromName(const QString& sf2type)
{
    if (sf2type == "Unknown")
    {
        m_sf2Type = Sf2FileType::Unknown;
    }
    else if (sf2type == "Configuration")
    {
        m_sf2Type = Sf2FileType::Configuration;
    }
    else if (sf2type == "Controller")
    {
        m_sf2Type = Sf2FileType::Controller;
    }
    else if (sf2type == "Entity")
    {
        m_sf2Type = Sf2FileType::Entity;
    }
    else if (sf2type == "FormType")
    {
        m_sf2Type = Sf2FileType::FormType;
    }
    else if (sf2type == "Repository")
    {
        m_sf2Type = Sf2FileType::Repository;
    }
    else if (sf2type == "Service")
    {
        m_sf2Type = Sf2FileType::Service;
    }
    else if (sf2type == "View")
    {
        m_sf2Type = Sf2FileType::View;
    }
}

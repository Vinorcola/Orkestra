#include "PHPConfig.hpp"





PHPConfig::PHPConfig() :
    m_command()
{
    
}





void PHPConfig::setCommand(const QString& command)
{
    m_command = command;
}





const QString& PHPConfig::getCommand() const
{
    return m_command;
}





void PHPConfig::save(QXmlStreamWriter& outputStream) const
{
    outputStream.writeStartElement(QStringLiteral("php"));
    outputStream.writeAttribute(QStringLiteral("command"), m_command);
    
    outputStream.writeEndElement();// php
}





bool PHPConfig::load(QXmlStreamReader& inputStream,
                     const Config& /*config*/)
{
    // Go to php.
    if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "php")
    {
        QXmlStreamAttributes attributes(inputStream.attributes());
        if (attributes.hasAttribute(QStringLiteral("command")))
        {
            m_command = attributes.value(QStringLiteral("command")).toString();
        }
        
        // We go to the end of the php tag.
        inputStream.readNextStartElement();
        while (!inputStream.atEnd() && inputStream.name() != "php")
        {
            inputStream.readNextStartElement();
        }
        
        return true;
    }
    
    return false;
}

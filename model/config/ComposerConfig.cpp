#include "ComposerConfig.hpp"





ComposerConfig::ComposerConfig() :
    m_command()
{
    
}





void ComposerConfig::setCommand(const QString& command)
{
    m_command = command;
}





const QString& ComposerConfig::getCommand() const
{
    return m_command;
}





void ComposerConfig::save(QXmlStreamWriter& outputStream) const
{
    outputStream.writeStartElement(QStringLiteral("composer"));
    outputStream.writeAttribute(QStringLiteral("command"), m_command);
    
    outputStream.writeEndElement();// composer
}





bool ComposerConfig::load(QXmlStreamReader& inputStream,
                          const Config& /*config*/)
{
    // Go to composer.
    if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "composer")
    {
        QXmlStreamAttributes attributes(inputStream.attributes());
        if (attributes.hasAttribute(QStringLiteral("command")))
        {
            m_command = attributes.value(QStringLiteral("command")).toString();
        }
        
        // We go to the end of the composer tag.
        inputStream.readNextStartElement();
        while (!inputStream.atEnd() && inputStream.name() != "composer")
        {
            inputStream.readNextStartElement();
        }
        
        return true;
    }
    
    return false;
}

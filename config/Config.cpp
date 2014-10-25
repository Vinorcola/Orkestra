#include "Config.hpp"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QXmlStreamWriter>

#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

#include "model/businessobjects/ProjectManager.hpp"





Config::Config(QObject* parent) :
    QObject(parent),
    m_directory(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + "/orkestra"),
    m_file(m_directory + "/config.xml"),
    m_lexerList(),
    m_phpConfig(),
    m_composerConfig()
{
    if (!QFileInfo::exists(m_directory))
    {
        QDir configDirectory(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first());
        configDirectory.mkdir("orkestra");
    }
    
    QFont monospace("Monospace", 9);
    
    // Create CSS lexer.
    QsciLexerCSS* cssLexer(new QsciLexerCSS(this));
    cssLexer->setFont(monospace);
    m_lexerList.insert(FileFormat::CSS, cssLexer);
    
    // Create HTML and PHP lexer.
    QsciLexerHTML* htmlLexer(new QsciLexerHTML(this));
    htmlLexer->setFont(monospace);
    m_lexerList.insert(FileFormat::HTML, htmlLexer);
    m_lexerList.insert(FileFormat::PHP, htmlLexer);
    
    // Create JS lexer.
    QsciLexerJavaScript* jsLexer(new QsciLexerJavaScript(this));
    jsLexer->setFont(monospace);
    m_lexerList.insert(FileFormat::JS, jsLexer);
    
    // Create XML lexer.
    QsciLexerXML* xmlLexer(new QsciLexerXML(this));
    xmlLexer->setFont(monospace);
    m_lexerList.insert(FileFormat::XML, xmlLexer);
    
    // Create YAML lexer.
    QsciLexerYAML* yamlLexer(new QsciLexerYAML(this));
    yamlLexer->setFont(monospace);
    m_lexerList.insert(FileFormat::YAML, yamlLexer);
}





QsciLexer* Config::getLexer(const FileFormat::Enum format) const
{
    return m_lexerList.value(format, 0);
}





PHPConfig& Config::getPHPConfig()
{
    return m_phpConfig;
}





ComposerConfig& Config::getComposerConfig()
{
    return m_composerConfig;
}





void Config::save(const ProjectManager* projectManager)
{
    if (m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QXmlStreamWriter outputStream(&m_file);
        
        outputStream.setAutoFormatting(true);
        outputStream.writeStartDocument();
        outputStream.writeDTD(QStringLiteral("<!DOCTYPE OrkestraConfig>"));
        
        outputStream.writeStartElement(QStringLiteral("orkestra"));
        outputStream.writeAttribute(QStringLiteral("version"), QStringLiteral("0.1"));
        
        outputStream.writeStartElement(QStringLiteral("config"));
        
        m_phpConfig.save(outputStream);
        m_composerConfig.save(outputStream);
        projectManager->save(outputStream);
        
        outputStream.writeEndElement();// config
        
        outputStream.writeEndElement();// orkestra
        
        outputStream.writeEndDocument();
        
        m_file.close();
    }
    else
    {
        qDebug() << "Can not open file:" << m_file.errorString();
    }
}





void Config::load(ProjectManager* projectManager)
{
    if (QFileInfo::exists(m_file.fileName()))
    {
        if (m_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QXmlStreamReader inputStream(&m_file);
            
            // Check DTD.
            while (!inputStream.atEnd() && inputStream.readNext() != QXmlStreamReader::DTD) {}
            if (inputStream.dtdName() != "OrkestraConfig") return;
            
            // Go to config.
            while (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() != "orkestra") {}
            while (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() != "config") {}
            
            m_phpConfig.load(inputStream, *this);
            m_composerConfig.load(inputStream, *this);
            projectManager->load(inputStream, *this);
            
            m_file.close();
        }
        else
        {
            qDebug() << "Can not open file:" << m_file.errorString();
        }
    }
}

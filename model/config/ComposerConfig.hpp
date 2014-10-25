#ifndef COMPOSERCONFIG_HPP
#define COMPOSERCONFIG_HPP

#include <QString>

#include "config/ConfigurableInterface.hpp"





class ComposerConfig : public ConfigurableInterface
{
    private:
        QString m_command;
        
        
        
    public:
        ComposerConfig();
        
        
        
        void setCommand(const QString& command);
        
        
        
        const QString& getCommand() const;
        
        
        
        virtual void save(QXmlStreamWriter& outputStream) const;
        
        
        
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config);
};

#endif // COMPOSERCONFIG_HPP

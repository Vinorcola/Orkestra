#ifndef PHPCONFIG_HPP
#define PHPCONFIG_HPP

#include "config/ConfigurableInterface.hpp"





class PHPConfig : ConfigurableInterface
{
    private:
        QString m_command;
        
        
        
    public:
        PHPConfig();
        
        
        
        void setCommand(const QString& command);
        
        
        
        const QString& getCommand() const;
        
        
        
        virtual void save(QXmlStreamWriter& outputStream) const;
        
        
        
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config);
};

#endif // PHPCONFIG_HPP

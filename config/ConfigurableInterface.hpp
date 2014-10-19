#ifndef CONFIGURABLEINTERFACE_HPP
#define CONFIGURABLEINTERFACE_HPP

#include <QXmlStreamWriter>

class Config;





class ConfigurableInterface
{
    public:
        /**
         * @brief Saves the object into the output configuration stream @a outputStream.
         * 
         * @param outputStream The output configuration stream.
         */
        virtual void save(QXmlStreamWriter& outputStream) const = 0;
        
        
        
        /**
         * @brief Loads the object from the input configuration stream @a inputStream.
         * 
         * @param inputStream The input configuration stream.
         * @param config The global configuration.
         * @return @b @c true if the load succed, @b @c false otherwise.
         */
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config) = 0;
};

#endif // CONFIGURABLEINTERFACE_HPP

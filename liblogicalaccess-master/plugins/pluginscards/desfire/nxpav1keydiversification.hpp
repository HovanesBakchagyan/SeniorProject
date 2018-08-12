#include "logicalaccess/cards/keydiversification.hpp"
#include "logicalaccess/key.hpp"
#include "nxpkeydiversification.hpp"
#include <vector>

#ifndef NXPAV1KEYDIVERSIFICATION_HPP__
#define NXPAV1KEYDIVERSIFICATION_HPP__

#include <string>

namespace logicalaccess
{
    class LIBLOGICALACCESS_API NXPAV1KeyDiversification : public NXPKeyDiversification
    {
    public:
        virtual void initDiversification(std::vector<unsigned char> identifier, int AID, std::shared_ptr<Key> key, unsigned char keyno, std::vector<unsigned char>& diversify);
        virtual std::vector<unsigned char> getDiversifiedKey(std::shared_ptr<Key> key, std::vector<unsigned char> diversify);

        NXPAV1KeyDiversification() {};
        virtual ~NXPAV1KeyDiversification() {};

        virtual std::string getType() { return "NXPAV1"; };

        virtual void serialize(boost::property_tree::ptree &parentNode) override;

        virtual void unSerialize(boost::property_tree::ptree& node) override;
        virtual std::string getDefaultXmlNodeName() const { return "NXPAV1KeyDiversification"; };

    private:
    };
}

#endif
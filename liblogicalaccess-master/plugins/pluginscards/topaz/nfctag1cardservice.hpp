/**
* \file nfctag1cardservice.hpp
* \author Maxime C. <maxime-dev@islog.com>
* \brief NFC Tag Type 1 card service.
*/

#ifndef LOGICALACCESS_NFCTAG1CARDSERVICE_HPP
#define LOGICALACCESS_NFCTAG1CARDSERVICE_HPP

#include "logicalaccess/services/nfctag/nfctagcardservice.hpp"
#include "logicalaccess/services/nfctag/ndefmessage.hpp"
#include "topazchip.hpp"

namespace logicalaccess
{
	/**
	* \brief The NFC Tag 1 storage card service base class.
	*/
	class LIBLOGICALACCESS_API NFCTag1CardService : public NFCTagCardService
	{
	public:

		/**
		* \brief Constructor.
		* \param chip The chip.
		*/
		NFCTag1CardService(std::shared_ptr<Chip> chip) : NFCTagCardService(chip) {};

		virtual ~NFCTag1CardService() {};

		virtual std::shared_ptr<logicalaccess::NdefMessage> readNDEF();

		virtual void writeNDEF(std::shared_ptr<logicalaccess::NdefMessage> records);

		virtual void eraseNDEF();

	protected:

        std::shared_ptr<TopazChip> getTopazChip() { return std::dynamic_pointer_cast<TopazChip>(getChip()); };
	};
}

#endif /* LOGICALACCESS_NFCTAG1CARDSERVICE_HPP */
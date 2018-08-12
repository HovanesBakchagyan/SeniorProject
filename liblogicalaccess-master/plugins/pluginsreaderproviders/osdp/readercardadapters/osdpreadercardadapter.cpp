/**
 * \file osdpreadercardadapter.cpp
 * \author Adrien J. <adrien-dev@islog.com>
 * \brief OSDP reader card adapter.
 */

#include "osdpreadercardadapter.hpp"
#include "logicalaccess/bufferhelper.hpp"

namespace logicalaccess
{
	OSDPReaderCardAdapter::OSDPReaderCardAdapter(std::shared_ptr<OSDPCommands> command, unsigned char address, std::shared_ptr<DESFireISO7816ResultChecker> resultChecker)
		: ISO7816ReaderCardAdapter(), m_commands(command), d_resultChecker(resultChecker), m_address(address)
	{
	}


	std::vector<unsigned char> OSDPReaderCardAdapter::sendCommand(const std::vector<unsigned char>& command, long timeout)
	{
		std::vector<unsigned char> osdpCommand;
		std::shared_ptr<OSDPChannel> channel = m_commands->getChannel();
		if (channel->isSCB)
		{
			channel->setSecurityBlockData(std::vector<unsigned char>(2));
			channel->setSecurityBlockType(OSDPSecureChannelType::SCS_17); //Enable MAC and Data Security
		}

		channel->setCommandsType(OSDPCommandsType::XWR);
		osdpCommand.push_back(0x01); //XRW_PROFILE 0x01
		osdpCommand.push_back(0x01); //XRW_PCMND 0x01
		osdpCommand.push_back(m_address);
		osdpCommand.insert(osdpCommand.end(), command.begin(), command.end());
		LOG(LogLevel::INFOS) << "OSDP Cmd: " << BufferHelper::getHex(osdpCommand);
		channel->setData(osdpCommand);
		//Transparent Content Send Request
		std::shared_ptr<OSDPChannel> result = m_commands->transmit();

		if (result->getCommandsType() == OSDPCommandsType::NAK)
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "NAK: Impossible to send Transparent Content");

		if (result->getCommandsType() == OSDPCommandsType::ACK)
		{
			LOG(LogLevel::INFOS) << "OSDP: No XRD recieve, try to send poll";
			result = m_commands->poll();
		}

		std::vector<unsigned char>& data = result->getData();

		if (result->getCommandsType() != OSDPCommandsType::XRD
			|| (result->getCommandsType() == OSDPCommandsType::XRD && data[0x01] != 0x02)) //is Not APDU answer - osdp_PR01SCREP = 0x02
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Impossible to send Transparent Content");

		std::vector<unsigned char> res(data.begin() + 4, data.end());

		if (res.size() > 0 && d_resultChecker)
		{
			d_resultChecker->CheckResult(&res[0], res.size());
		}
		return res;
	}

	OSDPReaderCardAdapter::~OSDPReaderCardAdapter()
	{
		//Restore Profile 0x00 command
		/*std::shared_ptr<OSDPChannel> result = m_commands->setProfile(0x00);
		if (result->getCommandsType() != OSDPCommandsType::ACK)
			LOG(LogLevel::ERRORS) << "Impossible to restore Profile 0x00";*/
	}
};
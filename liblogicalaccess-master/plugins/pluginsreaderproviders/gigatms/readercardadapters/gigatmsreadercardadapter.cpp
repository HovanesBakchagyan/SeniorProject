/**
 * \file gigatmsreadercardadapter.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief GIGA-TMS reader/card adapter.
 */

#include "gigatmsreadercardadapter.hpp"
#include "logicalaccess/logs.hpp"
#include "logicalaccess/myexception.hpp"

namespace logicalaccess
{
    const unsigned char GigaTMSReaderCardAdapter::STX1 = 0xAE;
	const unsigned char GigaTMSReaderCardAdapter::STX2 = 0x00;

    GigaTMSReaderCardAdapter::GigaTMSReaderCardAdapter()
        : ReaderCardAdapter()
    {
    }

    GigaTMSReaderCardAdapter::~GigaTMSReaderCardAdapter()
    {
    }

    std::vector<unsigned char> GigaTMSReaderCardAdapter::adaptCommand(const std::vector<unsigned char>& command)
    {
		std::vector<unsigned char> cmd;
		cmd.push_back(GigaTMSReaderCardAdapter::STX1);
		cmd.push_back(GigaTMSReaderCardAdapter::STX2);
        unsigned char crc = static_cast<unsigned char>(command.size());
        cmd.push_back(crc);
		cmd.insert(cmd.end(), command.begin(), command.end());
		for (size_t i = 0; i < command.size(); ++i)
		{
			crc ^= command[i];
		}
		cmd.push_back(crc);

        return cmd;
    }

    std::vector<unsigned char> GigaTMSReaderCardAdapter::adaptAnswer(const std::vector<unsigned char>& answer)
    {
        EXCEPTION_ASSERT_WITH_LOG(answer.size() >= 3, LibLogicalAccessException, "Bad command response. Data length too small.");
		EXCEPTION_ASSERT_WITH_LOG(answer[0] == GigaTMSReaderCardAdapter::STX1 && answer[1] == GigaTMSReaderCardAdapter::STX2, LibLogicalAccessException, "Bad command response. STX bytes doesn't match.");
		EXCEPTION_ASSERT_WITH_LOG(answer.size() >= static_cast<size_t>(3 + answer[2] + 1), LibLogicalAccessException, "Bad command response. Data length doesn't match.");

		unsigned char crc = 0x00;
		for (size_t i = 2; i < answer.size() - 1; ++i)
		{
			crc ^= answer[i];
		}

		EXCEPTION_ASSERT_WITH_LOG(crc == answer[3 + answer[2]], LibLogicalAccessException, "Bad command response. CRC doesn't match.");

		return std::vector<unsigned char>(answer.begin() + 5, answer.begin() + 3 + answer[2]);
    }
}
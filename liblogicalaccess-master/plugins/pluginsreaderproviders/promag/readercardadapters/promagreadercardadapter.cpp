/**
 * \file promagreadercardadapter.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Promag reader/card adapter.
 */

#include "promagreadercardadapter.hpp"

namespace logicalaccess
{
    const unsigned char PromagReaderCardAdapter::STX = 0x02;
    const unsigned char PromagReaderCardAdapter::ESC = 0x1b;
    const unsigned char PromagReaderCardAdapter::BEL = 0x07;
    const unsigned char PromagReaderCardAdapter::CR = 0x0d;
    const unsigned char PromagReaderCardAdapter::LF = 0x0a;

    PromagReaderCardAdapter::PromagReaderCardAdapter()
        : ReaderCardAdapter()
    {
    }

    PromagReaderCardAdapter::~PromagReaderCardAdapter()
    {
    }

    std::vector<unsigned char> PromagReaderCardAdapter::adaptCommand(const std::vector<unsigned char>& command)
    {
        return command;
    }

    std::vector<unsigned char> PromagReaderCardAdapter::adaptAnswer(const std::vector<unsigned char>& answer)
    {
        return answer;
    }
}
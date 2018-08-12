#include "stidprgreadercardadapter.hpp"
#include <logicalaccess/logs.hpp>

using namespace logicalaccess;

std::vector<unsigned char>
STidPRGReaderCardAdapter::adaptCommand(const std::vector<unsigned char> &command)
{
    std::vector<uint8_t> full_cmd;
    full_cmd.push_back(0x02);
    full_cmd.insert(full_cmd.end(), command.begin(), command.end());

    full_cmd.push_back(compute_crc(command));

    full_cmd.push_back(0x03);

    return full_cmd;
}

std::vector<unsigned char>
STidPRGReaderCardAdapter::adaptAnswer(const std::vector<unsigned char> &answer)
{
    // Ignore empty response
    if (answer.size() == 0)
        return answer;
    EXCEPTION_ASSERT_WITH_LOG(answer.size() >= 3, LibLogicalAccessException,
                              "Response is too short.");
    EXCEPTION_ASSERT_WITH_LOG(answer[0] == 0x02, LibLogicalAccessException,
                              "Invalid 'Start of Frame byte'.");
    EXCEPTION_ASSERT_WITH_LOG(answer[answer.size() - 1] == 0x03,
                              LibLogicalAccessException,
                              "Invalid 'End of Frame byte'.");

    // Drop "Start of Frame" as well as "End of Frame" and "CRC" bytes.
    std::vector<uint8_t> ret(answer.begin() + 1, answer.end() - 2);
    EXCEPTION_ASSERT_WITH_LOG(answer[answer.size() - 2] == compute_crc(ret),
                              LibLogicalAccessException, "Invalid CRC");
    return ret;
}

uint8_t STidPRGReaderCardAdapter::compute_crc(const std::vector<uint8_t> &cmd)
{
    EXCEPTION_ASSERT_WITH_LOG(cmd.size() >= 3, LibLogicalAccessException,
                              "Command / Response is too short.");
    uint8_t crc = 0;
    auto itr    = cmd.begin();
    while (itr != cmd.end())
    {
        crc ^= *itr;
        ++itr;
    }
    return crc;
}

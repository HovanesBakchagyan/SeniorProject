#include "stidprgbufferparser.hpp"
#include "logicalaccess/logs.hpp"

using namespace logicalaccess;

std::vector<unsigned char> STidPRGBufferParser::getValidBuffer(
    boost::circular_buffer<unsigned char> &circular_buffer)
{
    std::vector<unsigned char> result;
    std::vector<unsigned char> tmp;
    tmp.assign(circular_buffer.begin(), circular_buffer.end());

    if (circular_buffer.size() >= 6)
    {
        // Look for start of frame "0x02"
        if (circular_buffer[0] != 0x02)
            return result;
        // buffer[1] and buffer[2] are SW1/2
        // 2: SW1/2  ---- 1: LRC ---- 1: len ---- 2: Start/End of frame
        auto len = circular_buffer[3] + 2 + 1 + 1 + 2;
        // Advance until "end of frame", 0x03
        for (auto it = circular_buffer.begin(); it != circular_buffer.end(); ++it)
        {
            if (*it == 0x03 && std::distance(circular_buffer.begin(), it) == len - 1)
            {
                result.assign(circular_buffer.begin(), it + 1);
                circular_buffer.assign(it + 1, circular_buffer.end());
                circular_buffer.set_capacity(1024);
                return result;
            }
        }
    }
    return result;
}

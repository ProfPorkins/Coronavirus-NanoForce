module;

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>

export module misc;

namespace misc
{
    export constexpr float PER_MS_TO_US = static_cast<float>(1.0f / std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1)).count());

    export std::chrono::microseconds msTous(std::chrono::microseconds ms)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(ms);
    }

    export std::string timeToMMMSSString(std::chrono::milliseconds time)
    {
        //
        // Convert time played to hours, minutes, seconds
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time - minutes);
        auto partialSeconds = (time - minutes - seconds).count() / 1000.0f;

        //
        // Render the time played
        std::ostringstream strm;
        strm << std::setfill('0') << std::setw(3) << minutes.count() << ":";
        strm << std::setfill('0') << std::setw(2) << seconds.count() << ".";
        strm << std::setfill('0') << std::setw(2) << static_cast<int>(partialSeconds * 100.0f);

        return strm.str();
    }

} // namespace misc
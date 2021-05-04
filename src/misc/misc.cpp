/*
Copyright (c) 2021 James Dean Mathias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "misc.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace misc
{

    std::chrono::microseconds msTous(std::chrono::microseconds ms)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(ms);
    }

    std::string timeToMMMSSString(std::chrono::milliseconds time)
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

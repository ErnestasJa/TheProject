#ifndef STRING_UTIL_H_INCLUDED
#define STRING_UTIL_H_INCLUDED

#include <string>

namespace util
{
    std::string GetParentDirectory(std::string dir, std::string separator)
    {
        auto pos = dir.find_last_of(separator);

        if(pos==std::string::npos) return dir;

        if( (dir.length()-pos) == separator.length())
            pos = dir.find_last_of(separator, pos-separator.length());

        dir = dir.substr(0,pos);

        return dir;
    }

}

#endif // STRING_UTIL_H_INCLUDED

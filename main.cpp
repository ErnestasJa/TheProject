#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <string>
#include <string.h>
#include <stdarg.h>  // for va_start, etc
#include <memory>    // for std::unique_ptr

std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

using namespace std;

uint32_t FindTagEnd(string str,const char tag)
{
    std::string thistagopen=string_format("['%c",tag);
    std::string thistagclose=string_format("[%c']",tag);

    uint32_t tagstart=str.find(thistagopen);

    uint32_t naiveend=str.find(thistagclose);

    uint32_t prevtrack=0;
    uint32_t track=tagstart;
    uint32_t realend=naiveend;
    while(1)
    {
        prevtrack=str.find(thistagopen,track+3);
        bool nested=prevtrack!=string::npos;
        if(nested)
        {
            printf("nested\n");
            track=str.find(thistagopen,naiveend+4);
            naiveend=str.find(thistagclose,prevtrack+3);
        }
        else
            break;
    }
    realend=naiveend;
    return realend;
}

int main()
{
    string a="['s]Tags ['s]inside tags[s'] ['s]by the side of inside tags[s'][s']";

    uint32_t firsttag=a.find("['");
    char tag=a.substr(firsttag+2,1)[0];
    uint32_t tagclose=a.find("]",firsttag);
    uint32_t tagend=FindTagEnd(a,tag);
    uint32_t taglength=tagend-(tagclose+1);

    printf("Tag end at %d\n",tagend);

    std::string before=a.substr(tagclose+1,taglength);
    std::string after=a.substr(tagend+4);
    printf("before: %s  after: %s\n",before.c_str(),after.c_str());
    a=before+after;

    firsttag=a.find("['");
    tag=a.substr(firsttag+2,1)[0];
    tagclose=a.find("]",firsttag);
    tagend=FindTagEnd(a,tag);
    taglength=tagend-(tagclose+1);

    printf("Tag end at %d\n",tagend);

    before=a.substr(tagclose+1,taglength);
    after=a.substr(tagend+4);
    printf("before: %s  after: %s\n",before.c_str(),after.c_str());
    a=before+after;

    printf("Return value: %s\n",a.c_str());
    return 0;
}

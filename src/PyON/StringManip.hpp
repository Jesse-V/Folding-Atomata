
#ifndef STRING_MANIP
#define STRING_MANIP

#include <string>
#include <vector>

typedef std::string Str;
typedef std::vector<Str> StringVector;

class StringManip
{
    public:
        static Str between(const Str& str, const Str& header, 
                           const Str& footer, std::size_t start = 0
        );
        static StringVector explodeAndTrim(const Str& str, char delim, 
                                           const Str& whitespaces
        );
        static StringVector explode(const Str& str, char delim);
        static Str trim(const Str& str, const Str& whitespaces);
};

#endif

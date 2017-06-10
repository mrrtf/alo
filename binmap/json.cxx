#include "json.h"

std::ostream& curlybrace(std::ostream& out, void (*f)(std::ostream& out)) {
    out << "{";
    (*f)(out);
    out << "}";
    return out;
}

std::string quote(const char* s)
{
    std::string r = "\"";
    r += s;
    r += "\"";
    return r;
}

void outputArray(std::ostream& out, const char* key, std::vector<int> v) {
    std::sort(v.begin(),v.end());
    out << quote(key) << ":[";
        for ( std::vector<int>::size_type i = 0; i < v.size(); ++i) {
                out << v[i];
                if ( i < v.size()-1) out <<",";
            }
            out << "]";
}


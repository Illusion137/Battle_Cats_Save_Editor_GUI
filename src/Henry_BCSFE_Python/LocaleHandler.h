#pragma once

#include <iostream>
#include <vector>

namespace BCSFE {
    using namespace std;

    typedef char* Bytes;
    typedef string Str;
    typedef void* Any;

    class LocalManager {
        LocalManager(Str locale);
        void parse();
        Str get_key(Str property, Str key);
        Str search_key(Str key);
        static LocalManager from_config();
        static vector<Str> get_locales();
    };

}
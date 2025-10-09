#pragma once

#include <string>
#include <string_view>
#include <map>

namespace Utils::XLSX {

    using std::string;
    using std::string_view;
    using std::map;
    using std::pair;

    struct CellCoords {
        unsigned short row;
        unsigned short col;

        auto operator<=>(const CellCoords& other) const = default;
    };
    
    map<string, map<CellCoords, string> > read_xlsx(string_view path);
    void write_xlsx(string_view path, const map<string, map<CellCoords, string> >& data);
}
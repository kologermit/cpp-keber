#include <utils/XLSX/XLSX.hpp>
#include <xlsxio_read.h>
#include <xlsxio_write.h>
#include <xlsxwriter.h>
#include <fmt/core.h>
#include <stdexcept>
#include <memory>

namespace Utils::XLSX {

    using std::runtime_error;
    using std::shared_ptr;

    map<string, map<CellCoords, string> > read_xlsx(string_view path) {
        auto handle = xlsxioread_open(path.data());
        if (!handle) {
            throw runtime_error(fmt::format("Failed opening {} for reading!", path));
        }
        
        map<string, map<CellCoords, string> > result;
        xlsxioread_list_sheets(handle, [](const char* name, void* callbackdata) {
            static_cast<map<string, map<CellCoords, string> >*>(callbackdata)->insert({name, {}});
            return 0;
        }, &result);

        char* value = nullptr;

        for (auto& [sheet_name, sheet_matrix] : result) {
            auto sheet = xlsxioread_sheet_open(handle, sheet_name.data(), 0);
            if (sheet == NULL) {
                xlsxioread_close(handle);
                throw runtime_error(fmt::format("Failed opening sheet {}:{} for reading!", path, sheet_name));
            }

            for (unsigned short row = 0; xlsxioread_sheet_next_row(sheet); row++) {
                for (unsigned short col = 0; (value = xlsxioread_sheet_next_cell(sheet)); col++) {
                    sheet_matrix[{row, col}] = value;
                    free(value);
                }
            }

            xlsxioread_sheet_close(sheet);
        }

        xlsxioread_close(handle);
        return result;
    }

    void write_xlsx(string_view path, const map<string, map<CellCoords, string> >& data) {
        lxw_workbook* workbook = workbook_new(path.data());
        if (!workbook) {
            throw runtime_error(fmt::format("Failed opening {} for writing!", path));
        }
        for (const auto& [sheet_name, sheet_matrix] : data) {
            auto sheet = workbook_add_worksheet(workbook, sheet_name.data());
            if (!sheet) {
                workbook_close(workbook);
                throw runtime_error(fmt::format("Failed opening sheet {}:{} for writing!", path, sheet_name));
            }
            for (const auto& [coords, cell] : sheet_matrix) {
                worksheet_write_string(sheet, coords.row, coords.col, cell.data(), NULL);
            }
        }
        workbook_close(workbook);
    }
}
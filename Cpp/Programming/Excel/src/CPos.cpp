#include "CPos.hpp"

CPos::CPos(const std::string_view &str) {
    size_t split = 0;
    m_row = 0;
    m_absoluteColumn = false;
    m_absoluteRow = false;
    if (str.size() < 2 || !isValid(str, split))
        throw std::invalid_argument("Invalid position input");
    std::string_view column = str.substr(0, split);
    std::string_view row = str.substr(split);
    if (column[0] == '$') {
        m_absoluteColumn = true;
        column.remove_prefix(1);
    }
    if (row[0] == '$') {
        m_absoluteRow = true;
        row.remove_prefix(1);
    }
    std::string editColumn(column);
    std::string editRow(row);
    std::transform(editColumn.begin(), editColumn.end(), editColumn.begin(), ::toupper);
    std::istringstream iss(editRow);
    iss >> m_row;
    m_column = 0;
    int counter = (int) column.size() - 1;
    //simple conversion from 26-base to decimal
    for (const auto &it: editColumn) {
        m_column += (it - 64) * pow(26, counter);
        --counter;
    }
}

bool CPos::isValid(const std::string_view &str, size_t &separator) const {
    bool delimiter = false;
    for (auto item = str.begin(); item != str.end(); ++item) {
        if (!delimiter) {
            if (!isalpha(*item)) {
                if (item != str.begin() && (isdigit(*item) || *item == '$')) {
                    separator = item - str.begin();
                    delimiter = true;
                } else if (*item != '$')
                    return false;
            }
        } else if (!isdigit(*item) && *item != '$')
            return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const CPos &other) {
    if (other.m_absoluteColumn) os << "$";
    os << other.convertToString();
    if (other.m_absoluteColumn) os << "$";
    os << other.m_row;
    return os;
}

bool CPos::operator<(const CPos &other) const {
    if (m_column != other.m_column)
        return m_column < other.m_column;
    return m_row < other.m_row;
}

std::string CPos::convertToString() const {
    int val = (int) m_column;
    std::string res;
    //Converting decimal number to 26-base (represented as A-Z)
    while (val > 0) {
        int remainder = (val - 1) % 26;
        res.insert(res.begin(), remainder + 'A');
        val = (val - 1) / 26;
    }
    return res;
}
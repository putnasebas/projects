#include "CValString.hpp"

std::unique_ptr<CExpression> CValString::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<CValString>(map, std::get<std::string>(m_val));
}

void CValString::changePosition(const std::pair<ssize_t, ssize_t> &) {
    //Just a dead end of the recursion when searching for Value reference inside map
    //probably bad class design :( didn't know how to make it better
}

void CValString::print(std::ostream &os) const {
    os << '"';
    for (const auto &item: std::get<std::string>(m_val)) {
        os << item;
        if (item == '"')
            os << '"';
    }
    os << '"';
}

CValue CValString::eval() const {
    return std::get<std::string>(m_val);
}

ASTnode ValStr(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, std::string val) {
    return std::make_unique<CValString>(data, std::move(val));
}
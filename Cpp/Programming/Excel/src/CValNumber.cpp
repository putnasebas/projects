#include "CValNumber.hpp"

CValue CValNumber::eval() const {
    return std::get<double>(m_val);
}

std::unique_ptr<CExpression> CValNumber::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<CValNumber>(map, std::get<double>(m_val));
}

void CValNumber::changePosition(const std::pair<ssize_t, ssize_t> &) {
    //Just a dead end of recursion when searching for Value reference inside map
    //probably bad class design :( didn't know how to make it better
}

void CValNumber::print(std::ostream &os) const {
    os << std::get<double>(m_val);
}

ASTnode ValNum(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, double val) {
    return std::make_unique<CValNumber>(data, val);
}
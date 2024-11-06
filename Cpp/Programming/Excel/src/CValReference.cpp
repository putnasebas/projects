#include "CValReference.hpp"

std::unique_ptr<CExpression>
CValReference::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<CValReference>(map, m_position);
}

CValue CValReference::eval() const {
    if (m_data.contains(m_position))
        return m_data.at(m_position).first->eval();
    return std::monostate();
}

void CValReference::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    if (!m_position.m_absoluteColumn)
        m_position.m_column += move.first;
    if (!m_position.m_absoluteRow)
        m_position.m_row += move.second;
}

void CValReference::print(std::ostream &os) const {
    os << m_position;
}

ASTnode ValRef(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, const CPos &position) {
    return std::make_unique<CValReference>(data, position);
}
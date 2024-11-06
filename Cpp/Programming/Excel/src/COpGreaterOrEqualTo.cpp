#include "COpGreaterOrEqualTo.hpp"

CValue COpGreaterOrEqualTo::eval() const {
    return m_Lhs->eval() >= m_Rhs->eval();
}

std::unique_ptr<CExpression>
COpGreaterOrEqualTo::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<COpGreaterOrEqualTo>(map, m_Lhs->clone(map), m_Rhs->clone(map));
}

void COpGreaterOrEqualTo::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    m_Lhs->changePosition(move);
    m_Rhs->changePosition(move);
}

void COpGreaterOrEqualTo::print(std::ostream &os) const {
    os << "(";
    m_Lhs->print(os);
    os << ">=";
    m_Rhs->print(os);
    os << ")";
}

ASTnode GreatEqual(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs) {
    return std::make_unique<COpGreaterOrEqualTo>(data, std::move(lhs), std::move(rhs));
}
#include "COpPow.hpp"

CValue COpPow::eval() const {
    return m_Lhs->eval() ^ m_Rhs->eval();
}

std::unique_ptr<CExpression> COpPow::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<COpPow>(map, m_Lhs->clone(map), m_Rhs->clone(map));
}

void COpPow::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    m_Rhs->changePosition(move);
    m_Lhs->changePosition(move);
}

void COpPow::print(std::ostream &os) const {
    os << "(";
    m_Lhs->print(os);
    os << "^";
    m_Rhs->print(os);
    os << ")";
}

ASTnode Pow(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs) {
    return std::make_unique<COpPow>(data, std::move(lhs), std::move(rhs));
}
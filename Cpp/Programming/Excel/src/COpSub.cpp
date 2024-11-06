#include "COpSub.hpp"

CValue COpSub::eval() const {
    return m_Lhs->eval() - m_Rhs->eval();
}

std::unique_ptr<CExpression> COpSub::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
return std::make_unique<COpSub>(map, m_Lhs->clone(map), m_Rhs->clone(map));
}

void COpSub::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    m_Rhs->changePosition(move);
    m_Lhs->changePosition(move);
}

void COpSub::print(std::ostream &os) const {
    os << "(";
    m_Lhs->print(os);
    os << "-";
    m_Rhs->print(os);
    os << ")";
}

ASTnode Sub(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs) {
    return std::make_unique<COpSub>(data, std::move(lhs), std::move(rhs));
}
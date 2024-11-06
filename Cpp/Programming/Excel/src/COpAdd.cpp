#include "COpAdd.hpp"

CValue COpAdd::eval() const {
    return m_Lhs.get()->eval() + m_Rhs->eval();
}

std::unique_ptr<CExpression> COpAdd::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
return std::make_unique<COpAdd>(map, m_Lhs->clone(map), m_Rhs->clone(map));
}

void COpAdd::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    m_Rhs->changePosition(move);
    m_Lhs->changePosition(move);
}

void COpAdd::print(std::ostream &os) const {
    os << "(";
    m_Lhs->print(os);
    os << "+";
    m_Rhs->print(os);
    os << ")";
}

ASTnode Add(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs) {
    return std::make_unique<COpAdd>(data, std::move(lhs), std::move(rhs));
}
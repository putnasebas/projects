#include "COpNegate.hpp"

CValue COpNegate::eval() const {
    return -m_val->eval();
}

std::unique_ptr<CExpression> COpNegate::clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const {
    return std::make_unique<COpNegate>(map, m_val->clone(map));
}

void COpNegate::changePosition(const std::pair<ssize_t, ssize_t> &move) {
    m_val->changePosition(move);
}

void COpNegate::print(std::ostream &os) const {
    os << "(";
    os << "-";
    m_val->print(os);
    os << ")";
}

ASTnode Neg(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode val) {
    return std::make_unique<COpNegate>(data, std::move(val));
}
#include "CSpreadsheet.hpp"

bool CSpreadsheet::load(std::istream &is) {
    std::string line;
    CBuildExpr tmp;
    while (std::getline(is, line, '~')) {
        size_t separate = line.find(';');
        if (separate != std::string::npos) {
            std::string pos = line.substr(0, separate);
            std::string val = line.substr(separate + 1);
            try {
                CPos position(pos);
                parseExpression(val, tmp);
                tmp.add(position);
            } catch (...) {
                return false;
            }
        } else
            return false;
    }
    m_expr = tmp;
    return true;
}

bool CSpreadsheet::save(std::ostream &os) const {
    const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &toPrint = m_expr.getMap();
    bool first = true;
    for (const auto &item: toPrint) {
        if (first) {
            os << item.first << ";" << *item.second.first;
            first = false;
        } else
            os << "~" << item.first << ";" << *item.second.first;
    }
    return true;
}

bool CSpreadsheet::setCell(const CPos &pos, const std::string &contents) {
    try {
        parseExpression(contents, m_expr);
    } catch (...) { return false; }
    m_expr.add(pos);
    return true;
}

CValue CSpreadsheet::getValue(const CPos &pos) {
    return m_expr.getVal(pos);
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    m_expr.copyValues(dst, src, w, h);
}

std::ostream &operator<<(std::ostream &os, const CSpreadsheet &data) {
    return os << data.m_expr;
}
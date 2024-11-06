#include "CBuildExpr.hpp"

void CBuildExpr::opAdd() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Add(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opSub() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Sub(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opMul() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Multipl(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opDiv() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Div(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opPow() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Pow(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opNeg() {
    ASTnode val = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Neg(m_data, std::move(val)));
}

void CBuildExpr::opEq() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(Equiv(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opNe() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(NotEquiv(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opLt() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(LessThan(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opLe() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(LessEqual(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opGt() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(GreatThan(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::opGe() {
    ASTnode first = m_stack.top()->clone(m_data);
    m_stack.pop();
    ASTnode second = m_stack.top()->clone(m_data);
    m_stack.pop();
    m_stack.emplace(GreatEqual(m_data, std::move(second), std::move(first)));
}

void CBuildExpr::valNumber(double val) {
    m_stack.emplace(ValNum(m_data, val));
}

void CBuildExpr::valString(std::string val) {
    m_stack.emplace(ValStr(m_data, std::move(val)));
}

CValue CBuildExpr::getVal(const CPos &position) const {
    std::map<CPos, bool> graph;
    bool valid = true;
    hasCycle(position, graph, valid);
    if (m_data.contains(position) && valid)
        return m_data.at(position).first->eval();
    return {};
}


void CBuildExpr::hasCycle(const CPos &node, std::map<CPos, bool> &graph, bool &valid) const {
    if (graph.contains(node) && graph.at(node)) {
        valid = false;
        return;
    } else if (graph.contains(node) && !graph.at(node))
        return;
    if (m_data.contains(node) && valid) {
        graph[node] = true;
        for (const auto &item: m_data.at(node).second)
            hasCycle(item, graph, valid);
        graph[node] = false;
    }
}

CBuildExpr &CBuildExpr::add(const CPos &position) {
    m_data[position].first = m_stack.top()->clone(m_data);
    m_data[position].second = std::move(m_dependency);
    m_dependency.clear();
    m_stack.pop();
    return *this;
}

void CBuildExpr::valReference(std::string val) {
    CPos pos(val);
    m_dependency.emplace_back(pos);
    m_stack.emplace(ValRef(m_data, pos));
}

CBuildExpr &CBuildExpr::operator=(const CBuildExpr &other) {
    if (this == &other) return *this;
    m_data.clear();
    for (const auto &item: other.m_data) {
        m_data[item.first].first = item.second.first->clone(m_data);
        m_data[item.first].second = item.second.second;
    }
    return *this;
}

CBuildExpr::CBuildExpr(const CBuildExpr &other) {
    for (const auto &item: other.m_data)
        m_data[item.first] = {item.second.first->clone(m_data), item.second.second};
}

std::ostream &operator<<(std::ostream &os, const CBuildExpr &data) {
    for (const auto &item: data.m_data) {
        os << "[" << item.first << "] ";
        if (std::holds_alternative<double>(item.second.first->eval()))
            os << std::get<double>(item.second.first->eval()) << " = " << *item.second.first;
        else if (std::holds_alternative<std::string>(item.second.first->eval()))
            os << std::get<std::string>(item.second.first->eval()) << " = " << *item.second.first;
        else os << *item.second.first;
        std::cout << ", data: ";
        for (const auto &item1: item.second.second)
            os << item1 << ", ";
        std::cout << std::endl;
    }
    return os;
}

const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &CBuildExpr::getMap() const { return m_data; }

void CBuildExpr::funcCall(std::string fnName, int paramCount) {
    //I don't plan to implement, just a way to silence compiler
    (void) fnName;
    (void) (paramCount);
}

void CBuildExpr::valRange(std::string val) {
    //I don't plan to implement, just a way to silence compiler
    (void) val;

}

//w = column , h = row
CBuildExpr &CBuildExpr::copyValues(const CPos &dst, const CPos &src, int w, int h) {
    std::pair<ssize_t, ssize_t> move = {dst.m_column - src.m_column, dst.m_row - src.m_row};
    std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> saveOldValues;
    //saving old values in case of rectangle collision
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (m_data.contains({src.m_column + x, src.m_row + y}))
                saveOldValues[{src.m_column + x, src.m_row + y}] = {
                        m_data[{src.m_column + x, src.m_row + y}].first->clone(m_data),
                        m_data[{src.m_column + x, src.m_row + y}].second};
        }
    }
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            std::pair<size_t, size_t> from = {src.m_column + x, src.m_row + y};
            std::pair<size_t, size_t> to = {from.first + move.first, from.second + move.second};
            if (m_data.contains({from.first, from.second})) {
                m_data[{to.first, to.second}] = {saveOldValues[{from.first, from.second}].first->clone(m_data),
                                                 std::move(saveOldValues[{from.first, from.second}].second)};
                for (auto &it: m_data.at({to.first, to.second}).second) {
                    if (!it.m_absoluteColumn)
                        it.m_column += move.first;
                    if (!it.m_absoluteRow)
                        it.m_row += move.second;
                }
                //changing relative references inside AST
                m_data[{to.first, to.second}].first->changePosition(move);
            }
        }
    }
    return *this;
}

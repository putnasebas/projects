#pragma once
#include "CPos.hpp"
#include "CValue.hpp"
#include <map>
#include <memory>

class CExpression;
using ASTnode = std::unique_ptr<CExpression>;

/**
 * Abstract class for abstract syntax tree(AST from now on) from which other classes inherit from
 * */
class CExpression {
public:
    /**
     * CExpression constructor, takes reference to value table
     * @param[in] data - const reference to value table map
     * */
    CExpression(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data)
    : m_data(data) {}

    /**
     *  Virtual destructor for abstract class
     * */
    virtual ~CExpression() = default;

    /**
     * Pure virtual class methods from which its children inherit from
     * */
    virtual std::unique_ptr<CExpression>
    clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const = 0;

    virtual CValue eval() const = 0;

    virtual void print(std::ostream &os) const = 0;

    virtual void changePosition(const std::pair<ssize_t, ssize_t> &move) = 0;

    /**
     * Operator to print AST in format fitting to save method
     * */
    friend std::ostream &operator<<(std::ostream &os, const CExpression &other) {
        os << "=";
        other.print(os);
        return os;
    }

protected:
    /**
     * constant reference to value table
     * */
    const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &m_data;

};
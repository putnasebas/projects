#pragma once
#include "CExpression.hpp"

ASTnode ValNum(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, double val);

/**
 * Class representing double value inside AST
 * */
class CValNumber : public CExpression {
public:
    /**
     * Constructor for CValNumber which inherits from CExpression class
     * @param[in] data - value for CExpression constructor
     * @param[in] val - double value which should be stored inside node
     * */
    CValNumber(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, double val)
    : CExpression(data), m_val(val) {}

    /**
     * Because there is nothing to evaluate, simply returns m_val variable
     * */
    virtual CValue eval() const override;

    /**
     * prints m_val value
     * */
    virtual void print(std::ostream &os) const override;

    /**
     * Creates a deep copy of CValNumber instance
     * @param[in] map - new map to which new instance will point to
     * @returns a new std::unique_ptr to value
     * */
    virtual std::unique_ptr<CExpression>
    clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const override;

    /**
     * Recursion to change position reference reached dead end
     * */
    virtual void changePosition(const std::pair<ssize_t, ssize_t> &move) override;

private:
    CValue m_val;
};

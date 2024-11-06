#pragma once
#include "CExpression.hpp"

ASTnode Neg(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode val);

/**
 * Class representing  unary operator - inside AST and negates original value
 * */
class COpNegate : public CExpression {
    public:
        /**
          * Constructor for COpNegate which inherits from CExpression class
          * @param[in] data - value for CExpression constructor
          * @param[in] val - value which is to be negated
          * */
        COpNegate(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode val)
        : CExpression(data), m_val(std::move(val)) {}

        /**
         * Performs unary - operation on its child
         * */
        virtual CValue eval() const override;

        /**
         * Prints - and uses recursion to print its child
         * prints '(' and ')' as well to ensure that operator priority is not lost after print
         * */
        virtual void print(std::ostream &os) const override;

        /**
         * Creates a deep copy of COpNegate instance
         * @param[in] map - new map to which new instance will point to
         * @returns a new std::unique_ptr to COpNegate
         * */
        virtual std::unique_ptr<CExpression>
        clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const override;

        /**
         * Uses recursion to deliver move parameter to CValReference if exists
         * Just a transfer method
         * */
        virtual void changePosition(const std::pair<ssize_t, ssize_t> &move) override;

    private:
        ASTnode m_val;
};
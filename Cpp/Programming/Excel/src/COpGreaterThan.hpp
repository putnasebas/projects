#pragma once
#include "CExpression.hpp"

ASTnode GreatThan(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs);

/**
 * Class representing operator > inside AST and connects its 2 children
 * */
class COpGreaterThan : public CExpression {
    public:
        /**
         * Constructor for COpLesserOrEqualTo which inherits from CExpression class
         * @param[in] data - value for CExpression constructor
         * @param[in] lhs - left operand of > operator
         * @param[in] rhs - right operand of > operator
         * */
        COpGreaterThan(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, ASTnode lhs, ASTnode rhs)
                : CExpression(data), m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs)) {}

        /**
         * Performs > operation on its 2 children
         * @returns 1 if left > right, 0 if right <= left
         *  if datatypes which are compared differs, returns std::monostate
         * */
        CValue eval() const override;

        /**
         * Prints > and uses recursion to print left node and right node
         * prints '(' and ')' as well to ensure that operator priority is not lost after print
         * */
        virtual void print(std::ostream &os) const override;

        /**
         * Creates a deep copy of COpGreaterThan instance
         * @param[in] map - new map to which new instance will point to
         * @returns a new std::unique_ptr to COpGreaterThan
         * */
        virtual std::unique_ptr<CExpression>
        clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const override;

        /**
         * Uses recursion to deliver move parameter to CValReference if exists
         * Just a transfer method
         * */
        virtual void changePosition(const std::pair<ssize_t, ssize_t> &move) override;

    private:
        ASTnode m_Lhs;
        ASTnode m_Rhs;
};

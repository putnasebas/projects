#pragma once
#include "CExpression.hpp"

ASTnode ValRef(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, const CPos &position);

/**
 * Class representing Reference to value inside AST
 * */
class CValReference : public CExpression {
    public:
        /**
         * Constructor for CValReference which inherits from CExpression class
         * @param[in] data - value for CExpression constructor
         * @param[in] pos - position to which ValReference refers to
         * */
        CValReference(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &data, const CPos &position)
        : CExpression(data), m_position(position) {}

        /**
         * Calculates value on current position
         * @returns calculated value
         * */
        virtual CValue eval() const override;

        /**
         * Prints position to which m_position refers to
         * */
        virtual void print(std::ostream &os) const override;

        /**
         * Creates a deep copy of CValReference instance
         * @param[in] map - new map to which new instance will point to
         * @returns a new std::unique_ptr to position
         * */
        virtual std::unique_ptr<CExpression>
        clone(const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &map) const override;

        /**
         * Changes m_positions row and col if not absolute for amount received from input parameter
         * @param[in] move - pair of numbers telling how to move row and column if they are not absolute
         * */
        virtual void changePosition(const std::pair<ssize_t, ssize_t> &move) override;

    private:
        CPos m_position;
};

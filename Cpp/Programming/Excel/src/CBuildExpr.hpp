#pragma once
#include <stack>
#include "CValNumber.hpp"
#include "CValString.hpp"
#include "CValReference.hpp"
#include "COpAdd.hpp"
#include "COpSub.hpp"
#include "COpMultiplication.hpp"
#include "COpDivision.hpp"
#include "COpPow.hpp"
#include "COpNegate.hpp"
#include "COpEquivalent.hpp"
#include "COpNotEquivalent.hpp"
#include "COpLesserThan.hpp"
#include "COpLesserOrEqualTo.hpp"
#include "COpGreaterThan.hpp"
#include "COpGreaterOrEqualTo.hpp"
#include "expression.h"

/**
 * Class for creating AST and managing data inside spreadsheet
 * */
class CBuildExpr : public CExprBuilder {
    public:
        CBuildExpr() = default;

        CBuildExpr(const CBuildExpr &other);

        CBuildExpr &operator=(const CBuildExpr &other);

        /**
         * Operator binary + called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opAdd() override;

        /**
         * Operator binary - called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opSub() override;

        /**
         * Operator binary * called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opMul() override;

        /**
         * Operator binary / called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opDiv() override;

        /**
         * Operator binary ^ called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opPow() override;

        /**
         * Operator unary - called by parser, removes first element from stack and returns its result back
         * */
        virtual void opNeg() override;

        /**
         * Operator binary == called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opEq() override;

        /**
         * Operator binary != called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opNe() override;

        /**
         * Operator binary < called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opLt() override;

        /**
         * Operator binary <= called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opLe() override;

        /**
         * Operator binary > called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opGt() override;

        /**
         * Operator binary >= called by parser, removes first 2 elements from stack and returns its result back
         * */
        virtual void opGe() override;

        /**
         * Adds number value to the stack, with which other operators can work with
         * */
        virtual void valNumber(double val) override;

        /**
         * Adds string value to the stack, with which other operators can work with
         * */
        virtual void valString(std::string val) override;

        /**
         * Adds reference to position from a map to the stack, with which other operators can work with
         * */
        virtual void valReference(std::string val) override;

        /**
         * Not implemented methods in my solution, just there so given parser can work properly
         * */
        virtual void valRange(std::string val) override;

        virtual void funcCall(std::string fnName, int paramCount) override;

        /**
         * Calculates the value of the AST on given position if not empty
         * @param[in] position - position on the map which result is to be calculated
         * @returns result of AST if exists on the given position, otherwise default value of CValue
         * */
        CValue getVal(const CPos &position) const;

        /**
         * Checks whether given node has cycle and can be calculated without danger of ending up in cycle
         * Implemented as DFS algorithm
         * @param[in] node - node, which is currently being investigated by DFS algorithm
         * @param[out] graph - map containing status of discovered nodes during DFS algorithm
         *  false = node is open, true = node is already closed
         * @param[out] valid - information for class if node has cyclic dependency or not and can be calculated
         *  true if evaluation is possible otherwise false
         * */
        void hasCycle(const CPos &node, std::map<CPos, bool> &graph, bool &valid) const;

        /**
         * Assigns newly created AST to the given position
         * param[in] position - position, to which result AST should be assigned
         * */
        CBuildExpr &add(const CPos &position);

        /**
         * @param[in] dst - left top corner of destination rectangle
         * @param[in] src - ;eft top corner of source rectangle
         * param[in] w - amount of columns in rectangle
         * param[in] h - amount of rows in rectangle
         * */
        CBuildExpr &copyValues(const CPos &dst, const CPos &src, int w, int h);

        friend std::ostream &operator<<(std::ostream &os, const CBuildExpr &data);

        /**
         * Just a simple getter, nothing interesting
         * */
        const std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> &getMap() const;


    private:
        /**
         * stack which helps to create AST
         * */
        std::stack<ASTnode> m_stack;
        /**
         * vector of dependencies of current node
         * */
        std::vector<CPos> m_dependency;
        /**
         * map storing entire spreadsheet
         * */
        std::map<CPos, std::pair<ASTnode, std::vector<CPos>>> m_data;

};
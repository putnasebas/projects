#pragma once
#include <functional>
#include <iostream>
#include <utility>
#include <cmath>
#include <variant>

using CValue = std::variant<std::monostate, double, std::string>;

/**
 * Binary operator for CValue (std::variant) implementation
 * */
CValue operator+(const CValue &lhs, const CValue &rhs);

CValue operator-(const CValue &lhs, const CValue &rhs);

CValue operator*(const CValue &lhs, const CValue &rhs);

CValue operator/(const CValue &lhs, const CValue &rhs);

CValue operator^(const CValue &lhs, const CValue &rhs);

CValue operator-(const CValue &val);

CValue operator==(const CValue &lhs, const CValue &rhs);

CValue operator!=(const CValue &lhs, const CValue &rhs);

CValue operator<(const CValue &lhs, const CValue &rhs);

CValue operator<=(const CValue &lhs, const CValue &rhs);

CValue operator>(const CValue &lhs, const CValue &rhs);

CValue operator>=(const CValue &lhs, const CValue &rhs);
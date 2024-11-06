#include "CValue.hpp"

CValue operator+(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
        return std::get<double>(lhs) + std::get<double>(rhs);

    else if (std::holds_alternative<double>(lhs) && std::holds_alternative<std::string>(rhs))
        return std::to_string(std::get<double>(lhs)) + std::get<std::string>(rhs);

    else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<double>(rhs))
        return std::get<std::string>(lhs) + std::to_string(std::get<double>(rhs));

    else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
        return std::get<std::string>(lhs) + std::get<std::string>(rhs);

    return std::monostate();
}

CValue operator-(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
        return std::get<double>(lhs) - std::get<double>(rhs);
    return std::monostate();
}

CValue operator*(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
        return std::get<double>(lhs) * std::get<double>(rhs);
    return std::monostate();
}

CValue operator/(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs) && std::get<double>(rhs) != 0)
        return std::get<double>(lhs) / std::get<double>(rhs);
    return std::monostate();
}

CValue operator^(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
        return pow(std::get<double>(lhs), std::get<double>(rhs));
    return std::monostate();
}

CValue operator-(const CValue &val) {
    if (std::holds_alternative<double>(val))
        return std::get<double>(val) * -1;
    return std::monostate();
}

CValue operator==(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) == std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) == std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();

}

CValue operator<(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) < std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) < std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();
}

CValue operator<=(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) <= std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) <= std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();
}

CValue operator>(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) > std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) > std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();
}

CValue operator>=(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) >= std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) >= std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();
}

CValue operator!=(const CValue &lhs, const CValue &rhs) {
    if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
        if (std::get<double>(lhs) != std::get<double>(rhs))
            return 1.0;
        return 0.0;
    } else if ((std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))) {
        if (std::get<std::string>(lhs) != std::get<std::string>(rhs))
            return 1.0;
        return 0.0;
    }
    return std::monostate();
}
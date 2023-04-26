#include "bigint_base_arithmetic.h"

namespace ninteger {

    bigint_base_arithmetic::bigint_base_arithmetic(
            int sign,
            std::vector<unsigned int> & parts
    ) noexcept {
        _sign = sign;
        _parts = std::move(parts);
    }

    std::string & bigint_base_arithmetic::satiate_with_zeros(
            std::string && number
            ) {
        int zeros_to_add = 9 - static_cast<int>(number.size());
        for (int i = 0; i < zeros_to_add; ++i) {
            number.insert(0, "0");
        }
        return number;
    }

    bool bigint_base_arithmetic::parse_long_long(
            long long number,
            std::vector<unsigned int> & parts
            ) noexcept {
        number = (number > 0) ? number : -number;
        if (number <= INT_MAX) {
            return true;
        }
        unsigned int mod = 1e9;
        while (number > 0) {
            parts.push_back(number % mod);
            number /= mod;
        }
        return false;
    }

    bool bigint_base_arithmetic::parse_string(
            const std::string & number,
            std::vector<unsigned int> & parts
            ) {
        long long i = number[0] == '-';
        while (number[i] == '0') {
            ++i;
        }
        if (number.size() - i < 19) {
            return true;
        }
        long long j = static_cast<long long>(number.size()) - 9;
        for (; j >= i; j -= 9) {
            try {
                parts.push_back(std::stoll(number.substr(j, 9)));
            } catch (std::invalid_argument &) {
                throw std::runtime_error("Cannot perform converting to bigint_base_arithmetic, because invalid number passed!");
            }
        }
        j += 9;
        if (j > i) {
            try {
                parts.push_back(std::stoll(number.substr(i, j - i)));
            } catch (std::invalid_argument &) {
                throw std::runtime_error("Cannot perform converting to bigint_base_arithmetic, because invalid number passed!");
            }
        }
        return false;
    }

    bigint_base_arithmetic::bigint_base_arithmetic() noexcept {
        _sign = 0;
    }

    bigint_base_arithmetic::bigint_base_arithmetic(
            int number
            ) noexcept {
        _sign = number;
    }

    bigint_base_arithmetic::bigint_base_arithmetic(
            long long number
            ) noexcept {
        _sign = (number > 0) ? 1 : -1;
        if (parse_long_long(number, _parts)) {
            _sign = static_cast<int>(number);
        }
    }

    bigint_base_arithmetic::bigint_base_arithmetic(
            std::string const & number
            ) {
        _sign = (number[0] == '-') ? -1 : 1;
        if (parse_string(number, _parts)) {
            try {
                long long number_as_long_long = std::stoll(number);
                if (parse_long_long(number_as_long_long, _parts)) {
                    _sign = static_cast<int>(number_as_long_long);
                }
            } catch (std::invalid_argument &) {
                throw std::runtime_error("Cannot perform converting to bigint_base_arithmetic, because invalid number passed!");
            }
        }
    }

    bigint_base_arithmetic::bigint_base_arithmetic(
            bigint_base_arithmetic const & number
            ) {
        _sign = number._sign;
        for (unsigned int i = 0; i < number._parts.size(); ++i) {
            _parts.push_back(number._parts[i]);
        }
    }

    bigint_base_arithmetic & bigint_base_arithmetic::operator=(
            bigint_base_arithmetic && number
            ) noexcept {
        if (this == &number) {
            return *this;
        }
        _parts.clear();
        _sign = number._sign;
        _parts = std::move(number._parts); // или хранить указатель на вектор и его перебрасывать
        return *this;
    }

    bigint_base_arithmetic bigint_base_arithmetic::to_bigint(
            int number
            ) {
        return bigint_base_arithmetic(number);
    }

    bigint_base_arithmetic bigint_base_arithmetic::to_bigint(
            long long number
            ) {
        int sign = (number > 0) ? 1 : -1;
        std::vector<unsigned int> parts;
        if (parse_long_long(number, parts)) {
            return to_bigint(static_cast<int>(number));
        }
        return bigint_base_arithmetic(sign, parts);
    }

    bigint_base_arithmetic bigint_base_arithmetic::to_bigint(
            std::string const & number
            ) {
        int sign = (number[0] == '-') ? -1 : 1;
        std::vector<unsigned int> parts;
        if (parse_string(number, parts)) {
            try {
                long long number_as_long_long = std::stoll(number);
                if (parse_long_long(number_as_long_long, parts)) {
                    return to_bigint(static_cast<int>(number_as_long_long));
                }
                return bigint_base_arithmetic(sign, parts);
            } catch (std::invalid_argument &) {
                throw std::runtime_error("Cannot perform converting to bigint_base_arithmetic, because invalid number passed!");
            }
        }
        return bigint_base_arithmetic(sign, parts);
    }

    std::string bigint_base_arithmetic::to_string() const {
        if (_parts.empty()) {
            return std::to_string(_sign);
        }
        std::string number_as_string = (_sign == -1) ? "-" : "";
        unsigned long long i = _parts.size() - 1;
        while (true) {
            number_as_string += (i != _parts.size() - 1) ? satiate_with_zeros(std::to_string(_parts[i])) : std::to_string(_parts[i]);
            if (i == 0) {
                break;
            }
            --i;
        }
        return number_as_string;
    }

    bool bigint_base_arithmetic::equals(
            bigint_base_arithmetic const & other
            ) const {
        if (_sign != other._sign) {
            return false;
        }
        if (_parts.size() != other._parts.size()) {
            return false;
        }
        for (long long i = 0; i < _parts.size(); ++i) {
            if (_parts[i] != other._parts[i]) {
                return false;
            }
        }
        return true;
    }

    bool bigint_base_arithmetic::operator==(
            bigint_base_arithmetic const & other
            ) const {
        return equals(other);
    }

    bool bigint_base_arithmetic::not_equals(
            bigint_base_arithmetic const & other
            ) const {
        if (_sign != other._sign) {
            return true;
        }
        if (_parts.size() != other._parts.size()) {
            return true;
        }
        for (long long i = 0; i < _parts.size(); ++i) {
            if (_parts[i] != other._parts[i]) {
                return true;
            }
        }
        return false;
    }

    bool bigint_base_arithmetic::operator!=(
            bigint_base_arithmetic const & other
            ) const {
        return not_equals(other);
    }

    bool bigint_base_arithmetic::lower_than(
            bigint_base_arithmetic const & other
            ) const {
        if (_parts.empty() && other._parts.empty()) {
            return _sign < other._sign;
        }
        if (_parts.empty()) {
            return other._sign == 1;
        }
        if (other._parts.empty()) {
            return _sign == -1;
        }
        if (_parts.size() < other._parts.size()) {
            return other._sign == 1;
        }
        if (_parts.size() > other._parts.size()) {
            return _sign == -1;
        }
        for (long long i = static_cast<long long>(_parts.size()) - 1; i >= 0; --i) {
            if (_parts[i] == other._parts[i]) {
                continue;
            }
            return _parts[i] < other._parts[i];
        }
        return _sign < other._sign;
    }

    bool bigint_base_arithmetic::operator<(
            bigint_base_arithmetic const & other
            ) const {
        return lower_than(other);
    }

    bool bigint_base_arithmetic::greater_than(
            bigint_base_arithmetic const & other
            ) const {
        return other.lower_than(const_cast<bigint_base_arithmetic &>(*this));
    }

    bool bigint_base_arithmetic::operator>(
            bigint_base_arithmetic const & other
            ) const {
        return greater_than(other);
    }

    bool bigint_base_arithmetic::lower_than_or_equal_to(
            bigint_base_arithmetic const & other
            ) const {
        return !greater_than(other);
    }

    bool bigint_base_arithmetic::operator<=(
            bigint_base_arithmetic const & other
            ) const {
        return lower_than_or_equal_to(other);
    }

    bool bigint_base_arithmetic::greater_than_or_equal_to(
            bigint_base_arithmetic const & other
            ) const {
        return !lower_than(other);
    }

    bool bigint_base_arithmetic::operator>=(
            bigint_base_arithmetic const & other
            ) const {
        return greater_than_or_equal_to(other);
    }

    bool bigint_base_arithmetic::fits_in_integer(
            long long amount
            ) {
        return amount <= INT_MAX && amount >= INT_MIN;
    }

    bool bigint_base_arithmetic::attempt_simple_calculation(
            bigint_base_arithmetic & lhs,
            const bigint_base_arithmetic & rhs,
            bool sum
            ) {
        if (!lhs._parts.empty() || !rhs._parts.empty()) {
            return false;
        }
        long long intermediate_res = lhs._sign + (sum ? 1 : -1) * rhs._sign;
        if (fits_in_integer(intermediate_res)) {
            lhs._sign = static_cast<int>(intermediate_res);
        } else {
            lhs._sign = (intermediate_res < 0) ? -1 : 1;
            parse_long_long(intermediate_res, lhs._parts);
        }
        return true;
    }

    bigint_base_arithmetic & bigint_base_arithmetic::multiply_by_minus_one(
            bigint_base_arithmetic & number
            ) {
        if (number._sign == INT_MIN) {
            number._parts.push_back(static_cast<unsigned int>(INT_MAX + 1ll));
            number._sign = 1;
        } else if (number._parts.size() == 1 && number._parts[0] == INT_MAX + 1ll) {
            number._sign = INT_MIN;
            number._parts.pop_back();
        } else {
            number._sign = -number._sign;
        }
        return number;
    }

    bigint_base_arithmetic bigint_base_arithmetic::unary_minus() const {
        bigint_base_arithmetic opposite_number(*this);
        if (opposite_number._sign == INT_MIN) {
            opposite_number._parts.push_back(static_cast<unsigned int>(INT_MAX + 1ll));
            opposite_number._sign = 1;
        } else if (opposite_number._parts.size() == 1 && opposite_number._parts[0] == INT_MAX + 1ll) {
            opposite_number._sign = INT_MIN;
            opposite_number._parts.pop_back();
        } else {
            opposite_number._sign = -opposite_number._sign;
        }
        return opposite_number;
    }

    bigint_base_arithmetic bigint_base_arithmetic::operator-() const {
        return unary_minus();
    }

    bigint_base_arithmetic bigint_base_arithmetic::abs(
            bigint_base_arithmetic const & number
            ) {
        bigint_base_arithmetic abs_number(number);
        if (number._sign == INT_MIN) {
            abs_number._parts.push_back(static_cast<unsigned int>(INT_MAX + 1ll));
            abs_number._sign = 1;
        } else {
            abs_number._sign = std::abs(number._sign);
        }
        return abs_number;
    }

    bigint_base_arithmetic & bigint_base_arithmetic::add(
            bigint_base_arithmetic const & summand
            ) {
        if (attempt_simple_calculation(*this, summand)) {
            return *this;
        }

        // If we have numbers both positive and negative number
        // Then in fact we have to subtract elements
        if (_sign < 0 ^ summand._sign < 0) {
//                return summand._sign < 0 ? *this -= -summand : multiply_by_minus_one(*this), multiply_by_minus_one(*this -= summand); // ????
            if (summand._sign < 0) {
                return *this -= -summand;
            } else {
                multiply_by_minus_one(*this);
                return *this = std::move(multiply_by_minus_one(*this -= summand)); // ????????????
            }
        }

        // Suppose we have 2 positive or 2 negative numbers:
        if (_parts.empty()) {
            _parts.push_back(std::abs(_sign));
        }
        unsigned int to_size = std::max(_parts.size(), summand._parts.size());
        unsigned int carry = summand._parts.empty() ? std::abs(summand._sign) : 0;
        for (unsigned int i = 0; i < to_size || carry; ++i) {
            if (_parts.size() == i) {
                _parts.push_back(0);
            }
            long long intermediate_sum = _parts[i] + static_cast<long long>((i < summand._parts.size() ? summand._parts[i] : 0)) + carry;
            if (intermediate_sum >= base) {
                carry = 0; // ??????????????????????????????
                while (intermediate_sum >= base) {
                    intermediate_sum -= base;
                    carry += 1;
                }
//                    intermediate_sum -= base;
//                    carry = 1;
            } else {
                carry = 0;
            }
            _parts[i] = intermediate_sum;
        }
        if (_parts.size() == 1 && fits_in_integer(_sign)) {
            _sign = static_cast<int>((_sign < 0 ? -1 : 1) * _parts[0]);
            _parts.pop_back();
        } else {
            _sign = _sign < 0 ? -1 : 1;
        }
        return *this;
    }

    bigint_base_arithmetic & bigint_base_arithmetic::operator+=(
            bigint_base_arithmetic const & summand
            ) {
        return add(summand);
    }

    bigint_base_arithmetic bigint_base_arithmetic::sum(
            bigint_base_arithmetic const & summand
            ) {
        bigint_base_arithmetic sum(*this);
        sum += summand;
        return sum;
    }

    bigint_base_arithmetic bigint_base_arithmetic::operator+(
            bigint_base_arithmetic const & summand
            ) {
        return sum(summand);
    }

    bigint_base_arithmetic &bigint_base_arithmetic::subtract(
            bigint_base_arithmetic const & subtrahend
            ) {
        if (attempt_simple_calculation(*this, subtrahend, false)) {
            return *this;
        }

        // If we have 1 positive and 1 negative number
        // Then subtraction is in fact a sum
        if (_sign < 0 ^ subtrahend._sign < 0) {
//                return subtrahend._sign < 0 ? *this += -subtrahend : multiply_by_minus_one(*this), multiply_by_minus_one(*this += subtrahend); // ????
            if (subtrahend._sign < 0) {
                return *this += -subtrahend;
            } else {
                multiply_by_minus_one(*this);
                return *this = std::move(multiply_by_minus_one(*this += subtrahend));
            }
        }

        // > 0 && > 0
        // < 0 && < 0

        // This operation is tricky as we need a > b condition to be true to continue
        // If thats not true we will have to create an extra copy :'(
        if (abs(*this) < abs(subtrahend)) { // we need by module
            bigint_base_arithmetic subtraction_result = subtrahend - *this;
            return *this = std::move(multiply_by_minus_one(subtraction_result));
        }

        if (_parts.empty()) {
            _parts.push_back(std::abs(_sign));
        }
        // посмотреть, что будет, если занимаем из нуля
        unsigned int carry = subtrahend._parts.empty() ? std::abs(subtrahend._sign) : 0; // Overflow friendly
        for (unsigned int i = 0; i < _parts.size() || carry; ++i) {
            long long intermediate_sub = _parts[i] - static_cast<long long>((i < subtrahend._parts.size() ? subtrahend._parts[i] : 0)) - carry;
            if (intermediate_sub < 0) {
                // посмотреть нужно ли то же самое прикрутить для сложения
                carry = 0;
                while (intermediate_sub < 0) {
                    intermediate_sub += base;
                    carry += 1;
                }
            } else {
                carry = 0;
            }
            _parts[i] = intermediate_sub;
        }
        while (_parts.back() == 0) {
            _parts.pop_back();
        }
        if (_parts.size() == 1 && fits_in_integer(_parts[0])) {
            _sign = static_cast<int>((_sign < 0 ? -1 : 1) * _parts[0]);
            _parts.pop_back();
        } else {
            _sign = _sign < 0 ? -1 : 1;
        }
        return *this;
    }

    bigint_base_arithmetic &bigint_base_arithmetic::operator-=(
            bigint_base_arithmetic const & subtrahend
            ) {
        return subtract(subtrahend);
    }

    bigint_base_arithmetic bigint_base_arithmetic::subtraction(
            bigint_base_arithmetic const & subtrahend
            ) const {
        bigint_base_arithmetic result(*this);
        result -= subtrahend;
        return result;
    }

    bigint_base_arithmetic bigint_base_arithmetic::operator-(
            bigint_base_arithmetic const & subtrahend
            ) const {
        return subtraction(subtrahend);
    }

    std::ostream & operator<<(
            std::ostream & os,
            ninteger::bigint_base_arithmetic const & number
            ) {
        return os << number.to_string();
    }

    std::istream & operator>>(
            std::istream & in,
            ninteger::bigint_base_arithmetic & number
    ) {
        std::string string_number;
        in >> string_number;
        // Будет ли здесь вызван конструктор перемещения для объекта вектора (решено; добавлен свой конструктор перемещения)
        number = std::move(ninteger::bigint_base_arithmetic(string_number));
        return in;
    }

}

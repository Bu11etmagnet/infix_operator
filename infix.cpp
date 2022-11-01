#include <cstddef>
#include <type_traits>
#include <utility> // swap

#include <string>
std::string operator*(std::string const& str, std::size_t multiplier)
{
    std::string retval;
    retval.reserve(str.size() * multiplier);
    for (std::size_t i = 0; i < multiplier; ++i) {
        retval += str;
    }
    return retval;
}

namespace detail {

template<
    typename Left,
    typename Op,
    typename = typename std::decay<Op>::type::binary_operator_tag
>
struct bound_operator
{
    Left && left;
    Op && op;
};

} // detail

template<typename Left, typename Op>
constexpr detail::bound_operator<Left, Op>
operator<(Left && left, Op && op)
{
    return { std::forward<Left>(left), std::forward<Op>(op) };
}

template<typename Left, typename Op, typename Right>
constexpr auto
operator>(detail::bound_operator<Left, Op> && ref, Right && right)
    -> decltype(ref.op(std::forward<Left>(ref.left), std::forward<Right>(right)))
{
    return      ref.op(std::forward<Left>(ref.left), std::forward<Right>(right));
}

namespace infix {

constexpr struct swap_operator {
    typedef void binary_operator_tag;

    template<typename L, typename R>
    void operator()(L &lhs, R& rhs) const
    {
        using std::swap;
        swap(lhs, rhs);
    }
} swap = swap_operator{};

constexpr struct add_op
{
    typedef void binary_operator_tag;

    template<typename L, typename R>
    auto constexpr operator()(L const& lhs, R const& rhs) const -> decltype(lhs + rhs)
    {
        return lhs + rhs;
    }
} plus = add_op{};

constexpr struct sub_op
{
    typedef void binary_operator_tag;

    template<typename L, typename R>
    auto constexpr operator()(L const& lhs, R const& rhs) const -> decltype(lhs - rhs)
    {
        return lhs - rhs;
    }
} minus = sub_op{};

constexpr struct mul_op
{
    typedef void binary_operator_tag;

    template<typename L, typename R>
    auto constexpr operator()(L const& lhs, R const& rhs) const -> decltype(lhs * rhs)
    {
        return lhs * rhs;
    }
} times = mul_op{};

constexpr struct div_op
{
    typedef void binary_operator_tag;

    template<typename L, typename R>
    auto constexpr operator()(L const& lhs, R const& rhs) const -> decltype(lhs / rhs)
    {
        return lhs / rhs;
    }
} divided_by = div_op{};

}
using namespace infix;

#include <iostream>

int main()
{
    int a = 'A';
    int b = 'b';
    std::cout << "Before: a=" << a << ", b=" << b << "\n";

    a <swap> b;

    std::cout << "After : a=" << a << ", b=" << b << "\n";

    auto constexpr forty_two = 6 <times> 7;
    auto constexpr eris = 10 <plus> 3;

    static_assert(forty_two == 42, "The answer is wrong");
    static_assert(eris == 13, "You displease Eris");

    std::cout << "6 * 7 = " << forty_two << "\n";
    std::cout << "Hail Eris " << eris << "\n";

    auto constexpr answer2 = 100 <divided_by> 2 <minus> 10 <plus> 2;
    static_assert(answer2 == 42, "The answer is wrong");

    auto constexpr one23 = 1 <plus> 2 <times> 3;
    static_assert(one23 == 9, "Left-to-right evaluation");
    auto constexpr use_parens  = 1 <plus> (2 <times> 3);
    static_assert(use_parens == 7, "You have to use parens");

    std::string meow{"meow"};
    std::string multi_meow = meow * 2;
    std::cout << "Cat says " << multi_meow << "\n";
    multi_meow = meow <times> 3;
    std::cout << "Cat says " << multi_meow << "\n";
}

#include <cstddef>
#include <type_traits>
#include <utility> // swap

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

    template<typename T>
    void operator()(T &a, T& b) const
    {
        using std::swap;
        swap(a, b);
    }
} swap = swap_operator{};

constexpr struct add_op
{
    typedef void binary_operator_tag;

    template<typename T>
    auto constexpr operator()(T const& a, T const& b) const -> decltype(a + b)
    {
        return a + b;
    }
} plus = add_op{};

constexpr struct sub_op
{
    typedef void binary_operator_tag;

    template<typename T>
    auto constexpr operator()(T const& a, T const& b) const -> decltype(a - b)
    {
        return a - b;
    }
} minus = sub_op{};

constexpr struct mul_op
{
    typedef void binary_operator_tag;

    template<typename T>
    auto constexpr operator()(T const& a, T const& b) const -> decltype(a * b)
    {
        return a * b;
    }
} times = mul_op{};

constexpr struct div_op
{
    typedef void binary_operator_tag;

    template<typename T>
    auto constexpr operator()(T const& a, T const& b) const -> decltype(a / b)
    {
        return a / b;
    }
} divided_by = div_op{};


}
using namespace infix;

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

#include <iostream>

int main()
{
    int a = 'A';
    int b = 'b';
    std::cout << "Before: a=" << a << ", b=" << b << "\n";

    a <swap> b;

    std::cout << "After : a=" << a << ", b=" << b << "\n";

    auto constexpr fourty_two = 6 <times> 7;
    auto constexpr eris = 10 <plus> 3;

    static_assert(fourty_two == 42);
    static_assert(eris == 13, "You displease Eris");

    std::cout << "6 * 7 = " << fourty_two << "\n";
    std::cout << "Hail Eris " << eris << "\n";

    std::string meow{"meow"};
    std::string multimeow = meow * 2;
    std::cout << "Cat says " << multimeow << "\n";
    multimeow = meow <times> 3;
}

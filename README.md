# What?

Some crazy code I copied from the Internet.
Unfortunately, I forgot to note the source.

# What is happening?

For `a <swap> b`, main calls

        operator< <int&, infix::swap_operator const&> with 'A' and infix::mul_op{}
        operator><int&, infix::swap_operator const&, int&> with detail::bound_operator<int&, infix::swap_operator const&, void>{ 'A', infix::swap_operator } and 'b'
          infix::swap_operator::operator()<int, int>('A', 'b')

For `meow <times> 3`, main calls

        operator< (std::string&, infix::mul_op const&) with "meow" and infix::mul_op{}
        operator><std::string&, infix::mul_op const&, int> (ref={"meow", const infix::mul_op}, right=3)
          infix::mul_op::operator()<std::string, int> (lhs="meow", rhs=3)
# What's all this then?

Here's the AST dumped by clang for a function with a single infix operation:

        constexpr
        auto multiplier()
        {
            return 6 <times> 7;
        }


        |-FunctionDecl 0x2b7d9c0 <infix.cpp:108:1, line:112:1> line:109:6 used constexpr multiplier 'int ()'
        | `-CompoundStmt 0x2b845d0 <line:110:1, line:112:1>
        |   `-ReturnStmt 0x2b845c0 <line:111:5, col:22>
        |     `-ExprWithCleanups 0x2b845a8 <col:12, col:22> 'decltype(ref.op(std::forward<int>(ref.left), std::forward<int>(right)))':'int'
        |       `-CXXOperatorCallExpr 0x2b84490 <col:12, col:22> 'decltype(ref.op(std::forward<int>(ref.left), std::forward<int>(right)))':'int' '>'
        |         |-ImplicitCastExpr 0x2b84478 <col:20> 'auto (*)(detail::bound_operator<int, const mul_op &> &&, int &&) -> decltype(ref.op(std::forward<int>(ref.left), std::forward<int>(right)))' <FunctionToPointerDecay>
        |         | `-DeclRefExpr 0x2b836e0 <col:20> 'auto (detail::bound_operator<int, const mul_op &> &&, int &&) -> decltype(ref.op(std::forward<int>(ref.left), std::forward<int>(right)))' lvalue Function 0x2b83578 'operator>' 'auto (detail::bound_operator<int, const mul_op &> &&, int &&) -> decltype(ref.op(std::forward<int>(ref.left), std::forward<int>(right)))'
        |         |-MaterializeTemporaryExpr 0x2b836b0 <col:12, col:15> 'detail::bound_operator<int, const mul_op &>':'detail::bound_operator<int, const infix::mul_op &, void>' xvalue
        |         | `-CXXOperatorCallExpr 0x2b81d30 <col:12, col:15> 'detail::bound_operator<int, const mul_op &>':'detail::bound_operator<int, const infix::mul_op &, void>' '<'
        |         |   |-ImplicitCastExpr 0x2b81d18 <col:14> 'detail::bound_operator<int, const mul_op &> (*)(int &&, const infix::mul_op &)' <FunctionToPointerDecay>
        |         |   | `-DeclRefExpr 0x2b80228 <col:14> 'detail::bound_operator<int, const mul_op &> (int &&, const infix::mul_op &)' lvalue Function 0x2b800f0 'operator<' 'detail::bound_operator<int, const mul_op &> (int &&, const infix::mul_op &)'
        |         |   |-MaterializeTemporaryExpr 0x2b80210 <col:12> 'int' xvalue
        |         |   | `-IntegerLiteral 0x2b7da60 <col:12> 'int' 6
        |         |   `-DeclRefExpr 0x2b7da80 <col:15> 'const struct mul_op':'const infix::mul_op' lvalue Var 0x28f6f38 'times' 'const struct mul_op':'const infix::mul_op'
        |         `-MaterializeTemporaryExpr 0x2b836c8 <col:22> 'int' xvalue
        |           `-IntegerLiteral 0x2b81e50 <col:22> 'int' 7


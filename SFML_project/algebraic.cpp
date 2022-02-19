#include "algebraic.h"

expr operator+(expr left, expr right)
{
    return expr(left.ptr->add(right.ptr));
}

expr operator-(expr left, expr right)
{
    return expr(left.ptr->substract(right.ptr));
}

expr operator*(expr left, expr right)
{
    return expr(left.ptr->multiply(right.ptr));
}

expr operator*(expr _expr, constTy scalar)
{
    return expr(_expr.ptr->multiply(scalar));
}

expr operator/(expr left, expr right)
{
    return expr(left.ptr->divide(right.ptr));
}

const bool expr::operator==(const expr &other) const
{
    return (*this - other).expand().checkZeroEquality();
}

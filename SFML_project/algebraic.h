#pragma once
#include "calculator.h"

using namespace calc;

class expr
{
public:
    expr() {};
    expr(const expr_ptr &_ptr)
        : ptr(_ptr)
    {
    }
    expr(const expr_ptr &&_ptr)
        : ptr(_ptr)
    {
    }

    expr conj() const { return expr(ptr->conj()); }

    friend expr operator+(expr left, expr right);
    friend expr operator-(expr left, expr right);
    friend expr operator*(expr left, expr right);
    friend expr operator*(expr expr, constTy scalar);
    friend expr operator/(expr left, expr right);

    expr operator-() const { return expr(ptr->negate()); }

    expr expand() const { return expr(ptr->expand()); }

    const bool operator==(const expr &other) const;

    const bool checkZeroEquality() const { return ptr->checkZeroEquality(); }

    void print() const
    {
        ptr->print();
    }

private:
    expr_ptr ptr;
};
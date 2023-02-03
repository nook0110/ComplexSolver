#pragma once
#include "calculator.h"

using namespace calc;

class expr
{
public:
  expr() {};

  expr(const expr_ptr& _ptr)
    : ptr(_ptr)
  {}
  expr(const expr_ptr&& _ptr)
    : ptr(_ptr)
  {}

  expr conj() const { return expr(ptr->conj()); }
  expr get_quasi(const std::string& new_name)
  {
    poly_ptr quasi = make_quasi_term(new_name, ptr);
    return expr(quasi);
  }
  expr get_quasi_known_conj(const std::string& new_name, expr conj)
  {
    poly_ptr quasi = make_quasi_pair(new_name, ptr, conj.ptr);
    return expr(quasi);
  }

  friend expr operator+(expr left, expr right);
  friend expr operator-(expr left, expr right);
  friend expr operator*(expr left, expr right);
  friend expr operator*(expr expr, constTy scalar);
  friend expr operator/(expr left, expr right);

  expr operator-() const { return expr(ptr->negate()); }

  expr expand() const { return expr(ptr->expand()); }

  const bool operator==(const expr& other) const;

  const bool checkZeroEquality() const { return ptr->checkZeroEquality(); }

  void print() const
  {
    ptr->print();
  }

  const std::string getTEXformat() const { return ptr->getTEXformat(); }

private:
  expr_ptr ptr;

  //static printer out;
};
#pragma once

// * needed structures
#include <string>
#include <list>
#include <stack>
#include <complex>
#include <algorithm>

// * debugging
#include <iostream>

namespace calc
{
	using constTy = std::complex<int>;

	class expressionNode;
	class operationNode;
	class polyNode;

	using expr_ptr = std::shared_ptr<expressionNode>;
	using op_ptr = std::shared_ptr<operationNode>;
	using poly_ptr = std::shared_ptr<polyNode>;

	// main abstract class describing rational complex-valued function
	// represents operation tree whose leaves are multivariable polynomials
	// all contructors of subclasses are private, use create() to create a shared_ptr on a new object
	class expressionNode : public std::enable_shared_from_this<expressionNode>
	{
	public:
		virtual expr_ptr conj() const = 0;

		// double dispatching of binary operations
		virtual expr_ptr add(expr_ptr &second) = 0;
		virtual expr_ptr add(poly_ptr &secondPoly) = 0;
		virtual expr_ptr add(op_ptr &secondOp) = 0;

		virtual expr_ptr multiply(constTy coef) = 0;
		expr_ptr negate() { return multiply(-1); }
		expr_ptr substract(expr_ptr &second)
		{
			expr_ptr neg_second = second->negate();
			return add(neg_second);
		}

		virtual expr_ptr multiply(expr_ptr &second) = 0;
		virtual expr_ptr multiply(poly_ptr &secondPoly) = 0;
		virtual expr_ptr multiply(op_ptr &secondOp) = 0;

		virtual expr_ptr divide(expr_ptr &second, const bool isDivident = true) = 0;
		virtual expr_ptr divide(poly_ptr &secondPoly, const bool isDivident = true) = 0;
		virtual expr_ptr divide(op_ptr &secondOp, const bool isDivident = true) = 0;

		// double dispatching of bracket expansion
		virtual expr_ptr expandAddition(expr_ptr &second) = 0;
		virtual expr_ptr expandAddition(poly_ptr &secondPoly) = 0;
		virtual expr_ptr expandAddition(op_ptr &secondOp) = 0;
		virtual expr_ptr expandMultiplication(expr_ptr &second) = 0;
		virtual expr_ptr expandMultiplication(poly_ptr &secondPoly) = 0;
		virtual expr_ptr expandMultiplication(op_ptr &secondOp) = 0;

		virtual expr_ptr expand() = 0;

		// printing data, utility predicates
		virtual const bool checkZeroEquality() const = 0; // TODO: add optional printing // TODO: make pure virtual

		virtual void print() const = 0;
		virtual const bool requiresBracketsPrinting() const = 0;

	protected:
		template <class derived_node> std::shared_ptr<derived_node> shared_from_base()
		{
			// returns shared pointer to an object of one of derived classes
			return std::static_pointer_cast<derived_node>(shared_from_this());
		}
	};

	class operationNode : public expressionNode // a sum, a product, or a fraction of two expressions
	{
	public:
		enum class operationType
		{
			ADDITION,
			MULTIPLICATION,
			DIVISION
		};

	private:
		operationType operation;
		expr_ptr left;
		expr_ptr right;

		operationNode(expr_ptr &_left, expr_ptr &_second, operationType _operation)
			: left(_left), right(_second), operation(_operation)
		{
		}

		operationNode(const operationNode &other)
			: left(other.left), right(other.right), operation(other.operation)
		{
		}

	public:
		static op_ptr create(expr_ptr _left, expr_ptr _second, operationType _operation)
		{
			return op_ptr(new operationNode(_left, _second, _operation));
		}

		expr_ptr conj() const override;

		virtual expr_ptr add(expr_ptr &second) override;
		virtual expr_ptr add(poly_ptr &second) override;
		virtual expr_ptr add(op_ptr &second) override;

		virtual expr_ptr multiply(constTy coef) override;

		virtual expr_ptr multiply(expr_ptr &second) override
		{
			op_ptr shared_from_this_op = shared_from_base<operationNode>();
			return second->multiply(shared_from_this_op);
		}
		virtual expr_ptr multiply(poly_ptr &secondPoly) override;
		virtual expr_ptr multiply(op_ptr &secondOp) override;

		virtual expr_ptr divide(expr_ptr &second, const bool isDivident = true) override
		{
			op_ptr shared_from_this_op = shared_from_base<operationNode>();
			return second->divide(shared_from_this_op, !isDivident);
		};
		virtual expr_ptr divide(poly_ptr &secondPoly, const bool isDivident = true) override;
		virtual expr_ptr divide(op_ptr &secondOp, const bool isDivident = true) override;

		virtual expr_ptr expandAddition(expr_ptr &second) override
		{
			op_ptr shared_from_this_op = shared_from_base<operationNode>();
			return second->expandAddition(shared_from_this_op);
		}
		virtual expr_ptr expandAddition(poly_ptr &secondPoly) override;
		virtual expr_ptr expandAddition(op_ptr &secondOp) override;

		virtual expr_ptr expandMultiplication(expr_ptr &second) override
		{
			op_ptr shared_from_this_op = shared_from_base<operationNode>();
			return second->expandMultiplication(shared_from_this_op);
		}
		virtual expr_ptr expandMultiplication(poly_ptr &secondPoly) override;
		virtual expr_ptr expandMultiplication(op_ptr &secondOp) override;

		virtual expr_ptr expand() override;

		virtual const bool checkZeroEquality() const { return false; }

		virtual void print() const override;
		virtual const bool requiresBracketsPrinting() const;
	};

	class term
	{
	public:
		term() {}
		term(const std::string _name)
			: name(_name) {}

		bool hasConjugationMark = false;
		void invertMark() { hasConjugationMark = !hasConjugationMark; }
		const std::string name;
		const bool operator<(const term &other) const;
		const bool operator==(const term &other) const;

		virtual expr_ptr conj() const = 0;
	};

	class basicTerm : public term
	{
	public:
		expr_ptr conj() const;

		basicTerm(const std::string _name)
			: term(_name) {}

		struct termProperties
		{
			bool isReal;
			bool isUnit;
		};

		basicTerm(const std::string _name, termProperties _props)
			: term(_name), props(_props) {}

	private:
		const termProperties props = { false, false };
	};

	class quasiTerm : public term
	{
	public:
		expr_ptr conj() const;
		quasiTerm(const std::string _name, expr_ptr &_hiddenExpression)
			: term(_name), hiddenExpression(_hiddenExpression)
		{
		}

	private:
		expr_ptr hiddenExpression;
	};

	struct monomial
	{
		constTy coef = 1; // changing value of coef doesn't invalidate the order of monomials

		std::list<std::pair<term *, int>> product;

		monomial() {}
		monomial(const constTy _coef)
			: coef(_coef)
		{
		}
		monomial(constTy _coef, term *_term)
			: coef(_coef)
		{
			product.emplace_back(_term, 1);
		}

		const bool operator<(const monomial &other) const;

		const bool operator==(const monomial &other) const;

		expr_ptr conj() const;
		monomial operator*(const monomial &other) const;
		const monomial& operator*=(const constTy k);

		const bool dividedBy(const monomial &divider) const;
		monomial divide(const monomial &divider) const;
	};

	class polyNode : public expressionNode
	{
	private:
		polyNode() {}
		polyNode(const monomial &mono)
		{
			sum.push_back(mono);
		}
		polyNode(const polyNode &other)
			: sum(other.sum)
		{
		}

	public:
		static poly_ptr create()
		{
			return poly_ptr(new polyNode()); // returns zero expression
		}
		static poly_ptr create(const monomial &mono)
		{
			return poly_ptr(new polyNode(mono));
		}
		static poly_ptr create(const polyNode &other)
		{
			return poly_ptr(new polyNode(other));
		}

		expr_ptr conj() const override;

		polyNode& operator+=(const polyNode &other);
		polyNode operator+(const polyNode &other) const;
		polyNode operator*(const monomial &mono) const;
		polyNode operator*(const polyNode &other) const;

		polyNode operator*(const constTy k) const;

		polyNode operator-(const polyNode &other) const;

		virtual expr_ptr add(expr_ptr &second) override 
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return second->add(shared_from_this_poly); 
		}
		virtual expr_ptr add(poly_ptr &secondPoly) override { return polyNode::create(*this + *secondPoly); }
		virtual expr_ptr add(op_ptr &secondOp) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return secondOp->add(shared_from_this_poly);
		}

		virtual expr_ptr multiply(constTy coef) override { return polyNode::create(*this * coef); }

		virtual expr_ptr multiply(expr_ptr &second) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return second->multiply(shared_from_this_poly);
		}
		virtual expr_ptr multiply(poly_ptr &secondPoly) override
		{
			return polyNode::create((*this) * (*secondPoly));
		}
		virtual expr_ptr multiply(op_ptr &secondOp) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return secondOp->multiply(shared_from_this_poly);
		}

		virtual expr_ptr divide(expr_ptr &second, const bool isDivident = true) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return second->divide(shared_from_this_poly, !isDivident);
		}
		virtual expr_ptr divide(poly_ptr &secondPoly, const bool isDivident = true) override;
		virtual expr_ptr divide(op_ptr &secondOp, const bool isDivident = true) override;

		virtual expr_ptr expandAddition(expr_ptr &second) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return second->expandAddition(shared_from_this_poly);
		}
		virtual expr_ptr expandAddition(poly_ptr &secondPoly) override
		{
			return add(secondPoly);
		}
		virtual expr_ptr expandAddition(op_ptr &secondOp) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return secondOp->expandAddition(shared_from_this_poly);
		}

		virtual expr_ptr expandMultiplication(expr_ptr &second) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return second->expandMultiplication(shared_from_this_poly);
		}
		virtual expr_ptr expandMultiplication(poly_ptr &secondPoly) override
		{
			return multiply(secondPoly);
		}
		virtual expr_ptr expandMultiplication(op_ptr &secondOp) override
		{
			poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
			return secondOp->expandMultiplication(shared_from_this_poly);
		}

		virtual expr_ptr expand() override { return shared_from_base<polyNode>(); }

		virtual void print() const;

	private:
		std::list<monomial> sum; // sorted list of unique monomials

	public:
		const bool dividedBy(const monomial &divider) const;

		expr_ptr divide(const monomial &divider) const;

		virtual const bool requiresBracketsPrinting() const { return sum.size() > 1; }
		const bool checkZeroEquality() const { return sum.empty(); }
	};

	poly_ptr make_term(std::string name);
	poly_ptr make_unit_term(std::string name);
	poly_ptr make_real_term(std::string name);
	poly_ptr make_scalar(constTy scalar);
}
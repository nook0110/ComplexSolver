#include "calculator.h"

using namespace calc;

expr_ptr operationNode::conj()
{
	expr_ptr conj_left = left->conj();
	expr_ptr conj_right = right->conj();
	if (operation == operationType::ADDITION)
	{
		return conj_left->add(conj_right);
	}
	if (operation == operationType::MULTIPLICATION)
	{
		return conj_left->multiply(conj_right);
	}
	if (operation == operationType::DIVISION)
	{
		return conj_left->divide(conj_right);
	}
}

expr_ptr operationNode::add(expr_ptr& second)
{
	op_ptr shared_from_this_op = shared_from_base<operationNode>();
	return second->add(shared_from_this_op);
}

expr_ptr operationNode::add(poly_ptr& secondPoly)
{
	if (secondPoly->checkZeroEquality())
		return shared_from_base<operationNode>();
	if (operation != operationType::ADDITION)
		return create(secondPoly, shared_from_base<operationNode>(), operationType::ADDITION);
	return (secondPoly->add(left))->add(right);
}

expr_ptr operationNode::add(op_ptr& secondOp)
{
	if (operation == operationType::ADDITION)
	{
		if (secondOp->operation == operationType::ADDITION)
		{
			return (add(secondOp->left))->add(secondOp->right);
		}
		return create(shared_from_base<operationNode>(), secondOp, operationType::ADDITION);
	}
	if (secondOp->operation == operationType::ADDITION)
	{
		return create(secondOp, shared_from_base<operationNode>(), operationType::ADDITION);
	}
	if (operation == operationType::DIVISION && secondOp->operation == operationType::DIVISION)
	{
		expr_ptr diff = right->substract(secondOp->right);
		if (diff->checkZeroEquality())
		{
			expr_ptr nom = left->add(secondOp->left);
			return nom->divide(right);
		}
	}
	return create(shared_from_base<operationNode>(), secondOp, operationType::ADDITION);
}

expr_ptr operationNode::multiply(constTy coef)
{
	expr_ptr left_scaled = left->multiply(coef);
	if (operation == operationType::ADDITION)
	{
		expr_ptr right_scaled = right->multiply(coef);
		return left_scaled->add(right_scaled);
	}
	if (operation == operationType::MULTIPLICATION)
		return left->multiply(coef)->multiply(right);
	if (operation == operationType::DIVISION)
		return left->multiply(coef)->divide(right);
}

expr_ptr operationNode::multiply(poly_ptr& secondPoly)
{
	if (secondPoly->checkZeroEquality())
		return polyNode::create();
	if (operation == operationType::MULTIPLICATION)
		return (secondPoly->multiply(left))->multiply(right);
	if (operation == operationType::DIVISION)
		return (secondPoly->multiply(left))->divide(right);
	return create(secondPoly, shared_from_base<operationNode>(), operationType::MULTIPLICATION);
}

expr_ptr operationNode::multiply(op_ptr& secondOp)
{
	if (operation == operationType::DIVISION)
	{
		if (secondOp->operation == operationType::DIVISION)
		{
			expr_ptr nominator = left->multiply(secondOp->left);
			expr_ptr denominator = right->multiply(secondOp->right);
			return nominator->divide(denominator);
		}
		expr_ptr nominator = left->multiply(secondOp);
		return nominator->divide(right);
	}
	if (secondOp->operation == operationType::DIVISION)
	{
		expr_ptr nominator = multiply(secondOp->left);
		return nominator->divide(secondOp->right);
	}
	if (operation == operationType::MULTIPLICATION)
	{
		if (secondOp->operation == operationType::MULTIPLICATION)
		{
			return (multiply(secondOp->left))->multiply(secondOp->right);
		}
		return operationNode::create(shared_from_base<operationNode>(), secondOp, operationType::MULTIPLICATION);
	}
	if (secondOp->operation == operationType::MULTIPLICATION)
	{
		return create(secondOp, shared_from_base<operationNode>(), operationType::MULTIPLICATION);
	}
	return create(shared_from_base<operationNode>(), secondOp, operationType::MULTIPLICATION);
}

expr_ptr operationNode::divide(poly_ptr& secondPoly, const bool isDivident)
{
	if (isDivident)
	{
		if (operation == operationType::DIVISION)
		{
			expr_ptr denominator = right->multiply(secondPoly);
			return left->divide(denominator);
		}
		return create(shared_from_base<operationNode>(), secondPoly, operationType::DIVISION);
	}
	else
	{
		if (secondPoly->checkZeroEquality())
			return polyNode::create();
		if (operation == operationType::DIVISION)
		{
			expr_ptr nominator = secondPoly->multiply(right);
			return nominator->divide(left);
		}
		else
		{
			return create(secondPoly, shared_from_base<operationNode>(), operationType::DIVISION);
		}
	}
}

expr_ptr operationNode::divide(op_ptr& secondOp, const bool isDivident)
{
	if (isDivident)
	{
		if (operation == operationType::DIVISION)
		{
			if (secondOp->operation == operationType::DIVISION)
			{
				expr_ptr nominator = left->multiply(secondOp->right);
				expr_ptr denominator = right->multiply(secondOp->left);
				return nominator->divide(denominator);
			}
			expr_ptr denominator = right->multiply(secondOp);
			return left->divide(denominator);
		}
		if (secondOp->operation == operationType::DIVISION)
		{
			expr_ptr nominator = multiply(secondOp->right);
			return nominator->divide(secondOp->left);
		}
		return create(shared_from_base<operationNode>(), secondOp, operationType::DIVISION);
	}
	else
	{
		op_ptr shared_from_this_op = shared_from_base<operationNode>();
		return secondOp->divide(shared_from_this_op);
	}
}

expr_ptr operationNode::expandAddition(poly_ptr& secondPoly)
{
	if (operation != operationType::DIVISION)
		return add(secondPoly);
	expr_ptr right_times_secondPoly = right->multiply(secondPoly);
	return (left->add(right_times_secondPoly))->divide(right);
}

expr_ptr operationNode::expandAddition(op_ptr& secondOp)
{
	if (operation != operationType::DIVISION)
	{
		if (secondOp->operation != operationType::DIVISION)
			return add(secondOp);
		op_ptr shared_from_this_op = shared_from_base<operationNode>();
		return secondOp->expandAddition(shared_from_this_op);
	}
	if (secondOp->operation != operationType::DIVISION)
	{
		expr_ptr right_times_secondOp = right->multiply(secondOp);
		return (left->add(right_times_secondOp))->divide(right);
	}
	expr_ptr left_part = left->multiply(secondOp->right);
	expr_ptr right_part = right->multiply(secondOp->left);
	expr_ptr nominator = (left_part)->add(right_part);
	expr_ptr denominator = right->multiply(secondOp->right);
	return nominator->divide(denominator);
}

expr_ptr operationNode::expandMultiplication(poly_ptr& secondPoly)
{
	if (operation != operationType::ADDITION)
	{
		return multiply(secondPoly);
	}
	expr_ptr right_times_secondPoly = right->multiply(secondPoly);
	return (left->multiply(secondPoly))->add(right_times_secondPoly);
}

expr_ptr operationNode::expandMultiplication(op_ptr& secondOp)
{
	if (operation != operationType::ADDITION)
	{
		return multiply(secondOp);
	}
	expr_ptr right_times_secondOp = right->multiply(secondOp);
	return (left->multiply(secondOp))->add(right_times_secondOp);
}

expr_ptr operationNode::expand()
{
	left = left->expand();
	right = right->expand();
	if (operation == operationType::ADDITION)
	{
		return left->expandAddition(right);
	}
	if (operation == operationType::MULTIPLICATION)
	{
		return left->expandMultiplication(right);
	}
	if (operation == operationType::DIVISION)
	{
		return left->divide(right);
	}
}

const bool operationNode::requiresBracketsPrinting() const
{
	return !(operation == operationType::MULTIPLICATION);
}

const bool term::operator<(const term& other) const
{
	if (name != other.name)
		return name < other.name;
	if (hasConjugationMark != other.hasConjugationMark)
		return !hasConjugationMark;
	return false;
}

const bool term::operator==(const term& other) const
{
	return (name == other.name) && (hasConjugationMark == other.hasConjugationMark);
}

expr_ptr basicTerm::conj()
{
	if (props.isReal)
		return polyNode::create(monomial(1, this));
	if (props.isUnit)
	{
		poly_ptr poly_of_this = polyNode::create(monomial(1, this));
		return make_scalar(1)->divide(poly_of_this);
	}
	basicTerm* conjugated = new basicTerm(*this);
	conjugated->invertMark();
	return polyNode::create(monomial(1, conjugated));
}

expr_ptr quasiTerm::conj()
{
	return polyNode::create(monomial(1, conjugated));
}

expr_ptr calc::quasiTerm::getExpr()
{
	if (expandedForm)
		return hiddenExpression;
	else
	{
		if (hiddenExpression == nullptr)
			hiddenExpression = conjugated->hiddenExpression->conj();
		expandedForm = true;
		return (hiddenExpression = hiddenExpression->expand());
	}
}

expr_ptr calc::monomial::subExpand() const
{
	expr_ptr result = make_scalar(coef);
	for (auto& [term, degree] : product)
	{
		expr_ptr hidden_expr = term->getExpr();
		for (int i = 0; i < degree; ++i)
		{
			result = result->multiply(hidden_expr);
		}
	}
	return result;
}

expr_ptr monomial::conj() const
{
	expr_ptr result = make_scalar(std::conj(coef));
	for (auto const& [term, degree] : product)
	{
		expr_ptr conj_term = term->conj();
		for (int i = 0; i < degree; ++i)
			result = result->multiply(conj_term);
	}
	return result;
}

const bool monomial::operator<(const monomial& other) const
{
	auto jt = other.product.begin();
	for (auto it = product.begin(); it != product.end(); ++it)
	{
		if (jt == other.product.end())
			return false;
		if (!(*it->first == *jt->first))
			return *it->first < *jt->first;
		if (it->second < jt->second)
			return (it == std::prev(product.end()));
		if (jt->second < it->second)
			return (jt != std::prev(other.product.end()));
		++jt;
	}
	if (jt != other.product.end())
		return true;
	return false;
}

const bool monomial::operator==(const monomial& other) const
{
	return std::equal(product.begin(), product.end(), other.product.begin(), other.product.end(),
		[](const auto& a, const auto& b)
		{ return (*a.first == *b.first) && (a.second == b.second); });
}

monomial monomial::operator*(const monomial& other) const
{
	monomial result = *this;
	result.coef *= other.coef;
	if (other.product.empty())
		return result;
	auto it = result.product.begin();
	for (const auto& [term, degree] : other.product)
	{
		while (it != result.product.end() && *it->first < *term)
			++it;
		if (it != result.product.end() && *it->first == *term)
		{
			it->second += degree;
			++it;
		}
		else
			result.product.emplace(it, term, degree);
	}
	return result;
}

const monomial& monomial::operator*=(const constTy k)
{
	coef = coef * k;
	return (*this);
}

const bool monomial::dividedBy(const monomial& divider) const
{
	if (divider.product.empty())
		return true;
	auto it = product.begin();
	for (auto const& [term, degree] : divider.product)
	{
		while (it != product.end() && *it->first < *term)
			++it;
		if (it == product.end())
			return false;
		if (!(*it->first == *term) || it->second < degree)
			return false;
		++it;
	}
	return true;
}

monomial monomial::divide(const monomial& divider) const
{
	if (divider.product.empty())
		return *this;
	monomial result = *this;
	auto it = result.product.begin();
	for (const auto& [term, degree] : divider.product)
	{
		while (it != result.product.end() && *it->first < *term)
			++it;
		if (it->second != degree)
		{
			it->second -= degree;
			++it;
		}
		else
			result.product.erase(it++);
	}
	return result;
}

const std::string calc::monomial::getTEXformat() const
{
	std::string result("");
	if (coef != 1 && coef != -1)
	{
		if (coef.imag() != 0)
			result += "\\left(";
		result += std::to_string(coef.real());
		if (coef.imag() != 0)
			result += "+" + std::to_string(coef.imag()) + "i" + "\\right)";
	}
	else
	{
		result += (coef == 1 ? "" : "-");
		if (product.empty())
			result += "1";
	}
	for (const auto &deg : product)
	{
		bool conjugationMark = deg.first->hasConjugationMark;
		std::string name_index;
		if (deg.first->name.size() == 1)
			name_index = deg.first->name;
		else
		{
			name_index = deg.first->name.substr(0, 1) + "_{" + deg.first->name.substr(1) + "}";
		}
		result += (conjugationMark? "\\overline{" : "") + name_index + (deg.second == 1 ? "" : "^" + std::to_string(deg.second)) + (conjugationMark ? "}" : "");
	}
	return result;
}

expr_ptr polyNode::conj()
{
	expr_ptr result = create();
	for (monomial mono : sum)
	{
		expr_ptr conj_mono = mono.conj();
		result = result->add(conj_mono);
	}
	return result;
}

polyNode& polyNode::operator+=(const polyNode& other)
{
	if (other.sum.empty())
		return (*this);
	auto it = sum.begin();
	for (const auto& mono : other.sum)
	{
		if (mono.coef == 0)
			continue;
		while (it != sum.end() && (*it) < mono)
			++it;
		if (it != sum.end() && (*it) == mono)
		{
			if (it->coef != -mono.coef)
			{
				it->coef += mono.coef;
				++it;
			}
			else
				sum.erase(it++);
		}
		else
			sum.insert(it, mono);
	}
	return (*this);
}

polyNode polyNode::operator+(const polyNode& other) const
{
	polyNode result = *this;
	result += other;
	return result;
}

polyNode polyNode::operator*(const monomial& other) const
{
	polyNode result;
	for (const auto& mono : sum)
		result.sum.emplace_back(mono * other);
	return result;
}

polyNode polyNode::operator*(const polyNode& other) const
{
	polyNode result;
	for (const auto& mono : other.sum)
		result += (*this * mono);
	return result;
}

polyNode polyNode::operator*(const constTy k) const
{
	polyNode result(*this);
	for (auto& mono : result.sum)
		mono *= k;
	return result;
}

polyNode polyNode::operator-(const polyNode& other) const
{
	return (*this) + other * (-1);
}

expr_ptr polyNode::divide(poly_ptr& secondPoly, const bool isDivident)
{
	if (isDivident)
	{
		if (sum.empty())
			return create();
		if (secondPoly->sum.size() == 1)
		{
			monomial divider = *secondPoly->sum.begin();
			if (!divider.product.empty() && dividedBy(divider))
				return divide(divider);
			if (divider.product.empty() && (divider.coef == 1))
				return shared_from_base<polyNode>();
			if (divider.product.empty() && (divider.coef == -1))
				return negate();
		}
		if (sum.size() == 1)
		{
			monomial divider = *sum.begin();
			if (divider.product.size() && secondPoly->dividedBy(divider))
			{
				expr_ptr secondPoly_divided = secondPoly->divide(divider);
				return make_scalar(1)->divide(secondPoly_divided);
			}
		}
		return operationNode::create(shared_from_base<polyNode>(), secondPoly, operationNode::operationType::DIVISION);
	}
	else
	{
		poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
		return secondPoly->divide(shared_from_this_poly);
	}
}

expr_ptr polyNode::divide(op_ptr& secondOp, const bool isDivident)
{
	poly_ptr shared_from_this_poly = shared_from_base<polyNode>();
	return secondOp->divide(shared_from_this_poly, !isDivident);
}

const bool polyNode::dividedBy(const monomial& divider) const
{
	for (const auto &mono : sum)
	{
		if (!mono.dividedBy(divider))
			return false;
	}
	return true;
}

expr_ptr polyNode::divide(const monomial& divider) const
{
	expr_ptr result = create();
	for (auto mono : sum)
	{
		expr_ptr mono_divided = create(mono.divide(divider));
		result = result->add(mono_divided);
	}
	if (divider.coef != 1 && divider.coef != -1)
	{
		poly_ptr coef = make_scalar(divider.coef);
		return result->divide(coef);
	}
	else if (divider.coef == -1)
		return result->negate();
	else
		return result;
}

expr_ptr calc::polyNode::expand()
{
	expr_ptr result = create();
	for (auto mono : sum)
	{
		expr_ptr expanded_mono = mono.subExpand();
		result = result->expandAddition(expanded_mono);
	}
	return result;
}

poly_ptr calc::make_term(std::string name)
{
	return polyNode::create(monomial(1, new basicTerm(name)));
}

poly_ptr calc::make_unit_term(std::string name)
{
	return polyNode::create(monomial(1, new basicTerm(name, { false, true })));
}

poly_ptr calc::make_real_term(std::string name)
{
	return polyNode::create(monomial(1, new basicTerm(name, { true, false })));
}

poly_ptr calc::make_quasi_term(std::string name, expr_ptr& expr)
{
	return polyNode::create(monomial(1, new quasiTerm(name, expr)));
}

poly_ptr calc::make_scalar(constTy scalar)
{
	if (scalar == 0)
		return polyNode::create();
	return polyNode::create(monomial(scalar));
}

void operationNode::print() const
{
	std::string opChar;
	switch (operation)
	{
	case (operationType::ADDITION):
		opChar = " + ";
		break;
	case (operationType::MULTIPLICATION):
		opChar = "";
		break;
	case (operationType::DIVISION):
		opChar = " / ";
		break;
	}
	const bool printLeftBrackets = left->requiresBracketsPrinting();
	if (opChar != " + " && printLeftBrackets)
		std::cout << "(";
	left->print();
	if (opChar != " + " && printLeftBrackets)
		std::cout << ")";
	std::cout << opChar;
	const bool printRightBrackets = right->requiresBracketsPrinting();
	if (opChar != " + " && printRightBrackets)
		std::cout << "(";
	right->print();
	if (opChar != " + " && printRightBrackets)
		std::cout << ")";
}

void polyNode::print() const
{
	if (sum.empty())
	{
		std::cout << 0;
		return;
	}
	for (auto it = sum.begin(); it != sum.end(); ++it)
	{
		auto mono = *it;
		constTy coef = mono.coef;
		if (coef.imag() != 0)
		{
			if (it != sum.begin())
				std::cout << " + ";
			std::cout << "(";
			std::cout << coef.real();
			std::cout << " + ";
			std::cout << coef.imag();
			std::cout << "i)";
		}
		else
		{
			if (coef.real() > 0)
			{
				if (it != sum.begin())
					std::cout << " + ";
			}
			else
			{
				if (it != sum.begin())
					std::cout << " ";
				std::cout << "- ";
			}
			if ((mono.product.empty()) || (coef.real() != 1) && (coef.real() != -1))
				std::cout << std::abs(coef.real());
		}
		for (auto [term, degree] : mono.product)
			std::cout << term->name << (term->hasConjugationMark? "~" : "") << (degree > 1 ? "^" + std::to_string(degree) : "");
	}
}

const std::string operationNode::getTEXformat() const
{
	bool left_br = left->requiresBracketsPrinting();
	bool right_br = right->requiresBracketsPrinting();
	switch (operation)
	{
	case operationType::ADDITION:
		return left->getTEXformat() + "+" + right->getTEXformat();
	case operationType::MULTIPLICATION:
		return (left_br ? "\\left(" : "") + left->getTEXformat() + (left_br ? "\\right)" : "") + (right_br ? "\\left(" : "") + right->getTEXformat() + (right_br ? "\\right)" : "");
	case operationType::DIVISION:
		return "\\frac{" + left->getTEXformat() + "}{" + right->getTEXformat() + "}";
	}
}

const std::string polyNode::getTEXformat() const
{
	if (sum.empty())
		return "0";
	std::string result("");
	for (auto it = sum.begin(); it != sum.end(); ++it)
	{
		result += it->getTEXformat();
		if (it != prev(sum.end()))
			result += (next(it)->coef.real() < 0 && next(it)->coef.imag() == 0 ? "" : "+");
	}
	return result;
}

expr_ptr calc::basicTerm::getExpr()
{
	return polyNode::create(monomial(1, this));
}
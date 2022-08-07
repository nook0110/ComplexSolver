#include "algebraic.h"

// *

expr middlepoint(expr A, expr B)
{
	return (A + B) / (expr(make_scalar(2)));
}

expr harmonic(expr A, expr B)
{
	return (A * B * 2) / (A + B);
}

expr barycenter(expr A, expr B, expr C)
{
	return (A + B + C) / (expr(make_scalar(3)));
}

expr sum(expr &A, expr &B, expr &C)
{
	return A + B + C;
}

expr linear_comb(expr A, expr kA, expr B, expr kB)
{
	return A * kA + B * kB;
}

// param A - center
expr symm(expr A, expr B)
{
	return linear_comb(A, expr(make_scalar(2)), B, expr(make_scalar(-1)));
}

expr rotate90(expr A, expr B)
{
	return linear_comb(A, expr(make_scalar(constTy(1, -1))), B, expr(make_scalar(constTy(0, 1))));
}

expr rot_homothety(expr A, expr B, expr coef)
{
	return linear_comb(A, expr(make_scalar(1)) - coef, B, coef);
}

/*
	|X       Y		|
	|conj(X) conj(Y)|
*/
expr det(expr X, expr Y)
{
	return X * Y.conj() - Y * X.conj();
}

// AB - chord
expr projection_on_a_chord(expr A, expr B, expr M)
{
	return (A + B + M - A * B * M.conj()) / expr(make_scalar(2));
}

bool collinear(expr A, expr B, expr C)
{
	expr detABC = det(A, B) + det(B, C) + det(C, A);
	detABC.print();
	std::cout << std::endl;
	return detABC.expand().checkZeroEquality();
}

bool perpendicular(expr A, expr B, expr C, expr D)
{
	expr fraction = (A - B) / (C - D);
	return (fraction + fraction.conj()).expand().checkZeroEquality();
}

bool equal_length(expr A, expr B, expr C, expr D)
{
	expr AB = A - B;
	expr CD = C - D;
	return ((AB * AB.conj()) - (CD * CD.conj())).expand().checkZeroEquality();
}

struct line
{
	expr A, B, C;
};

line chord(expr X, expr Y)
{
	return {expr(make_scalar(1)), X * Y, X + Y};
}

line tangent(expr X)
{
	return {expr(make_scalar(1)), X * X, X * 2};
}

line by_two(expr X, expr Y)
{
	expr A = X.conj() - Y.conj();
	expr B = Y - X;
	expr C = det(Y, X);
	return {A, B, C};
}

line perpendicular_line(expr P, expr N, expr M)
{
	expr A = N.conj() - M.conj();
	expr B = N - M;
	expr C = P * A + P.conj() * B;
	return {A, B, C};
}

expr intersect(line l1, line l2)
{
	expr nom = l1.C * l2.B - l1.B * l2.C;
	expr denom = l1.A * l2.B - l1.B * l2.A;
	return nom / denom;
}

bool concurrent(line l1, line l2, line l3)
{
	expr det1 = l2.B * l3.C - l2.C * l3.B;
	expr det2 = l2.C * l3.A - l2.A * l3.C;
	expr det3 = l2.A * l3.B - l2.B * l3.A;
	expr det123 = l1.A * det1 + l1.B * det2 + l1.C * det3;
	det123.print();
	std::cout << std::endl;
	return det123.expand().checkZeroEquality();
}

int main()
{
	// TESTS
	/*
	// Butterfly lemma
	expr A(make_unit_term("a"));
	expr B(make_unit_term("b"));
	expr C(make_unit_term("c"));
	expr D(make_unit_term("d"));
	expr J(intersect(chord(A, C), chord(B, D)));
	expr O(make_scalar(0)); // unit circle center
	line l = perpendicular_line(J, J, O);
	expr N = intersect(l, chord(A, D));
	expr M = intersect(l, chord(B, C));
	std::cout << ((N + M) == (J * 2)) << std::endl;
	*/
	/*
	expr A(make_unit_term("a"));
	expr B(make_unit_term("b"));
	expr C(make_unit_term("c"));
	expr K = projection_on_a_chord(B, B, sum(A, B, C));
	expr L = middlepoint(A, C);
	std::cout << equal_length(B, L, K, L) << std::endl;
	*/
	/*
	expr A(make_unit_term("a"));
	expr B(make_unit_term("b"));
	expr C(make_unit_term("c"));
	expr P(make_unit_term("p"));
	expr X = symm(projection_on_a_chord(A, B, P), P);
	expr Y = symm(projection_on_a_chord(B, C, P), P);
	expr Z = symm(projection_on_a_chord(C, A, P), P);
	std::cout << collinear(X, Y, sum(A, B, C)) << std::endl;
	std::cout << collinear(X, Y, Z) << std::endl;
	*/
	/*
	// Newton's theorem
	expr A(make_unit_term("a"));
	expr B(make_unit_term("b"));
	expr C(make_unit_term("c"));
	expr D(make_unit_term("d"));
	expr K = middlepoint(harmonic(A, B), harmonic(C, D));
	expr L = middlepoint(harmonic(A, D), harmonic(B, C));
	std::cout << collinear(K, L, expr(make_scalar(0)));
	*/
	/*
	expr E = expr(make_unit_term("e"));
	expr F = expr(make_unit_term("f"));
	expr D = expr(make_unit_term("d"));
	expr B = harmonic(F, D);
	expr C = harmonic(E, D);
	expr G = intersect(by_two(B, E), by_two(C, F));
	expr U = symm(B, F);
	expr V = symm(C, E);
	std::cout << perpendicular(U, V, G, middlepoint(E, F)) << std::endl;
	*/
	/*
	auto a = make_unit_term("a");
	auto b = make_unit_term("b");
	auto c = make_unit_term("c");
	line A = chord(a->multiply(a), b->multiply(c)->negate());
	line B = chord(b->multiply(b), c->multiply(a)->negate());
	line C = chord(c->multiply(c), a->multiply(b)->negate());
	std::cout << concurrent(A, B, C) << std::endl;
	*/
	/*
	expr a(make_unit_term("a"));
	expr b(make_unit_term("b"));
	expr c(make_unit_term("c"));
	line A = tangent(a);
	line B = tangent(b);
	line C = tangent(c);
	auto a1 = intersect(B, C);
	auto b1 = intersect(A, C);
	auto c1 = intersect(A, B);
	std::cout << concurrent(by_two(a1, a), by_two(b1, b), by_two(c1, c)) << std::endl;
	*/
	// brianchon partial case
	/*
	auto a = expr(make_unit_term("a"));
	auto b = expr(make_unit_term("b"));
	auto c = expr(make_unit_term("c"));
	auto d = expr(make_unit_term("d"));
	line A = tangent(a);
	line B = tangent(b);
	line C = tangent(c);
	line D = tangent(d);
	auto X = intersect(A, B);
	auto Y = intersect(B, C);
	auto Z = intersect(C, D);
	auto T = intersect(D, A);
	std::cout << concurrent(by_two(X, Z), by_two(Y, T), chord(a, c));
	*/
	/*
	auto a = expr(make_unit_term("a"));
	auto b = expr(make_unit_term("b"));
	auto c = expr(make_unit_term("c"));
	auto d = expr(make_unit_term("d"));
	auto e = expr(make_unit_term("e"));
	auto f = expr(make_unit_term("f"));
	line AE = chord(a, e);
	line CE = chord(c, e);
	line BD = chord(b, d);
	line BF = chord(b, f);
	line AD = chord(a, d);
	line CF = chord(c, f);
	expr first = intersect(AE, BF);
	expr second = intersect(CE, BD);
	expr third = intersect(CF, AD);
	std::cout << collinear(first, second, third) << std::endl;
	*/
	/*

	*/
	/*
	auto a = make_unit_term("a");
	auto b = make_unit_term("b");
	auto c = make_unit_term("c");
	line AHa = by_two(a, b->multiply(c)->divide(a)->negate());
	line BHb = by_two(b, c->multiply(a)->divide(b)->negate());
	line CHc = by_two(c, a->multiply(b)->divide(c)->negate());
	std::cout << concurrent(AHa, BHb, CHc) << std::endl;
	*/
	/*
	expr a(make_term("a"));
	expr b(make_term("b"));
	expr c(make_term("c"));
	line AMa = by_two(a, middlepoint(b, c));
	line BMb = by_two(b, middlepoint(a, c));
	line CMc = by_two(c, middlepoint(a, b));
	std::cout << concurrent(AMa, BMb, CMc) << std::endl;
	*/
	expr a(make_unit_term("a"));
	expr b(make_unit_term("b"));
	expr c(make_unit_term("c"));
	expr d(make_unit_term("d"));
	expr e(make_unit_term("e"));
	expr f(make_unit_term("f"));
	line first = by_two(harmonic(a, b), harmonic(d, e));
	line second = by_two(harmonic(b, c), harmonic(e, f));
	line third = by_two(harmonic(c, d), harmonic(f, a));
	std::cout << concurrent(first, second, third) << std::endl;
	/*
	std::string s;
	std::cin >> s;
	auto res = parseString(s, 0, s.size());
	res->expand()->print();
	*/
	return 0;
}
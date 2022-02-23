#include "Prover.h"

expr determinant(expr& A1, expr& B1, expr& C1, expr& A2, expr& B2, expr& C2, expr& A3, expr& B3, expr& C3)
{
	/*
		A1 B1 C1
		A2 B2 C2
		A3 B3 C3
	*/
	expr detA = B2 * C3 - C2 * B3;
	expr detB = C2 * A3 - A2 * C3;
	expr detC = A2 * B3 - B2 * A3;
	return A1 * detA + B1 * detB + C1 * detC;
}

bool proveCollinearity(expr A, expr B, expr C)
{
	expr A_conj = A.conj();
	expr B_conj = B.conj();
	expr C_conj = C.conj();
	/* matrix
		|A A~ 1|
		|B B~ 1| = 0?
		|C C~ 1|
	*/
	expr detA = B * C_conj - B_conj * C;
	expr detB = C * A_conj - C_conj * A;
	expr detC = A * B_conj - A_conj * B;
	expr det = detA + detB + detC;
	det.print();
	std::cout << std::endl;
	return det.expand().checkZeroEquality();
}

bool proveConcurrency(expr& A1, expr& B1, expr& C1, expr& A2, expr& B2, expr& C2, expr& A3, expr& B3, expr& C3)
{
	expr det = determinant(A1, B1, C1, A2, B2, C2, A3, B3, C3);
	det.print();
	std::cout << std::endl;
	return det.expand().checkZeroEquality();
}

bool Prover::started, Prover::theorem;
std::thread Prover::provingThread;
void Prover::proveCollinearity(Point* first, Point* second, Point* third)
{
	started = true;

	ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(first->construction);
	ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(second->construction);
	ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(third->construction);
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	expr C = third_point_data->coord;
	provingThread = std::thread(
		[=]() {
			Prover::theorem = ::proveCollinearity(A, B, C);
			std::cout << theorem;
		});
	provingThread.detach();
}
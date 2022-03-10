#include "Object.h"

IntersectionOfTwoLines::IntersectionOfTwoLines(Point* object, Line* first, Line* second)
	:firstParent(first), secondParent(second), ConstructionPoint(object)
{
	ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->construction);
	ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->construction);
	expr result;
	if (dynamic_cast<Tangent*>(first_line_data) && dynamic_cast<Tangent*>(second_line_data))
	{
		expr C1 = first_line_data->free_coef;
		expr C2 = second_line_data->free_coef;
		result = (C1 * C2) / (C1 + C2);
	}
	else
	{
		expr A1 = first_line_data->z_coef;
		expr A2 = second_line_data->z_coef;
		expr B1 = first_line_data->z_conj_coef;
		expr B2 = second_line_data->z_conj_coef;
		expr C1 = first_line_data->free_coef;
		expr C2 = second_line_data->free_coef;
		result = (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
	}
	coord = result.get_quasi(object->getLowerCaseName());
}

CentralProjection::CentralProjection(UnitPoint* object, UnitCircle* first, Point* second, UnitPoint* third)
	:firstParent(first), secondParent(second), thirdParent(third), ConstructionPoint(object)
{
	ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(second->construction);
	ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(third->construction);
	expr m = first_point_data->coord.get_quasi(second->getLowerCaseName());
	expr a = second_point_data->coord.get_quasi(third->getLowerCaseName());
	expr m_conj = m.conj();
	coord = (m - a) / (expr(make_scalar(1)) - m_conj * a);
}

ByTwoPoints::ByTwoPoints(Object* object, Point* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent), ConstructionLine(object)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr a = first_point_data->coord;
	expr a_conj = a.conj();
	expr b = second_point_data->coord;
	expr b_conj = b.conj();
	z_coef = a_conj - b_conj;
	z_conj_coef = b - a;
	free_coef = a * b_conj - a_conj * b;
}

ByTwoUnitPoints::ByTwoUnitPoints(Object* object, UnitPoint* firstParent, UnitPoint* secondParent) : ByTwoPoints(object, firstParent, secondParent)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr a = first_point_data->coord;
	expr b = second_point_data->coord;
	z_coef = expr(make_scalar(1));
	z_conj_coef = a * b;
	free_coef = a + b;
}

Tangent::Tangent(Object* object, UnitCircle* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent), ConstructionLine(object)
{
	if (!secondParent->isOnCircle())
		throw std::invalid_argument("Point isnt on circle?!");

	const ConstructionPoint* point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr a = point_data->coord;
	z_coef = expr(make_scalar(1));
	z_conj_coef = a * a;
	free_coef = a * 2;
}

Projection::Projection(Point* object, Point* first, Line* second)
	:firstParent(first), secondParent(second), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = dynamic_cast<ConstructionPoint*>(first->construction);
	const ConstructionLine* line_data = dynamic_cast<ConstructionLine*>(second->construction);
	expr m = point_data->coord;
	expr A = line_data->z_coef;
	expr B = line_data->z_conj_coef;
	expr C = line_data->free_coef;
	coord = (C - B * m.conj()) / (A * 2);
}

OnPlane::OnPlane(Point* object, Vector2f point)
	:point(point), ConstructionPoint(object)
{
	coord = expr(make_term(object->getLowerCaseName()));
}

byTwoPointsFixedRatio::byTwoPointsFixedRatio(Object* object, Point* firstParent, Point* secondParent, std::pair<int, int> masses)
	: firstParent(firstParent), secondParent(secondParent), masses(masses), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	coord = ((A * masses.first) + (B * masses.second)) / expr(make_scalar(masses.first + masses.second));
}

OnLine::OnLine(Object* object, Point* firstParent, Point* secondParent, float ratio, Line* thirdParent)
	:firstParent(firstParent), secondParent(secondParent), ratio(ratio), thirdParent(thirdParent), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	expr k = expr(make_real_term("k" + firstParent->getLowerCaseName() + secondParent->getLowerCaseName()));
	coord = (A * k) + (B * (expr(make_scalar(1)) - k));
}

OnCircle::OnCircle(Point* object, UnitCircle* firstParent, float angle)
	:firstParent(firstParent), angle(angle), ConstructionPoint(object)
{
	coord = expr(make_unit_term(object->getLowerCaseName()));
}

IntersectionParallelChord::IntersectionParallelChord(Object* object, UnitCircle* unitCircle, Chord* firstParent, UnitPoint* secondParent)
	: unitCircle(unitCircle), firstParent(firstParent), secondParent(secondParent), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	const ConstructionLine* line_data = dynamic_cast<ConstructionLine*>(firstParent->construction);
	expr m = point_data->coord;
	expr B = line_data->z_conj_coef;
	coord = B / m;
}

IntersectionPerpendicularChord::IntersectionPerpendicularChord(Object* object, UnitCircle* unitCircle, UnitPoint* firstParent, Chord* secondParent)
	:unitCircle(unitCircle), firstParent(firstParent), secondParent(secondParent), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionLine* line_data = dynamic_cast<ConstructionLine*>(secondParent->construction);
	expr m = point_data->coord;
	expr B = line_data->z_conj_coef;
	coord = (-B) / m;
}

Rotation90::Rotation90(Object* object, Point* firstParent, Point* secondParent, int sign)
	:firstParent(firstParent), secondParent(secondParent), sign(sign), ConstructionPoint(object)
{

	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	if (sign == -1)
	{
		coord = A * constTy(1, -1) + B * constTy(0, 1);
	}
	if (sign == 1)
	{
		coord = A * constTy(1, 1) + B * constTy(0, -1);
	}
}

Orthocenter::Orthocenter(Object* object, UnitPoint* firstParent, UnitPoint* secondParent, UnitPoint* thirdParent) : ConstructionPoint(object),
firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	const ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(thirdParent->construction);
	coord = first_point_data->coord + second_point_data->coord + third_point_data->coord;
}

Barycenter::Barycenter(Object* object, Point* firstParent, Point* secondParent, Point* thirdParent) : ConstructionPoint(object),
firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	const ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(thirdParent->construction);
	coord = (first_point_data->coord + second_point_data->coord + third_point_data->coord) / expr(make_scalar(3));
}

Parallel::Parallel(Object* object, Line* first, Point* second)
	:firstParent(first), secondParent(second), ConstructionLine(object)
{
	const ConstructionPoint* point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	const ConstructionLine* line_data = dynamic_cast<ConstructionLine*>(firstParent->construction);
	expr m = point_data->coord;
	expr m_conj = m.conj();
	expr B = line_data->z_conj_coef;
	expr B_conj = B.conj();
	z_coef = m_conj * 2 - m * B_conj;
	z_conj_coef = B * m_conj - m * 2;
	free_coef = B_conj * m * m - B * m_conj * m_conj;
}
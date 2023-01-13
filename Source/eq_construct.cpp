#include "Object.h"

IntersectionOfTwoLines::IntersectionOfTwoLines(Point* object, Line* first, Line* second)
	:firstParent(first), secondParent(second), ConstructionPoint(object)
{
	ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->GetConstruction());
	ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->GetConstruction());
	expr result;
	expr conj_result;
	auto first_tangent_data = dynamic_cast<Tangent*>(first_line_data);
	auto second_tangent_data = dynamic_cast<Tangent*>(second_line_data);
	if (first_tangent_data && second_tangent_data)
	{
		expr A = static_cast<ConstructionPoint*>(first_tangent_data->secondParent->GetConstruction())->coord;
		expr B = static_cast<ConstructionPoint*>(second_tangent_data->secondParent->GetConstruction())->coord;
		result = (A * B * 2) / (A + B);
		conj_result = expr(make_scalar(2)) / (A + B);
		coord = result; //.get_quasi_known_conj(object->getLowerCaseName(), conj_result);
	}
	else
	{
		expr A1 = first_line_data->z_coef;
		expr A2 = second_line_data->z_coef;
		expr B1 = first_line_data->z_conj_coef;
		expr B2 = second_line_data->z_conj_coef;
		expr C1 = first_line_data->free_coef;
		expr C2 = second_line_data->free_coef;
		expr denom = A1 * B2 - A2 * B1;
		result = (C1 * B2 - C2 * B1) / denom;
		conj_result = (A1 * C2 - A2 * C1) / denom;
		coord = result; //.get_quasi_known_conj(object->getLowerCaseName(), conj_result);
	}
}

CentralProjection::CentralProjection(UnitPoint* object, UnitCircle* first, Point* second, UnitPoint* third)
	:firstParent(first), secondParent(second), thirdParent(third), ConstructionPoint(object)
{
	ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(second->GetConstruction());
	ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(third->GetConstruction());
	expr m = first_point_data->coord; //.get_quasi(second->getLowerCaseName());
	expr a = second_point_data->coord;
	expr m_conj = m.conj();
	coord = (m - a) / (expr(make_scalar(1)) - m_conj * a);
}

ByTwoPoints::ByTwoPoints(Object* object, Point* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent), ConstructionLine(object)
{
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	expr a = first_point_data->coord;
	expr a_conj = a.conj();
	expr b = second_point_data->coord;
	expr b_conj = b.conj();
	z_coef = a_conj - b_conj;
	z_conj_coef = b - a;
	free_coef = a_conj * b - a * b_conj;
}

ByTwoUnitPoints::ByTwoUnitPoints(Object* object, UnitPoint* firstParent, UnitPoint* secondParent) : ByTwoPoints(object, firstParent, secondParent)
{
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
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

	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	expr a = point_data->coord;
	z_coef = expr(make_scalar(1));
	z_conj_coef = a * a;
	free_coef = a * 2;
}

Projection::Projection(Point* object, Point* first, Line* second)
	:firstParent(first), secondParent(second), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(first->GetConstruction());
	const ConstructionLine* line_data = static_cast<ConstructionLine*>(second->GetConstruction());
	expr m = point_data->coord;
	expr A = line_data->z_coef;
	expr B = line_data->z_conj_coef;
	expr C = line_data->free_coef;
	coord = (C - B * m.conj() + m * A) / (A * 2);
}

OnPlane::OnPlane(Point* object, Vector2f point)
	:point(point), ConstructionPoint(object)
{
	coord = expr(make_term(object->GetLowerCaseName()));
}

byTwoPointsFixedRatio::byTwoPointsFixedRatio(Object* object, Point* firstParent, Point* secondParent, std::pair<int, int> masses)
	: firstParent(firstParent), secondParent(secondParent), masses(masses), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	expr linear_combination = A * masses.second + B * masses.first;
	if (masses.first + masses.second == 1)
		coord = linear_combination;
	else if (masses.first + masses.second == -1)
		coord = -linear_combination;
	else
		coord = linear_combination / expr(make_scalar(masses.first + masses.second));
}

OnLine::OnLine(Point* object, Point* firstParent, Point* secondParent, float ratio, Line* thirdParent)
	:firstParent(firstParent), secondParent(secondParent), ratio(ratio), thirdParent(thirdParent), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	expr k = expr(make_real_term("k" + object->GetLowerCaseName()));
	coord = (A * k) + (B * (expr(make_scalar(1)) - k));
}

OnCircle::OnCircle(Point* object, UnitCircle* firstParent, float angle)
	:firstParent(firstParent), angle(angle), ConstructionPoint(object)
{
	coord = expr(make_unit_term(object->GetLowerCaseName()));
}

IntersectionParallelChord::IntersectionParallelChord(Object* object, UnitCircle* unit_circle, Chord* firstParent, UnitPoint* secondParent)
	: unit_circle(unit_circle), firstParent(firstParent), secondParent(secondParent), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	const ConstructionLine* line_data = static_cast<ConstructionLine*>(firstParent->GetConstruction());
	expr m = point_data->coord;
	expr B = line_data->z_conj_coef;
	coord = B / m;
}

IntersectionPerpendicularChord::IntersectionPerpendicularChord(Object* object, UnitCircle* unit_circle, UnitPoint* firstParent, Chord* secondParent)
	:unit_circle(unit_circle), firstParent(firstParent), secondParent(secondParent), ConstructionPoint(object)
{

	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionLine* line_data = static_cast<ConstructionLine*>(secondParent->GetConstruction());
	expr m = point_data->coord;
	expr B = line_data->z_conj_coef;
	coord = (-B) / m;
}

Rotation90::Rotation90(Object* object, Point* firstParent, Point* secondParent, int sign)
	:firstParent(firstParent), secondParent(secondParent), sign(sign), ConstructionPoint(object)
{

	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
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
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	const ConstructionPoint* third_point_data = static_cast<ConstructionPoint*>(thirdParent->GetConstruction());
	coord = first_point_data->coord + second_point_data->coord + third_point_data->coord;
}

Incenter::Incenter(Object* object, UnitPoint* firstParent, UnitPoint* secondParent, UnitPoint* thirdParent) : ConstructionPoint(object),
firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
	coord = expr(make_term("i"));
}

Barycenter::Barycenter(Object* object, Point* firstParent, Point* secondParent, Point* thirdParent) : ConstructionPoint(object),
firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
	const ConstructionPoint* first_point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionPoint* second_point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	const ConstructionPoint* third_point_data = static_cast<ConstructionPoint*>(thirdParent->GetConstruction());
	coord = (first_point_data->coord + second_point_data->coord + third_point_data->coord) / expr(make_scalar(3));
}

Parallel::Parallel(Object* object, Line* first, Point* second)
	:firstParent(first), secondParent(second), ConstructionLine(object)
{
	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(secondParent->GetConstruction());
	const ConstructionLine* line_data = static_cast<ConstructionLine*>(firstParent->GetConstruction());
	expr m = point_data->coord;
	expr m_conj = m.conj();
	expr A = line_data->z_coef;
	expr B = line_data->z_conj_coef;
	z_coef = A;
	z_conj_coef = B;
	free_coef = A * m + B * m_conj;
}

Perpendicular::Perpendicular(Object* object, Point* firstParent, Line* secondParent)
	: firstParent(firstParent), secondParent(secondParent), ConstructionLine(object)
{
	const ConstructionPoint* point_data = static_cast<ConstructionPoint*>(firstParent->GetConstruction());
	const ConstructionLine* line_data = static_cast<ConstructionLine*>(secondParent->GetConstruction());
	expr m = point_data->coord;
	expr m_conj = m.conj();
	expr A = line_data->z_coef;
	expr B = line_data->z_conj_coef;
	z_coef = A;
	z_conj_coef = -B;
	free_coef = A * m - B * m_conj;
}
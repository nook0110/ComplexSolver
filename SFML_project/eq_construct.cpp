#include "Object.h"

IntersectionOfTwoLines::IntersectionOfTwoLines(Object* object, Line* first, Line* second)
	:firstParent(first), secondParent(second), ConstructionPoint(object)
{
	const ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->construction);
	const ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->construction);
	expr A1 = first_line_data->z_coef;
	expr A2 = second_line_data->z_coef;
	expr B1 = first_line_data->z_conj_coef;
	expr B2 = second_line_data->z_conj_coef;
	expr C1 = first_line_data->free_coef;
	expr C2 = second_line_data->free_coef;
	coord = (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
}

CentralProjection::CentralProjection(Object* object, UnitCircle* first, Point* second, UnitPoint* third)
	:firstParent(first), secondParent(second), thirdParent(third), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(second->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(third->construction);
	expr m = first_point_data->coord;
	expr a = second_point_data->coord;
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

Projection::Projection(Object* object, Point* first, Line* second)
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

byTwoPointsFixedRatio::byTwoPointsFixedRatio(Object* object, Point* firstParent, Point* secondParent, float ratio)
	:firstParent(firstParent), secondParent(secondParent), ratio(ratio), ConstructionPoint(object)
{
	const ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(firstParent->construction);
	const ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(secondParent->construction);
	expr A = first_point_data->coord;
	expr B = second_point_data->coord;
	coord = (A * ratio) + (B * (1 - ratio));
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
#include "Box.h"
//
//#include "../../Engine/Core/GameManager.h"
//
//Box::Box(const std::string& texID, const Point& boxSize, const Point& initPos)
//	: Object(texID, boxSize)
//{
//	Init(initPos);
//}
//
//Box::Box(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos)
//	: Object(texID, animData, boxSize)
//{
//	Init(initPos);
//}
//
//void Box::Reset()
//{
//	m_canHit = true;
//	m_justHit = false;
//	Object::Reset();
//}
//
//void Box::WasJustHit()
//{
//	if (m_canHit)
//		m_justHit = true;
//}
//
//void Box::Init(const Point& initPos)
//{
//	SetInitialDirection(true);
//	SetDirection(GetInitialDirection());
//	SetInitialPosition(initPos);
//	SetPosition(GetInitialPosition());
//	GetBoundingBox()->Update(GetPosition());
//}
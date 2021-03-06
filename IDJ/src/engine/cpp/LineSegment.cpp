/*
 * LineSegment.cpp
 *
 *  Created on: Jun 9, 2014
 *      Author: Gabriel Naves
 */

#include "../include/LineSegment.h"

LineSegment::LineSegment() : p1(), p2()
{
    a = b = 0;
}

LineSegment::LineSegment(const Point& p1, const Point& p2) : p1(p1), p2(p2)
{
    FindAandB();
}

LineSegment::LineSegment(float x1, float y1, float x2, float y2)
{
    p1.Set(x1, y1);
    p2.Set(x2, y2);
    FindAandB();
}

void LineSegment::Set(const Point& p1, const Point& p2)
{
    this->p1.Set(p1);
    this->p1.Set(p2);
    FindAandB();
}

void LineSegment::Set(float x1, float y1, float x2, float y2)
{
    this->p1.Set(x1, y1);
    this->p2.Set(x2, y2);
    FindAandB();
}

void LineSegment::FindAandB()
{
    // y-y0 = a(x-x0)
    a = (p1.GetY() - p2.GetY()) / (p1.GetX() - p2.GetX());
    b = p1.GetY() - a * p1.GetX();
}

bool LineSegment::IsAbove(const Point& point)
{
    if (a*point.GetX() + b > point.GetY()) return true;
    return false;
}

bool LineSegment::IsBelow(const Point& point)
{
    if (a*point.GetX() + b < point.GetY()) return true;
    return false;
}

float LineSegment::GetHighestX()
{
    return p1.GetX() >= p2.GetX() ? p1.GetX() : p2.GetX();
}

float LineSegment::GetLowestX()
{
    return p1.GetX() <= p2.GetX() ? p1.GetX() : p2.GetX();
}

float LineSegment::GetVerticalDistance(const Point& point)
{
    return (a*point.GetX()+b) - point.GetY();
}

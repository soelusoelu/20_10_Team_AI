#include "Triangle.h"

Triangle::Triangle()
    : p0()
    , p1()
    , p2()
    , normal()
{
}

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    : p0(p0)
    , p1(p1)
    , p2(p2)
    , normal()
{
    auto p0p1 = p1 - p0;
    auto p0p2 = p2 - p0;

    //外積により2辺に垂直なベクトルを算出する
    normal = Vector3::cross(p0p1, p0p2);
    normal.normalize();
}

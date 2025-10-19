#ifndef RAYLIB_TEST_MATH_VEC3_H
#define RAYLIB_TEST_MATH_VEC3_H

#include <math.h>
#include <raylib.h>

static const Vector3 Vector3UP = {0.0f, 1.0f, 0.0f};
static const Vector3 Vector3DOWN = {0.0f, -1.0f, 0.0f};

#define VECTOR3_OP(NAME, OP) static inline Vector3 Vector3##NAME(const Vector3 v1, const Vector3 v2) \
{ \
    return (Vector3){ \
        (v1.x) OP (v2.x), \
        (v1.y) OP (v2.y), \
        (v1.z) OP (v2.z)}; \
}

#define VECTOR3_OP_IP(NAME, OP) static inline void Vector3##NAME##Ip(Vector3 *v1, const Vector3 v2) \
{ \
    v1->x = v1->x OP v2.x; \
    v1->y = v1->y OP v2.y; \
    v1->z = v1->z OP v2.z; \
}

#define VECTOR3_OP_SCALAR(NAME, OP) static inline Vector3 Vector3##NAME##Scalar(const Vector3 v1, const float s) \
{ \
    return (Vector3){ \
        (v1.x) OP (s), \
        (v1.y) OP (s), \
        (v1.z) OP (s)}; \
}

#define VECTOR3_OP_IP_SCALAR(NAME, OP) static inline void Vector3##NAME##ScalarIp(Vector3 *v1, const float s) \
{ \
    v1->x = v1->x OP s; \
    v1->y = v1->y OP s; \
    v1->z = v1->z OP s; \
}

VECTOR3_OP(Add, +)
VECTOR3_OP(Sub, -)
VECTOR3_OP(Mul, *)
VECTOR3_OP(Div, /)

VECTOR3_OP_IP(Add, +)
VECTOR3_OP_IP(Sub, -)
VECTOR3_OP_IP(Mul, *)
VECTOR3_OP_IP(Div, /)

VECTOR3_OP_SCALAR(Add, +)
VECTOR3_OP_SCALAR(Sub, -)
VECTOR3_OP_SCALAR(Mul, *)
VECTOR3_OP_SCALAR(Div, /)

VECTOR3_OP_IP_SCALAR(Add, +)
VECTOR3_OP_IP_SCALAR(Sub, -)
VECTOR3_OP_IP_SCALAR(Mul, *)
VECTOR3_OP_IP_SCALAR(Div, /)

static inline Vector3 Vector3Cross(const Vector3 v1, const Vector3 v2)
{
    return (Vector3){
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x
    };
}

static inline float Vector3Dot(const Vector3 v1, const Vector3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline float Vector3LengthSquared(const Vector3 v)
{
    return Vector3Dot(v, v);
}

static inline float Vector3Length(const Vector3 v)
{
    return sqrtf(Vector3LengthSquared(v));
}

static inline Vector3 Vector3Normalize(const Vector3 v)
{
    return Vector3DivScalar(v, Vector3Length(v));
}

static inline bool Vector3IsZero(const Vector3 v)
{
    // exact match for now. consider adding an IsClose function or similar later on
    return Vector3LengthSquared(v) == 0.0;
}

static inline Vector3 Vector3Rotate(const Vector3 v, const Vector3 axis, const float angle)
{
    const Vector3 naxis = Vector3Normalize(axis);
    const Vector3 a = Vector3MulScalar(v, cosf(angle));
    const Vector3 b = Vector3MulScalar(Vector3Cross(naxis, v), sinf(angle));
    const Vector3 c = Vector3MulScalar(Vector3MulScalar(naxis, Vector3Dot(naxis, v)), 1 - cosf(angle));
    return Vector3Add(Vector3Add(a, b), c);
}

#define PrintVector3(V) {\
    typeof (V) _local_V = (V); \
    printf("{.x=%f, .y=%f, .z=%f}\n", _local_V.x, _local_V.y, _local_V.z); \
    }


#endif //RAYLIB_TEST_MATH_VEC3_H

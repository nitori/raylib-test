#ifndef RAYLIB_TEST_COMPONENTS_H
#define RAYLIB_TEST_COMPONENTS_H

#include <raylib.h>
#include <flecs.h>

typedef Vector3 Translation;
typedef Quaternion Rotation;
typedef Vector3 Scale;

typedef Vector3 Velocity;

typedef struct {
    Vector3 center;
    float radius;
    float halfLength;
    Vector3 direction;
} Capsule3D;

typedef struct {
    Vector3 lookDirection;
} Player;

void RegsiterComponentReflection();

void InitWorld(int argc, char *argv[]);

void DrawCapsule3D(Capsule3D cap, int slices, int rings, Color color);

void DrawCapsule3DWires(Capsule3D cap, int slices, int rings, Color color);

extern ecs_world_t *world;
extern ECS_COMPONENT_DECLARE(Translation);
extern ECS_COMPONENT_DECLARE(Rotation);
extern ECS_COMPONENT_DECLARE(Scale);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(Capsule3D);

extern ECS_TAG_DECLARE(IsPlayer);
extern ECS_TAG_DECLARE(Local);
extern ECS_TAG_DECLARE(World);

#endif //RAYLIB_TEST_COMPONENTS_H

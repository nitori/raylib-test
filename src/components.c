#include "components.h"
#include "math/vec3.h"

ecs_world_t *world;
ECS_COMPONENT_DECLARE(Translation);
ECS_COMPONENT_DECLARE(Rotation);
ECS_COMPONENT_DECLARE(Scale);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(Capsule3D);
ECS_TAG_DECLARE(PlayerTag);
ECS_TAG_DECLARE(Local);
ECS_TAG_DECLARE(World);

void DrawCapsule3D(const Capsule3D cap, const int slices, const int rings, const Color color)
{
    const Vector3 start = Vector3Sub(cap.center, Vector3MulScalar(cap.direction, cap.halfLength));
    const Vector3 end = Vector3Add(cap.center, Vector3MulScalar(cap.direction, cap.halfLength));
    DrawCapsule(start, end, cap.radius, slices, rings, color);
}

void DrawCapsule3DWires(const Capsule3D cap, const int slices, const int rings, const Color color)
{
    const Vector3 start = Vector3Sub(cap.center, Vector3MulScalar(cap.direction, cap.halfLength));
    const Vector3 end = Vector3Add(cap.center, Vector3MulScalar(cap.direction, cap.halfLength));
    DrawCapsuleWires(start, end, cap.radius, slices, rings, color);
}

void RegsiterComponentReflection()
{
    // Register reflection for the existing struct layout
    ecs_struct_init(
        world,
        &(ecs_struct_desc_t){
            .entity = ecs_id(Translation),
            .members = {
                {.name = "x", .type = ecs_id(ecs_f32_t), .offset = offsetof(Translation, x)},
                {.name = "y", .type = ecs_id(ecs_f32_t), .offset = offsetof(Translation, y)},
                {.name = "z", .type = ecs_id(ecs_f32_t), .offset = offsetof(Translation, z)},
            }
        }
    );
}

void InitWorld(int argc, char *argv[])
{
    world = ecs_init_w_args(argc, argv);
    ECS_COMPONENT_DEFINE(world, Translation);
    ECS_COMPONENT_DEFINE(world, Rotation);
    ECS_COMPONENT_DEFINE(world, Scale);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, Capsule3D);

    ECS_TAG_DEFINE(world, PlayerTag);
    ECS_TAG_DEFINE(world, Local);
    ECS_TAG_DEFINE(world, World);
}

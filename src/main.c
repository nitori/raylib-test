#include <stdio.h>
#include <time.h>

#include <raylib.h>
#include <flecs.h>

#include "math/vec3.h"
#include "components.h"

void setup_systems();

void SystemSetup(ecs_iter_t *it);

void SystemUpdateWorldTransforms(ecs_iter_t *it);

void SystemDrawBasicShapes(ecs_iter_t *it);

void SystemUpdate(ecs_iter_t *it);


void setup_systems()
{
    ecs_entity_t GlobalTransformPhase = ecs_new_w_id(world, EcsPhase);
    ecs_entity_t DrawPhase = ecs_new_w_id(world, EcsPhase);

    ecs_add_pair(world, GlobalTransformPhase, EcsDependsOn, EcsOnUpdate);
    ecs_add_pair(world, DrawPhase, EcsDependsOn, GlobalTransformPhase);

    ECS_SYSTEM(world, SystemSetup, EcsOnStart);
    ECS_SYSTEM(world, SystemUpdate, EcsOnUpdate, [out] Translation, [in] IsPlayer);
    ECS_SYSTEM(world, SystemUpdateWorldTransforms, GlobalTransformPhase,
               [in] Translation,
               [out] (Translation, World),
               [in] ?Translation(cascade, World));
    ECS_SYSTEM(world, SystemDrawBasicShapes, DrawPhase, [in] (Translation, World), [out] Capsule3D);
}

void SystemSetup(ecs_iter_t *it)
{
    ecs_entity_t player = ecs_entity(world, {.name = "Player"});
    // world and local translation
    ecs_add(world, player, IsPlayer);
    ecs_set_pair(world, player, Translation, World, {0});
    ecs_set(world, player, Translation, {0, 3., 0});
    ecs_set(world, player, Capsule3D, {
            .center = {0},
            .direction = Vector3UP,
            .halfLength = 2.0f,
            .radius = 1.0f,
            });

    ecs_entity_t hat = ecs_entity(world, {.name = "Hat"});
    ecs_add_pair(world, hat, EcsChildOf, player);
    ecs_set_pair(world, hat, Translation, World, {1, 1, 1});
    ecs_set(world, hat, Translation, {0.5f, 2.f, -0.5f});
    ecs_set(world, hat, Capsule3D, {
            .center = {0},
            .direction = Vector3UP,
            .halfLength = .5f,
            .radius = 1.2f,
            });
}

void SystemUpdateWorldTransforms(ecs_iter_t *it)
{
    Translation *t_local = ecs_field(it, Translation, 0);
    Translation *t_world = ecs_field(it, Translation, 1);
    Translation *parent_world = ecs_field(it, Translation, 2);

    for (int i = 0; i < it->count; i++) {
        t_world[i].x = t_local[i].x;
        t_world[i].y = t_local[i].y;
        t_world[i].z = t_local[i].z;
        if (parent_world) {
            t_world[i].x += parent_world->x;
            t_world[i].y += parent_world->y;
            t_world[i].z += parent_world->z;
        }
    }
}

void SystemDrawBasicShapes(ecs_iter_t *it)
{
    Translation *t = ecs_field(it, Translation, 0);
    Capsule3D *c = ecs_field(it, Capsule3D, 1);

    for (int i = 0; i < it->count; i++) {
        const char *name = ecs_get_name(world, it->entities[i]);

        c[i].center = t[i];

        if (name[0] == 'P') {
            DrawCapsule3DWires(c[i], 10, 10, RED);
        } else {
            DrawCapsule3DWires(c[i], 10, 10, BLUE);
        }
    }
}

void SystemUpdate(ecs_iter_t *it)
{
    Translation *t = ecs_field(it, Translation, 0);

    Vector3 dir = {
        .x = (float) (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        .y = 0.0f,
        .z = (float) (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    for (int i = 0; i < it->count; i++) {
        if (Vector3LengthSquared(dir) == 0.0) {
            continue;
        }
        dir = Vector3Normalize(dir);
        t[i] = Vector3Add(t[i], Vector3MulScalar(dir, it->delta_system_time * 5.f));
    }
}


int main(const int argc, char *argv[])
{
    // flecs setup
    ecs_log_enable_colors(false);
    InitWorld(argc, argv);

    // rest api stuff
    ECS_IMPORT(world, FlecsStats);
    ecs_singleton_set(world, EcsRest, {0});
    RegsiterComponentReflection();
    // rest api stuff end

    setup_systems();

    // raylib setup
    InitWindow(800, 450, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(120);

    Camera3D camera = {
        .position = {0, 5.f, 15.f},
        .up = Vector3UP,
        .target = {0, 2.f, 0},
        .fovy = 45.f,
        .projection = CAMERA_PERSPECTIVE,
    };

    // @formatter:off
    ecs_query_t *q = ecs_query(world, {
        .terms = {
            { .id = ecs_id(Capsule3D), .inout = EcsIn }
        },
    });
    // @formatter:on

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            if (IsKeyPressed(KEY_H)) {
                if (IsCursorHidden()) {
                    EnableCursor();
                } else {
                    DisableCursor();
                }
            }

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
            {
                ecs_log_set_level(1);
                ecs_progress(world, 0);
                ecs_log_set_level(-1);
                DrawGrid(10, 1.f);
            }
            EndMode3D();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

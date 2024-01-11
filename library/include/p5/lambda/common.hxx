#pragma once

#include <tuple>
#include <array>
#include <type_traits>

#include "common+fwd.hxx"


namespace p5::lambda::common {

struct Vector3D final { Scalar x = 0, y = 0, z = 0; };

namespace hud {

struct TextParams final {
    float x = 0;
    float y = 0;
    int effect = 0;
    Byte r1 = 0, g1 = 0, b1 = 0, a1 = 0;
    Byte r2 = 0, g2 = 0, b2 = 0, a2 = 0;
    float fade_in_time = 0;
    float fade_out_time = 0;
    float hold_time = 0;
    float fx_ime = 0;
    int channel = 0;
};

} // namespace hud

namespace trace {

struct Result final {
    int all_solid = false;                  // if true, plane is not valid
    int start_solid = false;                // if true, the initial point was in a solid area
    int in_open = 0;
    int in_water = 0;
    float fraction = 1;                     // time completed, 1.0 = didn't hit anything
    Vector3D final_position = {};           // final position
    float plane_distance = 0;
    Vector3D plane_normal = {};             // surface normal at impact
    entity::Dictionary *entity = nullptr;   // entity the surface is on
    int hit_group = 0;                      // 0 == generic, non zero is specific body part
};

} // namespace trace

namespace entity {

struct Link final { Link *previous = nullptr, *next = nullptr; };

struct Variables final {
    IntegerString class_name = 0;
    IntegerString global_name = 0;

    Vector3D origin;
    Vector3D old_origin;
    Vector3D velocity;
    Vector3D base_velocity;
    Vector3D client_base_velocity;  // Base velocity that was passed in to server physics so client can predict conveyors correctly. Server zeroes it, so we need to store here, too.
    Vector3D move_direction;

    Vector3D angles;                // Model angles
    Vector3D angular_velocity;      // angle velocity (degrees per second)
    Vector3D punch_angle;           // auto-decaying view angle adjustment
    Vector3D view_angle;            // Viewing angle (player only)

    // For parametric entities
    Vector3D end_position;
    Vector3D start_position;
    float impact_time = 0;
    float start_time = 0;

    int fix_angle = 0;              // 0:nothing, 1:force view angles, 2:add avelocity
    float ideal_pitch = 0;
    float pitch_speed = 0;
    float ideal_yaw = 0;
    float yaw_speed = 0;

    int model_index = 0;
    IntegerString model = 0;

    int view_model = 0;             // player's viewmodel
    int weapon_model = 0;           // what other players see

    Vector3D abs_min;               // BB max translated to world coord
    Vector3D abs_max;               // BB max translated to world coord
    Vector3D mins;                  // local BB min
    Vector3D maxs;                  // local BB max
    Vector3D size;                  // maxs - mins

    float last_think_time = 0;
    float next_think = 0;

    MoveType move_type = ::std::decay_t<decltype(move_type)>::None;
    SolidType solid = ::std::decay_t<decltype(solid)>::Not;

    int skin = 0;
    int body = 0;                   // sub-model selection for studiomodels
    EffectFlags effects = ::std::decay_t<decltype(effects)>::None;

    float gravity = 0;              // % of "normal" gravity
    float friction = 0;             // inverse elasticity of BOUNCE

    int light_level = 0;

    int sequence = 0;               // animation sequence
    int gait_sequence = 0;          // movement animation sequence for player (0 for none)
    float frame = 0;                // % playback position in animation sequences (0..255)
    float animation_time = 0;       // world time when frame was set
    float frame_rate = 0;           // animation playback rate (-8x to 8x)
    Byte controller[4] = {0};       // bone controller setting (0..255)
    Byte blending[2] = {0};         // blending amount between sub-sequences (0..255)

    float scale = 0;                // sprite rendering scale (0..255)

    RenderMode render_mode = ::std::decay_t<decltype(render_mode)>::Normal;
    float render_amount = 0;
    Vector3D render_color;
    RenderEffect render_effect = ::std::decay_t<decltype(render_effect)>::None;

    float health = 0;
    float frags = 0;
    int weapons = 0;                // bit mask for available weapons
    float take_damage = static_cast<::std::decay_t<decltype(take_damage)>>(DamageState::No);

    DeadState dead_state = ::std::decay_t<decltype(dead_state)>::No;
    Vector3D view_offset;           // eye position

    int button = 0;
    int impulse = 0;

    Dictionary *chain = nullptr;    // Entity pointer when linked into a linked list
    Dictionary *damage_inflictor = nullptr;
    Dictionary *enemy = nullptr;
    Dictionary *aim = nullptr;      // entity pointer when FOLLOW
    Dictionary *owner = nullptr;
    Dictionary *ground = nullptr;

    int spawn_flags = 0;
    int flags = 0;

    int colormap = 0;               // lowByte topcolor, highByte bottomcolor
    int team = 0;

    float max_health = 0;
    float teleport_time = 0;
    float armor_type = 0;
    float armor_value = 0;
    int water_level = 0;
    int water_type = 0;

    IntegerString target = 0;
    IntegerString target_name = 0;
    IntegerString net_name = 0;
    IntegerString message = 0;

    float damage_take = 0;
    float damage_save = 0;
    float damage = 0;
    float damage_time = 0;

    ::std::array<IntegerString, 4> noise;

    float speed = 0;
    float air_finished = 0;
    float pain_finished = 0;
    float rad_suit_finished = 0;

    Dictionary *container = nullptr;

    int player_class = 0;
    float max_speed = 0;

    float fov = 0;
    int weapon_animation = 0;
    int push_millisecond = 0;

    int in_duck = 0;
    int step_sound_time = 0;
    int swim_time = 0;
    int duck_time = 0;
    int step_left = 0;
    float fall_velocity = 0;

    int game_state = 0;
    int old_buttons = 0;
    int groupinfo = 0;

    // For mods
    ::std::tuple<
        ::std::array<int, 4>,
        ::std::array<float, 4>,
        ::std::array<Vector3D, 4>,
        ::std::array<Dictionary *, 4>
    > user;
};

struct Dictionary final {
    inline constexpr static auto max_leafs = 48;

    IntegerBoolean free = IntegerBoolean::False;
    int serial_number = 0;
    Link area = {};                 // linked to a division node or leaf

    int head_node = 0;              // -1 to use normal leaf check
    int num_leafs = 0;
    short leaf_nums[max_leafs] = {0};

    float free_time = 0;            // sv.time when the object was freed
    void *private_data = nullptr;   // Alloced and freed by engine, used by DLLs
    Variables variables = {};       // C exported fields from progs

    // other fields from progs come immediately after
};

} // namespace entity
} // namespace p5::lambda::common


#include "common+inl.hxx"

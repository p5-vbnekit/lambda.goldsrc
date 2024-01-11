#pragma once

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
    float fadeinTime = 0;
    float fadeoutTime = 0;
    float holdTime = 0;
    float fxTime = 0;
    int channel = 0;
};

} // namespace hud

namespace trace {

struct Result final {
    int fAllSolid = false;              // if true, plane is not valid
    int fStartSolid = false;            // if true, the initial point was in a solid area
    int fInOpen = 0;
    int fInWater = 0;
    float flFraction = 1;               // time completed, 1.0 = didn't hit anything
    Vector3D vecEndPos = {};            // final position
    float flPlaneDist = 0;
    Vector3D vecPlaneNormal = {};       // surface normal at impact
    entity::Dictionary *pHit = nullptr; // entity the surface is on
    int iHitgroup = 0;                  // 0 == generic, non zero is specific body part
};

} // namespace trace

namespace entity {

struct Link final { Link *prev = nullptr, *next = nullptr; };

struct Variables final {
    IntegerString classname = 0;
    IntegerString globalname = 0;

    Vector3D origin;
    Vector3D oldorigin;
    Vector3D velocity;
    Vector3D basevelocity;
    Vector3D clbasevelocity;        // Base velocity that was passed in to server physics so client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
    Vector3D movedir;

    Vector3D angles;                // Model angles
    Vector3D avelocity;             // angle velocity (degrees per second)
    Vector3D punchangle;            // auto-decaying view angle adjustment
    Vector3D v_angle;               // Viewing angle (player only)

    // For parametric entities
    Vector3D endpos;
    Vector3D startpos;
    float impacttime = 0;
    float starttime = 0;

    int fixangle = 0;               // 0:nothing, 1:force view angles, 2:add avelocity
    float idealpitch = 0;
    float pitch_speed = 0;
    float ideal_yaw = 0;
    float yaw_speed = 0;

    int modelindex = 0;
    IntegerString model = 0;

    int viewmodel = 0;              // player's viewmodel
    int weaponmodel = 0;            // what other players see

    Vector3D absmin;                // BB max translated to world coord
    Vector3D absmax;                // BB max translated to world coord
    Vector3D mins;                  // local BB min
    Vector3D maxs;                  // local BB max
    Vector3D size;                  // maxs - mins

    float ltime = 0;
    float nextthink = 0;

    MoveType movetype = ::std::decay_t<decltype(movetype)>::None;
    SolidType solid = ::std::decay_t<decltype(solid)>::Not;

    int skin = 0;
    int body = 0;                   // sub-model selection for studiomodels
    EffectFlags effects = ::std::decay_t<decltype(effects)>::None;

    float gravity = 0;              // % of "normal" gravity
    float friction = 0;             // inverse elasticity of BOUNCE

    int light_level = 0;

    int sequence = 0;               // animation sequence
    int gaitsequence = 0;           // movement animation sequence for player (0 for none)
    float frame = 0;                // % playback position in animation sequences (0..255)
    float animtime = 0;             // world time when frame was set
    float framerate = 0;            // animation playback rate (-8x to 8x)
    Byte controller[4] = {0};       // bone controller setting (0..255)
    Byte blending[2] = {0};         // blending amount between sub-sequences (0..255)

    float scale = 0;                // sprite rendering scale (0..255)

    RenderMode rendermode = ::std::decay_t<decltype(rendermode)>::Normal;
    float renderamt = 0;
    Vector3D rendercolor;
    RenderFx renderfx = ::std::decay_t<decltype(renderfx)>::None;

    float health = 0;
    float frags = 0;
    int weapons = 0;                // bit mask for available weapons
    float takedamage = static_cast<::std::decay_t<decltype(takedamage)>>(DamageState::No);

    DeadState deadstate = ::std::decay_t<decltype(deadstate)>::No;
    Vector3D view_ofs;              // eye position

    int button = 0;
    int impulse = 0;

    Dictionary *chain = nullptr;    // Entity pointer when linked into a linked list
    Dictionary *dmg_inflictor = nullptr;
    Dictionary *enemy = nullptr;
    Dictionary *aiment = nullptr;   // entity pointer when FOLLOW
    Dictionary *owner = nullptr;
    Dictionary *groundentity = nullptr;

    int spawnflags = 0;
    int flags = 0;

    int colormap = 0;               // lowByte topcolor, highByte bottomcolor
    int team = 0;

    float max_health = 0;
    float teleport_time = 0;
    float armortype = 0;
    float armorvalue = 0;
    int waterlevel = 0;
    int watertype = 0;

    IntegerString target = 0;
    IntegerString targetname = 0;
    IntegerString netname = 0;
    IntegerString message = 0;

    float dmg_take = 0;
    float dmg_save = 0;
    float dmg = 0;
    float dmgtime = 0;

    IntegerString noise = 0;
    IntegerString noise1 = 0;
    IntegerString noise2 = 0;
    IntegerString noise3 = 0;

    float speed = 0;
    float air_finished = 0;
    float pain_finished = 0;
    float radsuit_finished = 0;

    Dictionary *pContainingEntity = nullptr;

    int playerclass = 0;
    float maxspeed = 0;

    float fov = 0;
    int weaponanim = 0;
    int pushmsec = 0;

    int bInDuck = 0;
    int flTimeStepSound = 0;
    int flSwimTime = 0;
    int flDuckTime = 0;
    int iStepLeft = 0;
    float flFallVelocity = 0;

    int gamestate = 0;
    int oldbuttons = 0;
    int groupinfo = 0;

    // For mods
    int iuser1 = 0;
    int iuser2 = 0;
    int iuser3 = 0;
    int iuser4 = 0;
    float fuser1 = 0;
    float fuser2 = 0;
    float fuser3 = 0;
    float fuser4 = 0;
    Vector3D vuser1;
    Vector3D vuser2;
    Vector3D vuser3;
    Vector3D vuser4;
    Dictionary *euser1 = nullptr;
    Dictionary *euser2 = nullptr;
    Dictionary *euser3 = nullptr;
    Dictionary *euser4 = nullptr;
};

struct Dictionary final {
    inline constexpr static auto maxLeafs = 48;

    IntegerBoolean free = IntegerBoolean::False;
    int serialnumber = 0;
    Link area = {};                 // linked to a division node or leaf

    int headnode = 0;               // -1 to use normal leaf check
    int num_leafs = 0;
    short leafnums[maxLeafs] = {0};

    float freetime = 0;             // sv.time when the object was freed
    void *pvPrivateData = nullptr;  // Alloced and freed by engine, used by DLLs
    Variables v = {};               // C exported fields from progs

    // other fields from progs come immediately after
};

} // namespace entity
} // namespace p5::lambda::common


#include "common+inl.hxx"

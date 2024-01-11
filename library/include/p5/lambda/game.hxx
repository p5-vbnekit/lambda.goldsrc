#pragma once

#include "game+fwd.hxx"

#include "common.hxx"


namespace p5::lambda::game {
namespace api {

constexpr auto version(parent_::functions::Standard const *) noexcept(true);
constexpr auto version(parent_::functions::Extension const *) noexcept(true);

} // namespace api

namespace functions {

struct Standard final {
    // Initialize/shutdown the game (one-time call after loading of game .dll)
    void (*gameInit)(void) = nullptr;
    int (*spawn)(common::entity::Dictionary *) = nullptr;
    void (*think)(common::entity::Dictionary *) = nullptr;
    void (*use)(common::entity::Dictionary *used, common::entity::Dictionary *other) = nullptr;
    void (*touch)(common::entity::Dictionary *touched, common::entity::Dictionary *other) = nullptr;
    void (*blocked)(common::entity::Dictionary *blocked, common::entity::Dictionary *other) = nullptr;
    void (*keyValue)(common::entity::Dictionary *, common::key_value::Data *) = nullptr;
    void (*save)(common::entity::Dictionary *, common::save_restore::Data *) = nullptr;
    int (*restore)(common::entity::Dictionary *, common::save_restore::Data *, int global) = nullptr;
    void (*setAbsBox)(common::entity::Dictionary *) = nullptr;

    void (*saveWriteFields)(common::save_restore::Data *, char const *, void *, common::TypeDescription *, int) = nullptr;
    void (*saveReadFields)(common::save_restore::Data *, char const *, void *, common::TypeDescription *, int) = nullptr;

    void (*saveGlobalState)(common::save_restore::Data *) = nullptr;
    void (*restoreGlobalState)(common::save_restore::Data *) = nullptr;
    void (*resetGlobalState)(void) = nullptr;

    common::IntegerBoolean (*clientConnect)(common::entity::Dictionary *, char const *name, char const *address, char rejectReason[128]) = nullptr;

    void (*clientDisconnect)(common::entity::Dictionary *) = nullptr;
    void (*clientKill)(common::entity::Dictionary *) = nullptr;
    void (*clientPutInServer)(common::entity::Dictionary *) = nullptr;
    void (*clientCommand)(common::entity::Dictionary *) = nullptr;
    void (*clientUserInfoChanged)(common::entity::Dictionary *, char *buffer) = nullptr;

    void (*serverActivate)(common::entity::Dictionary *list, int count, int clientMax) = nullptr;
    void (*serverDeactivate)(void) = nullptr;

    void (*playerPreThink)(common::entity::Dictionary *) = nullptr;
    void (*playerPostThink)(common::entity::Dictionary *) = nullptr;

    void (*startFrame)(void) = nullptr;
    void (*parmsNewLevel)(void) = nullptr;
    void (*parmsChangeLevel)(void) = nullptr;

    // Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
    char const * (*getGameDescription)(void) = nullptr;

    // Notify dll about a player customization.
    void (*playerCustomization)(common::entity::Dictionary *, common::Customization *) = nullptr;

    // Spectator funcs
    void (*spectatorConnect)(common::entity::Dictionary *) = nullptr;
    void (*spectatorDisconnect)(common::entity::Dictionary *) = nullptr;
    void (*spectatorThink)(common::entity::Dictionary *) = nullptr;

    // Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.
    void (*sys_Error)(char const *) = nullptr;

    void (*pM_Move) (common::player::Move *, common::IntegerBoolean server) = nullptr;
    void (*pM_Init) (common::player::Move *) = nullptr;
    char (*pM_FindTextureType)(char *name) = nullptr;
    void (*setupVisibility)(common::entity::Dictionary *view, common::entity::Dictionary *client, unsigned char **pvs, unsigned char **pas) = nullptr;
    void (*updateClientData)(common::entity::Dictionary const *, int sendWeapons, common::client::Data *) = nullptr;
    int (*addToFullPack)(common::entity::State *, int e, common::entity::Dictionary *, common::entity::Dictionary *host, int hostFlags, int player, unsigned char *set) = nullptr;
    void (*createBaseline)(int player, int entityIndex, common::entity::State *baseLine, common::entity::Dictionary *, int playerModelindex, common::Vector3D playerMins, common::Vector3D playerMaxs) = nullptr;
    void(*registerEncoders)(void) = nullptr;
    int (*getWeaponData)(common::entity::Dictionary *, common::weapon::Data *) = nullptr;

    void (*cmdStart)(common::entity::Dictionary const *, common::user::Command const *, unsigned int randomSeed) = nullptr;
    void (*cmdEnd)(common::entity::Dictionary const *) = nullptr;

    // Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
    //  size of the response_buffer, so you must zero it out if you choose not to respond.
    int (*connectionlessPacket)(common::network::Address const *from, char const *args, char *responseBuffer, int *responseBufferSize) = nullptr;

    // Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
    int (*getHullBounds)(int hull, float *mins, float *maxs) = nullptr;

    // Create baselines for certain "unplaced" items.
    void (*createInstancedBaselines)(void) = nullptr;

    // One of the forceUnmodified files failed the consistency check for the specified player
    // Return 0 to allow the client to continue, 1 to force immediate disconnection (with an optional disconnect message of up to 256 characters)
    int (*inconsistentFile)(common::entity::Dictionary const *, char const *path, char *disconnectMessage) = nullptr;

    // The game .dll should return 1 if lag compensation should be allowed (could also just set the sv_unlag cvar).
    // Most games right now should return 0, until client-side weapon prediction code is written and tested for them.
    int (*allowLagCompensation)(void) = nullptr;
};

struct Extension final {
    // Called right before the object's memory is freed. Calls its destructor.
    void (*onFreeEntPrivateData)(common::entity::Dictionary *) = nullptr;
    void (*gameShutdown)(void) = nullptr;
    int (*shouldCollide)(common::entity::Dictionary *touched, common::entity::Dictionary *other) = nullptr;

    // Added 2005/08/11 (no SDK update):
    void (*cvarValue)(common::entity::Dictionary const *, char const *) = nullptr;

    // Added 2005/11/21 (no SDK update):
    //    value is "Bad CVAR request" on failure (i.e that user is not connected or the console variable does not exist).
    //    value is "Bad Player" if invalid player entity dictionary.
    void (*cvarValue2)(common::entity::Dictionary const *, int requestID, char const *name, char const *value) = nullptr;
};

struct Pointers final {
    using Standard = functions::Standard;
    using Extension = functions::Extension;

    Standard const *standard = nullptr;
    Extension const *extension = nullptr;
};

} // namespace functions

struct Functions final {
    using Standard = functions::Standard;
    using Extension = functions::Extension;

    Standard standard = {};
    Extension extension = {};
};

} // namespace p5::lambda::game


#include "game+inl.hxx"

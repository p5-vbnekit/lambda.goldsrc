#pragma once

#include "engine+fwd.hxx"

#include "common.hxx"


namespace p5::lambda::engine {
namespace api {

consteval auto version() noexcept(true);

} // namespace api

struct Globals final {
    float time = 0;
    float frametime = 0;
    float force_retouch = 0;
    common::IntegerString mapname = 0;
    common::IntegerString startspot = 0;
    float deathmatch = 0;
    float coop = 0;
    float teamplay = 0;
    float serverflags = 0;
    float found_secrets = 0;
    common::Vector3D v_forward = {};
    common::Vector3D v_up = {};
    common::Vector3D v_right = {};
    float trace_allsolid = 0;
    float trace_startsolid = 0;
    float trace_fraction = 0;
    common::Vector3D trace_endpos = {};
    common::Vector3D trace_plane_normal = {};
    float trace_plane_dist = 0;
    common::entity::Dictionary *trace_ent = nullptr;
    float trace_inopen = 0;
    float trace_inwater = 0;
    int trace_hitgroup = 0;
    int trace_flags = 0;
    int msg_entity = 0;
    int cdAudioTrack = 0;
    int maxClients = 0;
    int maxEntities = 0;
    char const *pStringBase = nullptr;

    void *pSaveData = nullptr;
    common::Vector3D vecLandmarkOffset = {};
};

struct Functions final {
    int (*precacheModel)(char const *) = nullptr;
    int (*precacheSound)(char const *) = nullptr;
    void (*setModel)(common::entity::Dictionary *, char const *) = nullptr;
    int (*modelIndex)(char const *) = nullptr;
    int (*modelFrames)(int index) = nullptr;
    void (*setSize)(common::entity::Dictionary *, float const *min, float const *max) = nullptr;
    void (*changeLevel)(char *, char *) = nullptr;
    void (*getSpawnParms)(common::entity::Dictionary const *) = nullptr;
    void (*saveSpawnParms)(common::entity::Dictionary *) = nullptr;
    float (*vecToYaw)(float const *vector) = nullptr;
    void (*vecToAngles)(float const *vectorIn, float *vectorOut) = nullptr;
    void (*moveToOrigin)(common::entity::Dictionary *, float const *goal, float distance, int moveType) = nullptr;
    void (*changeYaw)(common::entity::Dictionary *) = nullptr;
    void (*changePitch)(common::entity::Dictionary *) = nullptr;
    common::entity::Dictionary * (*findEntityByString)(common::entity::Dictionary const *startSearchAfter, char const *field, char const *value) = nullptr;
    int (*getEntityIllum)(common::entity::Dictionary const *) = nullptr;
    common::entity::Dictionary * (*findEntityInSphere)(common::entity::Dictionary const *startSearchAfter, float const *org, float rad) = nullptr;
    common::entity::Dictionary * (*findClientInPVS)(common::entity::Dictionary const *player) = nullptr;
    common::entity::Dictionary * (*entitiesInPVS)(common::entity::Dictionary const *player) = nullptr;
    void (*makeVectors)(float const *vector) = nullptr;
    void (*angleVectors)(float const *vector, float *forward, float *right, float *up) = nullptr;
    common::entity::Dictionary * (*createEntity)(void) = nullptr;
    void (*removeEntity)(common::entity::Dictionary *) = nullptr;
    common::entity::Dictionary * (*createNamedEntity)(int name) = nullptr;
    void (*makeStatic)(common::entity::Dictionary *) = nullptr;
    int (*entIsOnFloor)(common::entity::Dictionary const *) = nullptr;
    int (*dropToFloor)(common::entity::Dictionary *) = nullptr;
    int (*walkMove)(common::entity::Dictionary *, float yaw, float distance, int iMode) = nullptr;
    void (*setOrigin)(common::entity::Dictionary *, float const *origin) = nullptr;
    void (*emitSound)(common::entity::Dictionary *, int channel, char const *sample, float volume, float attenuation, int fFlags, int pitch) = nullptr;
    void (*emitAmbientSound)(common::entity::Dictionary *, float *position, char const *samples, float volume, float attenuation, int flags, int pitch) = nullptr;
    void (*traceLine)(float const *v1, float const *v2, int noMonsters, common::entity::Dictionary const *skip, common::trace::Result *) = nullptr;
    void (*traceToss)(common::entity::Dictionary const *, common::entity::Dictionary const *ignore, common::trace::Result *) = nullptr;
    int (*traceMonsterHull)(common::entity::Dictionary const *, float const *v1, float const *v2, int noMonsters, common::entity::Dictionary const *skip, common::trace::Result *) = nullptr;
    void (*traceHull)(float const *v1, float const *v2, int noMonsters, int hull, common::entity::Dictionary const *skip, common::trace::Result *) = nullptr;
    void (*traceModel)(float const *v1, float const *v2, int hull, common::entity::Dictionary const *, common::trace::Result *) = nullptr;
    char const * (*traceTexture)(common::entity::Dictionary const *, float const *v1, float const *v2) = nullptr;
    void (*traceSphere)(float const *v1, float const *v2, int noMonsters, float radius, common::entity::Dictionary const *skip, common::trace::Result *) = nullptr;
    void (*getAimVector)(common::entity::Dictionary const *, float speed, float *returnValue) = nullptr;
    void (*serverCommand)(char const *) = nullptr;
    void (*serverExecute)(void) = nullptr;
    void (*clientCommand)(common::entity::Dictionary *, char *format, ...) = nullptr;
    void (*particleEffect)(float const *org, float const *dir, float color, float count) = nullptr;
    void (*lightStyle)(int style, char *value) = nullptr;
    int (*decalIndex)(char const *name) = nullptr;
    int (*pointContents)(float const *vector) = nullptr;
    void (*messageBegin)(int destination, int type, float const *origin, common::entity::Dictionary *) = nullptr;
    void (*messageEnd)(void) = nullptr;
    void (*writeByte)(int) = nullptr;
    void (*writeChar)(int) = nullptr;
    void (*writeShort)(int) = nullptr;
    void (*writeLong)(int) = nullptr;
    void (*writeAngle)(float) = nullptr;
    void (*writeCoord)(float) = nullptr;
    void (*writeString)(char const *) = nullptr;
    void (*writeEntity)(int) = nullptr;
    void (*cVarRegister)(common::console::Variable *) = nullptr;
    float (*cVarGetFloat)(char const *) = nullptr;
    char const * (*cVarGetString)(char const *) = nullptr;
    void (*cVarSetFloat)(char const *, float) = nullptr;
    void (*cVarSetString)(char const *name, char const *value) = nullptr;
    void (*alertMessage)(common::AlertType, char const *format, ...) = nullptr;
#ifdef HLSDK_3_2_OLD_EIFACE
    void (*engineFprintf)(FILE *file, char *format, ...) = nullptr;
    void * (*pvAllocEntPrivateData)(entity::Dictionary *, long) = nullptr;
#else
    void (*engineFprintf)(void *pfile, char *format, ...) = nullptr;
    void * (*pvAllocEntPrivateData)(common::entity::Dictionary *, common::Integer32) = nullptr;
#endif
    void * (*pvEntPrivateData)(common::entity::Dictionary *) = nullptr;
    void (*freeEntPrivateData)(common::entity::Dictionary *) = nullptr;
    char const * (*szFromIndex)(int) = nullptr;
    int (*allocString)(char const *) = nullptr;
    common::entity::Variables * (*getVarsOfEntity)(common::entity::Dictionary *) = nullptr;
    common::entity::Dictionary * (*entityOfEnttityOffset)(int) = nullptr;
    int (*entityOffsetOfEntity)(common::entity::Dictionary const *) = nullptr;
    int (*indexOfEntityDictionary)(common::entity::Dictionary const *) = nullptr;
    common::entity::Dictionary * (*pEntityOfEntityIndex)(int) = nullptr;
    common::entity::Dictionary * (*findEntityByVariables)(common::entity::Variables *) = nullptr;
    void * (*getModelPointer)(common::entity::Dictionary *) = nullptr;
    int (*regUserMsg)(char const *name, int size) = nullptr;
    void (*animationAutomove)(common::entity::Dictionary const *, float time) = nullptr;
    void (*getBonePosition)(common::entity::Dictionary const *, int bone, float *origin, float *angles) = nullptr;
#ifdef HLSDK_3_2_OLD_EIFACE
    unsigned long (*functionFromName)(char const *) = nullptr;
    char const * (*nameForFunction)(unsigned long) = nullptr;
#else
    common::UnsignedInteger32 (*functionFromName)(char const *) = nullptr;
    char const * (*nameForFunction)(common::UnsignedInteger32) = nullptr;
#endif
    void (*clientPrintf)(common::entity::Dictionary *, common::PrintType, char const *) = nullptr; // JOHN: engine callbacks so game DLL can print messages to individual clients
    void (*serverPrint)(char const *) = nullptr;
    char const * (*cmd_Args)(void) = nullptr; // these 3 added
    char const * (*cmd_Argv)(int) = nullptr;  // so game DLL can easily
    int (*cmd_Argc)(void) = nullptr;          // access client 'cmd' strings
    void (*getAttachment)(common::entity::Dictionary const *, int attachment, float *origin, float *angles) = nullptr;
    void (*crc32_Init)(common::Crc32 *) = nullptr;
    void (*crc32_ProcessBuffer)(common::Crc32 *, void *, int) = nullptr;
    void (*crc32_ProcessByte)(common::Crc32 *, unsigned char) = nullptr;
    common::Crc32 (*crc32_Final)(common::Crc32) = nullptr;
#ifdef HLSDK_3_2_OLD_EIFACE
    long (*randomLong)(long low, long high) = nullptr;
#else
    common::Integer32 (*randomLong)(common::Integer32 low, common::Integer32 high) = nullptr;
#endif
    float (*randomFloat)(float kow, float high) = nullptr;
    void (*setView)(common::entity::Dictionary const *client, common::entity::Dictionary const *target) = nullptr;
    float (*time)(void) = nullptr;
    void (*crosshairAngle)(common::entity::Dictionary const *client, float pitch, float yaw) = nullptr;
    common::Byte * (*loadFileForMe)(char *path, int *length) = nullptr;
    void (*freeFile)(void *) = nullptr;
    void (*endSection)(char const *) = nullptr; // trigger_endsection
    int (*compareFileTime)(char *path1, char *path2, int *compare) = nullptr;
    void (*getGameDir)(char *) = nullptr;
    void (*cvar_RegisterVariable)(common::console::Variable *) = nullptr;
    void (*fadeClientVolume)(common::entity::Dictionary const *, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) = nullptr;
    void (*setClientMaxspeed)(common::entity::Dictionary const *, float) = nullptr;
    common::entity::Dictionary * (*createFakeClient) (char const *name) = nullptr; // returns NULL if fake client can't be created
    void (*runPlayerMove) (common::entity::Dictionary *, float const *viewAngles, float forwardMove, float sideMove, float upMove, unsigned short buttons, common::Byte impulse, common::Byte msec) = nullptr;
    int (*numberOfEntities) (void) = nullptr;
    char * (*getInfoKeyBuffer)(common::entity::Dictionary *) = nullptr; // passing in NULL gets the serverinfo
    char * (*infoKeyValue)(char *buffer, char *key) = nullptr;
    void (*setKeyValue)(char *buffer, char *key, char *value) = nullptr;
    void (*setClientKeyValue)(int index, char *buffer, char *key, char *value) = nullptr;
    int (*isMapValid)(char *) = nullptr;
    void (*staticDecal)(float const *origin, int decal, int entity, int model) = nullptr;
    int (*precacheGeneric)(char *) = nullptr;
    int (*getPlayerUserId)(common::entity::Dictionary *) = nullptr; // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
    void (*buildSoundMsg)(common::entity::Dictionary *, int channel, char const *sample, /*int*/float volume, float attenuation, int flags, int pitch, int msgDest, int msgType, float const *origin, common::entity::Dictionary *) = nullptr;
    int (*isDedicatedServer)(void) = nullptr; // is this a dedicated server?
    common::console::Variable * (*cVarGetPointer)(char const *) = nullptr;
    unsigned int (*getPlayerWONId)(common::entity::Dictionary *) = nullptr; // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

// YWB 8/1/99 TFF Physics additions
    void (*info_RemoveKey)(char *, char const *key) = nullptr;
    char const * (*getPhysicsKeyValue)(common::entity::Dictionary const *client, char const *key) = nullptr;
    void (*setPhysicsKeyValue)(common::entity::Dictionary const *client, char const *key, char const *value) = nullptr;
    char const * (*getPhysicsInfoString)(common::entity::Dictionary const *client) = nullptr;
    unsigned short (*precacheEvent)(int type, char const *) = nullptr;
    void (*playbackEvent)(int flags, common::entity::Dictionary const *invoker, unsigned short index, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = nullptr;

    unsigned char * (*setFatPVS)(float *org) = nullptr;
    unsigned char * (*setFatPAS)(float *org) = nullptr;

    int (*checkVisibility)(common::entity::Dictionary const *, unsigned char *pset) = nullptr;

    void (*deltaSetField)(common::Delta *, char const *name) = nullptr;
    void (*deltaUnsetField)(common::Delta *, char const *name) = nullptr;
    void (*deltaAddEncoder)(char *name, void (*conditionalencode)(common::Delta *, const unsigned char *from, const unsigned char *to)) = nullptr;
    int (*getCurrentPlayer)(void) = nullptr;
    int (*canSkipPlayer)(common::entity::Dictionary const *) = nullptr;
    int (*deltaFindField)(common::Delta *, char const *name) = nullptr;
    void (*deltaSetFieldByIndex)(common::Delta *, int number) = nullptr;
    void (*deltaUnsetFieldByIndex)(common::Delta *, int number) = nullptr;

    void (*setGroupMask)(int mask, int op) = nullptr;

    int (*createInstancedBaseline)(int className, common::entity::State *baseLine) = nullptr;
    void (*cvar_DirectSet)(common::console::Variable *, char *) = nullptr;

    // Forces the client and server to be running with the same version of the specified file (e.g., a player model).
    // Calling this has no effect in single player
    void (*forceUnmodified)(common::ForceType, float *mins, float *maxs, char const *path) = nullptr;

    void (*getPlayerStats)(common::entity::Dictionary const *, int *ping, int *loss) = nullptr;

    void (*addServerCommand)(char *name, void (*function)(void)) = nullptr;

    // For voice communications, set which clients hear eachother.
    // NOTE: these functions take player entity indices (starting at 1).
    common::IntegerBoolean (*voice_GetClientListening)(int receiver, int sender) = nullptr;
    common::IntegerBoolean (*voice_SetClientListening)(int receiver, int sender, common::IntegerBoolean listen) = nullptr;

    char const * (*getPlayerAuthId)(common::entity::Dictionary *) = nullptr;

    // PSV: Added for CZ training map
    //char const *(*keyNameForBinding)(char const *pBinding);

    common::sequence::Entry * (*sequenceGet)(char const *file, char const *entry) = nullptr;
    common::sentence::Entry * (*sequencePickSentence)(char const *group, int pickMethod, int *picked) = nullptr;

    // LH: Give access to filesize via filesystem
    int (*getFileSize)(char *) = nullptr;

    unsigned int (*getApproxWavePlayLen)(char const *path) = nullptr;
    // MDC: Added for CZ career-mode
    int (*isCareerMatch)(void) = nullptr;

    // BGC: return the number of characters of the localized string referenced by using "label"
    int (*getLocalizedStringLength)(char const *label) = nullptr;

    // BGC: added to facilitate persistent storage of tutor message decay values for
    // different career game profiles.  Also needs to persist regardless of mp.dll being
    // destroyed and recreated.
    void (*registerTutorMessageShown)(int mid) = nullptr;
    int (*getTimesTutorMessageShown)(int mid) = nullptr;
    void (*processTutorMessageDecayBuffer)(int *, int length) = nullptr;
    void (*constructTutorMessageDecayBuffer)(int *, int length) = nullptr;
    void (*resetTutorMessageDecayData)(void) = nullptr;

    // Added 2005/08/11 (no SDK update):
    void (*queryClientCvarValue)(common::entity::Dictionary const *, char const *) = nullptr;

    // Added 2005/11/21 (no SDK update):
    void (*queryClientCvarValue2)(common::entity::Dictionary const *, char const *, int requestId) = nullptr;

    // Added 2009/06/19 (no SDK update):
    int (*engCheckParm)(char const *token, char **next) = nullptr;
};

} // namespace p5::lambda::engine


#include "engine+inl.hxx"

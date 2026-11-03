#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>
#include <pl/Input.hpp>
#include <pl/Config.hpp>
#include <pl/memory/Hook.hpp>
#include <mc/world/actor/player/Player.h>
#include <mc/world/actor/Actor.h>
#include <mc/deps/core/math/Vec2.h>
#include <mc/deps/core/math/Vec3.h>
#include <cmath>
#include <algorithm>

class CodexAim {
public:
    bool aimEnabled = true;
    bool autoJumpEnabled = false;
    float fov = 20.0f;
    float smooth = 0.15f;
    float sensitivity = 0.5f;
    float maxTurnSpeed = 2.5f;

    static float WrapDegrees(float deg) {
        float r = std::fmod(deg, 360.0f);
        if (r >= 180.0f) r -= 360.0f;
        if (r < -180.0f) r += 360.0f;
        return r;
    }

    Vec2 QuantizeToGCD(float targetYaw, float targetPitch, float curYaw, float curPitch) {
        float f = sensitivity * 0.6f + 0.2f;
        float gcd = f * f * f * 1.2f;
        float deltaYaw = WrapDegrees(targetYaw - curYaw);
        float deltaPitch = targetPitch - curPitch;
        float stepsYaw = std::round(deltaYaw / gcd);
        float stepsPitch = std::round(deltaPitch / gcd);
        return { WrapDegrees(curYaw + stepsYaw * gcd), std::clamp(curPitch + stepsPitch * gcd, -89.9f, 89.9f) };
    }

    void onTick(Player& player) {
        Vec3 myPos = player.getPosition();
        Vec2 myRot = player.getRotation();

        if (aimEnabled) {
            auto& level = player.getLevel();
            auto entities = level.getRuntimeActorList();

            Actor* bestTarget = nullptr;
            float bestAngle = 999.0f;
            Vec3 targetAimPos;

            for (Actor* ent : entities) {
                if (ent == &player || !ent->isPlayer()) continue;
                if (ent->getHealth() <= 0) continue;

                Vec3 entPos = ent->getPosition();
                float dist = myPos.distance(entPos);
                if (dist > 3.5f) continue;

                Vec3 aimPos = { entPos.x, entPos.y + 0.9f, entPos.z };
                float dx = aimPos.x - myPos.x;
                float dy = aimPos.y - (myPos.y + 1.6f);
                float dz = aimPos.z - myPos.z;
                
                float targetYaw = WrapDegrees((std::atan2(dz, dx) * 180.0f / 3.14159265f) - 90.0f);
                float targetPitch = -(std::atan2(dy, std::sqrt(dx*dx + dz*dz)) * 180.0f / 3.14159265f);

                float yawDiff = std::abs(WrapDegrees(myRot.x - targetYaw));
                if (yawDiff > fov) continue;

                float totalAngle = yawDiff + std::abs(myRot.y - targetPitch);
                if (totalAngle < bestAngle) {
                    bestAngle = totalAngle;
                    bestTarget = ent;
                    targetAimPos = aimPos;
                }
            }

            if (bestTarget) {
                float dx = targetAimPos.x - myPos.x;
                float dy = targetAimPos.y - (myPos.y + 1.6f);
                float dz = targetAimPos.z - myPos.z;
                float targetYaw = WrapDegrees((std::atan2(dz, dx) * 180.0f / 3.14159265f) - 90.0f);
                float targetPitch = -(std::atan2(dy, std::sqrt(dx*dx + dz*dz)) * 180.0f / 3.14159265f);

                float stepYaw = WrapDegrees(targetYaw - myRot.x) * smooth;
                float stepPitch = (targetPitch - myRot.y) * smooth;

                stepYaw = std::clamp(stepYaw, -maxTurnSpeed, maxTurnSpeed);
                stepPitch = std::clamp(stepPitch, -maxTurnSpeed, maxTurnSpeed);

                Vec2 safeRot = QuantizeToGCD(myRot.x + stepYaw, myRot.y + stepPitch, myRot.x, myRot.y);
                player.setRotation(Vec2(safeRot.x, safeRot.y));
            }
        }

        if (autoJumpEnabled) {
            if (player.isOnGround()) {
                player.jump();
            }
        }
    }
};

// Регистрация мода через API PreLoader
PL_REGISTER_MOD(CodexAim, [](auto& mod) {
    mod.load = []() { return true; };
    mod.enable = []() { return true; };
    mod.disable = []() { return true; };
    // Здесь можно подписаться на события через pl::API
});
 #include <pl/Mod.hpp>
#include <pl/api/Event.hpp>
#include <pl/api/Player.hpp>
#include <mc/world/actor/player/Player.h>

class CodexAim {
public:
    // Этот метод будет вызываться каждый тик игры
    void onTick(Player& player) {
        // Тут будет логика AimAssist. 
        // Пока оставляем пустым, чтобы чит собрался и запустился.
    }
};

// Обязательная регистрация мода для Codex Client / PreLoader
PL_REGISTER_MOD(CodexAim, "CodexAim", "arest");
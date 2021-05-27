#ifndef _DUELRESET_H_
#define _DUELRESET_H_

#include "Common.h"
#include "Player.h"
#include "Pet.h"
#include "SpellInfo.h"
#include "Config.h"
#include <unordered_map>

class DuelReset
{
public:
    static DuelReset* instance();

    void LoadConfig(bool reload);

    void ResetSpellCooldowns(Player* player, bool onStartDuel);
    void SaveCooldownStateBeforeDuel(Player* player);
    void RestoreCooldownStateAfterDuel(Player* player);

    void SaveHealthBeforeDuel(Player* player);
    void SaveManaBeforeDuel(Player* player);
    void RestoreHealthAfterDuel(Player* player);
    void RestoreManaAfterDuel(Player* player);

    bool GetResetCooldownsEnabled() const;
    bool GetResetHealthEnabled() const;
private:
    // Config values
    bool m_enableCooldowns;
    bool m_enableHealth;

    // Player value maps
    typedef std::unordered_map<Player*, SpellCooldowns> PlayersCooldownMap;
    typedef std::unordered_map<Player*, uint32> PlayersHealthMap;
    typedef std::unordered_map<Player*, uint32> PlayersManaMap;

    PlayersCooldownMap m_spellCooldownsBeforeDuel;
    PlayersHealthMap m_healthBeforeDuel;
    PlayersManaMap m_manaBeforeDuel;
};

#define sDuelReset DuelReset::instance()

#endif

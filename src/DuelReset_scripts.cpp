/*
 *  Originally written  for TrinityCore by ShinDarth and GigaDev90 (www.trinitycore.org)
 *  Converted as module for AzerothCore by ShinDarth and Yehonal   (www.azerothcore.org)
 *  Reworked by Gozzim
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Pet.h"
#include "SpellInfo.h"
#include "Config.h"
#include "DuelReset.h"

class DuelResetScript : public PlayerScript {
public:
    DuelResetScript() : PlayerScript("DuelResetScript") {}

    // Called when a duel starts (after 3s countdown)
    void OnDuelStart(Player *player1, Player *player2) override {
        // Cooldowns reset
        if (sConfigMgr->GetBoolDefault("DuelResetCooldowns", true)) {
            sDuelReset->SaveCooldownStateBeforeDuel(player1);
            sDuelReset->SaveCooldownStateBeforeDuel(player2);

            sDuelReset->ResetSpellCooldowns(player1, true);
            sDuelReset->ResetSpellCooldowns(player2, true);
        }

        // Health and mana reset
        if (sConfigMgr->GetBoolDefault("DuelResetHealthMana", true)) {
            sDuelReset->SaveHealthBeforeDuel(player1);
            if (player1->getPowerType() == POWER_MANA || player1->getClass() == CLASS_DRUID) {
                sDuelReset->SaveManaBeforeDuel(player1);
            }
            player1->ResetAllPowers();

            sDuelReset->SaveHealthBeforeDuel(player2);
            if (player2->getPowerType() == POWER_MANA || player2->getClass() == CLASS_DRUID) {
                sDuelReset->SaveManaBeforeDuel(player2);
            }
            player2->ResetAllPowers();
        }
    }

    // Called when a duel ends
    void OnDuelEnd(Player *winner, Player *loser, DuelCompleteType type) override {
        // do not reset anything if DUEL_INTERRUPTED or DUEL_FLED
        if (type == DUEL_WON) {
            // Cooldown restore
            if (sConfigMgr->GetBoolDefault("DuelResetCooldowns", true)) {
                sDuelReset->ResetSpellCooldowns(winner, false);
                sDuelReset->ResetSpellCooldowns(loser, false);

                sDuelReset->RestoreCooldownStateAfterDuel(winner);
                sDuelReset->RestoreCooldownStateAfterDuel(loser);
            }

            // Health and mana restore
            if (sConfigMgr->GetBoolDefault("DuelResetHealthMana", true)) {
                sDuelReset->RestoreHealthAfterDuel(winner);
                sDuelReset->RestoreHealthAfterDuel(loser);

                // check if player1 class uses mana
                if (winner->getPowerType() == POWER_MANA || winner->getClass() == CLASS_DRUID)
                    sDuelReset->RestoreManaAfterDuel(winner);

                // check if player2 class uses mana
                if (loser->getPowerType() == POWER_MANA || loser->getClass() == CLASS_DRUID)
                    sDuelReset->RestoreManaAfterDuel(loser);
            }
        }
    }
};

void AddSC_DuelReset()
{
    new DuelResetScript();
}
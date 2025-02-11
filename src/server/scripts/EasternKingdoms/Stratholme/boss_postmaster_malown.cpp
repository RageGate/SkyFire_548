/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: boss_postmaster_malown
SD%Complete: 50
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

//Spell ID to summon this guy is 24627 "Summon Postmaster Malown"
//He should be spawned along with three other elites once the third postbox has been opened

enum Says
{
    SAY_KILL                    = 0
};

enum Spells
{
    SPELL_WAILINGDEAD           = 7713,
    SPELL_BACKHAND              = 6253,
    SPELL_CURSEOFWEAKNESS       = 8552,
    SPELL_CURSEOFTONGUES        = 12889,
    SPELL_CALLOFTHEGRAVE        = 17831
};

enum Events
{
    EVENT_WAILINGDEAD          = 1,
    EVENT_BACKHAND             = 2,
    EVENT_CURSEOFWEAKNESS      = 3,
    EVENT_CURSEOFTONGUES       = 4,
    EVENT_CALLOFTHEGRAVE       = 5
};

class boss_postmaster_malown : public CreatureScript
{
public:
    boss_postmaster_malown() : CreatureScript("boss_postmaster_malown") { }

    struct boss_postmaster_malownAI : public BossAI
    {
        boss_postmaster_malownAI(Creature* creature) : BossAI(creature, TYPE_MALOWN) { }

        void Reset() OVERRIDE { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_WAILINGDEAD, 19000);     // lasts 6 sec
            events.ScheduleEvent(EVENT_BACKHAND, 8000);         // 2 sec stun
            events.ScheduleEvent(EVENT_CURSEOFWEAKNESS, 20000); // lasts 2 mins
            events.ScheduleEvent(EVENT_CURSEOFTONGUES, 22000);
            events.ScheduleEvent(EVENT_CALLOFTHEGRAVE, 25000);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_KILL);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WAILINGDEAD:
                        if (rand()%100 < 65) //65% chance to cast
                            DoCastVictim(SPELL_WAILINGDEAD, true);
                        events.ScheduleEvent(EVENT_WAILINGDEAD, 19000);
                        break;
                    case EVENT_BACKHAND:
                        if (rand()%100 < 45) //45% chance to cast
                            DoCastVictim(SPELL_BACKHAND, true);
                        events.ScheduleEvent(EVENT_WAILINGDEAD, 8000);
                        break;
                    case EVENT_CURSEOFWEAKNESS:
                        if (rand()%100 < 3) //3% chance to cast
                            DoCastVictim(SPELL_CURSEOFWEAKNESS, true);
                        events.ScheduleEvent(EVENT_WAILINGDEAD, 20000);
                        break;
                    case EVENT_CURSEOFTONGUES:
                        if (rand()%100 < 3) //3% chance to cast
                            DoCastVictim(SPELL_CURSEOFTONGUES, true);
                        events.ScheduleEvent(EVENT_WAILINGDEAD, 22000);
                        break;
                    case EVENT_CALLOFTHEGRAVE:
                        if (rand()%100 < 5) //5% chance to cast
                            DoCastVictim(SPELL_CALLOFTHEGRAVE, true);
                        events.ScheduleEvent(EVENT_WAILINGDEAD, 25000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_postmaster_malownAI(creature);
    }
};

void AddSC_boss_postmaster_malown()
{
    new boss_postmaster_malown();
}

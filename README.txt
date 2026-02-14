26-01-24

After a 5-6 year break, getting back into EQing. Dusting off old Autobot/RaidDruid to get it back into shape as I find issues. Managed to get it working with minor tweaks.

Moving to github for better collaboration/updating.

Ensure you put the RDCommon.ini file into the "config" folder of MQ.
If using one of the example INI, ensure you put them in the "config" folder of MQ.
Editing out "trade" function until it can be repaired. 
Switched to using MQ2Cast_Spell_Routines.inc. This means the plugin MQ2Cast is required. Put it in the "macros" folder of MQ.
If you wish to keep using Spell_Routines.inc, find and replace "/call MQ2Cast" with "/call cast".
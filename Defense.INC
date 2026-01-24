|**	Defense.inc V2.3
	Updated 20-04-28 (add ToV stuff)

	 NecroGnomie:
		This include is ment to help automate defensive measures for various classes. There is no INI file to go with this,
			this is simply my version of what each class ought to be doing as each line of defense goes down. 
			Currently set up for use with AA, disc's, and BP clicks up to level 110.
			What this means is the include file should detect what disc/AA you have and click on it as "/call Defense" is activated. 
			It checks if the lowest versionof X line is available (no timer) And then starts with the highest version and works it's
			way down the list for that line, until it finds the highest rendition the toon has, and activates it.
		

	Someplace in your main loop put some sort of call, for example:
		"/if (!${Me.ActiveDisc.ID} && ${Me.CombatState.Equal[COMBAT]} && ${Select[${Me.Class.ShortName},WAR,PAL,SHD]} && (${Target.Named} || ${Me.XTarget}>2 || ${Me.PctHPs}<60) && ${Target.Distance}<75) /call Defense"

	Requires:
	Spell_Routines.inc
	

	This is set up to work with RaidDruid.mac, so it utilizes a ${ChatChannel} to make announcements. It also uses a message sub similar to:

	Sub AnnounceMessage(Channel, EventTotal, Color1, Event1, Color2, Event2, Color3, Event3, Color4, Event4, Color5, Event5,)
		/if (${Channel.Equal[BC]} && ${EventTotal.Equal[1]}) /BC [+${Color1}+] ${Event1}
		/if (${Channel.Equal[BC]} && ${EventTotal.Equal[2]}) /BC [+${Color1}+] ${Event1} [+${Color2}+] ${Event2}
		/if (${Channel.Equal[BC]} && ${EventTotal.Equal[3]}) /BC [+${Color1}+] ${Event1} [+${Color2}+] ${Event2} [+${Color3}+] ${Event3}
		/if (${Channel.Equal[BC]} && ${EventTotal.Equal[4]}) /BC [+${Color1}+] ${Event1} [+${Color2}+] ${Event2} [+${Color3}+] ${Event3} [+${Color4}+] ${Event4}
		/if (${Channel.Equal[BC]} && ${EventTotal.Equal[5]}) /BC [+${Color1}+] ${Event1} [+${Color2}+] ${Event2} [+${Color3}+] ${Event3} [+${Color4}+] ${Event4} ${Event5}
		/if (${Channel.NotEqual[BC]} && ${EventTotal.Equal[1]}) /${Channel} ${Event1}
		/if (${Channel.NotEqual[BC]} && ${EventTotal.Equal[2]}) /${Channel} ${Event1} ${Event2} 
		/if (${Channel.NotEqual[BC]} && ${EventTotal.Equal[3]}) /${Channel} ${Event1} ${Event2} ${Event3} 
		/if (${Channel.NotEqual[BC]} && ${EventTotal.Equal[4]}) /${Channel} ${Event1} ${Event2} ${Event3} ${Event4}
		/if (${Channel.NotEqual[BC]} && ${EventTotal.Equal[5]}) /${Channel} ${Event1} ${Event2} ${Event3} ${Event4} ${Event5}
		/return

	In RD type in "/reportdefense" to toggle, else add the declare to your current macro.
		EX: /declare ReportDefense 		bool 	outer

		If Defense.inc finds no "ReportDefense" variable, it will make one and default it to FALSE
		If Defense.inc finds no "ChatChannel" variable, it will make one and default it to Echo
**|
	
Sub Defense
	/if (!${Defined[ReportDefense]}) /declare ReportDefense bool outer FALSE
	/if (!${Defined[ChatChannel]}) /declare ChatChannel string outer echo
	/if (${Me.Class.ShortName.Equal[WAR]}) /call WarDefense
	/if (${Me.Class.ShortName.Equal[PAL]}) /call PalDefense
	/if (${Me.Class.ShortName.Equal[SHD]}) /call SKDefense
	/return

Sub WarDefense
	/if (!${Me.ActiveDisc.ID}) {
		/if (${Me.PctEndurance}>5) {
			/if (${Me.CombatAbilityReady[${Spell[Dichotomic Shield].RankName}]}) {
				/disc ${Spell[${Spell[Dichotomic Shield].RankName}].ID}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Dichotomic Shield" r "Active"
				/return		
				}
			/if (!${Me.CombatAbilityReady[${Spell[Dichotomic Shield].RankName}]}) {
				/if (${Me.CombatAbilityReady[${Spell[Defensive Discipline].RankName}]}) {
					/if (${Spell[${Spell[Culminating Stand Discipline].RankName}].ID}) {
						/disc ${Spell[Ultimate Stand Discipline].RankName}
						/goto :WARStandEnd
						}
					/if (${Spell[${Spell[Culminating Stand Discipline].RankName}].ID}) {
						/disc ${Spell[Culminating Stand Discipline].RankName}
						/goto :WARStandEnd
						}
					/if (${Spell[${Spell[Last Stand Discipline].RankName}].ID}) {
						/disc ${Spell[Last Stand Discipline].RankName}
						/goto :WARStandEnd
						}
					/if (${Spell[${Spell[Final Stand Discipline].RankName}].ID}) {
						/disc ${Spell[Final Stand Discipline].RankName}
						/goto :WARStandEnd
						}
					/if (${Spell[${Spell[Defensive Discipline].RankName}].ID}) {
						/disc ${Spell[Defensive Discipline].RankName}
						/goto :WARStandEnd
						}						
					:WARStandEnd
					/if (${ReportDefense}) /call AnnounceMessage 2 y "Stand Discipline" r "Active"
					/return
					}
				}
			}
		/if (${Cast.Ready[${Me.Inventory[chest]}]}) {
			/call Cast "${Me.Inventory[chest]}" Item		
			/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Clicking BP" r "Active"!
			/return
			}
		/if (${Me.CombatAbilityReady[${Spell[No Time To Bleed].RankName}]} && !${Me.CombatAbilityReady[${Spell[Defensive Discipline].RankName}]} && !${Me.CombatAbilityReady[${Spell[Dichotomic Shield].RankName}]}) {
			/if (${Me.CombatAbilityReady[${Spell[Pain Doesn't Hurt].RankName}]}) {
				/disc ${Spell[${Spell[Pain Doesn't Hurt].RankName}].ID}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Pain Doesn't Hurt" r "Active"
				/return
				}
			/if (${Me.CombatAbilityReady[${Spell[No Time To Bleed].RankName}]}) {
				/disc ${Spell[${Spell[No Time To Bleed].RankName}].ID}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "No Time To Bleed" r "Active"
				/return
				}
			}
		/if (!${Me.CombatAbilityReady[${Spell[No Time To Bleed].RankName}]} && !${Me.CombatAbilityReady[${Spell[Defensive Discipline].RankName}]} && ${Me.CombatAbilityReady[${Spell[Armor of Draconic Runes].RankName}]} && ${Me.PctEndurance}>5) {
			/if (${Spell[${Spell[Armor of Relentless Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Relentless Runes].RankName}
				/goto :WARRuneEnd
				}			
			Armor of Relentless Runes
			/if (${Spell[${Spell[Armor of Tenacious Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Tenacious Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Darkened Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Darkened Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Stalwart Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Stalwart Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Mystical Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Mystical Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Phantasmic Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Phantasmic Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Timeworn Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Timeworn Runes].RankName}
				/goto :WARRuneEnd
				}
			/if (${Spell[${Spell[Armor of Draconic Runes].RankName}].ID}) {
				/disc ${Spell[Armor of Draconic Runes].RankName}
				/goto :WARRuneEnd
				}				
			:WARRuneEnd
			/if (${ReportDefense}) /call AnnounceMessage 2 y "Runes Discipline" r "Active"
			/return
			}			
		/if (!${Me.CombatAbilityReady[${Spell[No Time To Bleed].RankName}]} && !${Me.CombatAbilityReady[${Spell[Defensive Discipline].RankName}]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal[No Effect]}) {
			/if (${Me.AltAbilityReady[Warlord's Bravery]}) {
				/call Cast "Warlord's Bravery" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Warlord's_Bravery" r "Active"
				/return	
				}
			/if (${Me.AltAbilityReady[Warlord's Tenacity]}) {
				/call Cast "Warlord's Tenacity" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Warlords_Tenacity" r "Active"
				/return	
				}				
			/if (${Me.AltAbilityReady[Brace for Impact]}) {
				/call Cast "Brace for Impact" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Brace_For_Impact" r "Active"		
				/return	
				}				
			/if (${Me.AltAbilityReady[Armor of Experience]}) {
				/call Cast "Armor of Experience" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor_of_Experience" r "Active"
				/return			
				}				
			/if (${Me.AltAbilityReady[Spire of the Warlord]}) {
				/call Cast " Third Spire of the Warlord" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Third_Spire" r "Active"
				/return			
				}				
			}			
		/if (${Me.AltAbilityReady[Blade Guardian]}) {
			/call Cast "Blade Guardian" ALT
			/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Blade Guardian" r "Active"
			/return
			}	
		/if (${Me.AltAbilityReady[Hold the Line]}) {
			/call Cast "Hold the Line" ALT
			/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Hold_the_Line" r "Active"
			/return		
			} else {
			/if (${Me.AltAbilityReady[Resplendent Glory]}) {
				/call Cast "Resplendant Glory" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Respendant_Glory" r "Active"
				/return
				}
			}	
		/if (${Me.AltAbilityReady[Warlord's Resurgence]}) {
			/call Cast "Warlord's Resurgence" ALT
			/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Warlord's_Resurgence" r "Active"
			/return
			}
		}
	/return	

Sub PalDefense
	/if (!${Me.ActiveDisc.ID}) {
		/if (${Me.AltAbilityReady[Group Armor of the Inquisitor]}) {
			/call Cast "Group Armor of the Inquisitor" ALT
			/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Group Armor of the Inquisiton" r "Active"
			/return				
			}
		/if (${Me.PctEndurance}>5) {
			/if (${Me.CombatAbilityReady[${Spell[Holy Guardian Discipline].RankName}]}) {
				/if (${Spell[${Spell[Blessed Guardian Discipline].RankName}].ID}) {
					/disc ${Spell[${Spell[Blessed Guardian Discipline].RankName}].ID}
					} Else {
					/disc ${Spell[${Spell[Holy Guardian Discipline].RankName}].ID}
					}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Guardian Discipline, Defense line 1" r "Active"
				/return				
				}
			/if (!${Me.CombatAbilityReady[${Spell[Holy Guardian Discipline].RankName}]} && ${Me.CombatAbilityReady[${Spell[Honorific Mantle].RankName}]}) {		
				/if (${Spell[${Spell[Mantle of the Sapphire Cohort].RankName}].ID}) {
					/disc ${Spell[Mantle of the Sapphire Cohort].RankName}
					/goto :PALMantleEnd
					}			
				/if (${Spell[${Spell[Kar'Zok Mantle].RankName}].ID}) {
					/disc ${Spell[Kar'Zok Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Skalber Mantle].RankName}].ID}) {
					/disc ${Spell[Skalber Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Brightwing Mantle].RankName}].ID}) {
					/disc ${Spell[Brightwing Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Prominent Mantle].RankName}].ID}) {
					/disc ${Spell[Prominent Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Honorific Mantle].RankName}].ID}) {
					/disc ${Spell[Honorific Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Armor of Endless Honor].RankName}].ID}) {
					/disc ${Spell[Armor of Endless Honor].RankName}
					/goto :PALMantleEnd
					}					
				:PALMantleEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Mantle Discipline, Defense line 2" r "Active"	
				/return
				}
			/if (!${Me.CombatAbilityReady[${Spell[Honorific Mantle].RankName}]} && ${Me.CombatAbilityReady[${Spell[Armor of Courage].RankName}]}) {	
				/if (${Spell[${Spell[Armor of Sincerity].RankName}].ID}) {
					/disc ${Spell[Armor of Sincerity].RankName}
					/goto :PALArmorEnd
					}			
				/if (${Spell[${Spell[Armor of Mercy].RankName}].ID}) {
					/disc ${Spell[Armor of Mercy].RankName}
					/goto :PALArmorEnd
					}		
				/if (${Spell[${Spell[Devout Penitence].RankName}].ID}) {
					/disc ${Spell[Devout Penitence].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Reverent Penitence].RankName}].ID}) {
					/disc ${Spell[Reverent Penitence].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Armor of Zeal].RankName}].ID}) {
					/disc ${Spell[Armor of Zeal].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Armor of Courage].RankName}].ID}) {
					/disc ${Spell[Armor of Courage].RankName}
					/goto :PALArmorEnd
					}		
				:PALArmorEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Penitence Discipline, Defense line 3" r "Active"
				/return 		
				}
			}
		/if (!${Me.CombatAbilityReady[${Spell[Armor of Courage].RankName}]}) {
			/if (${Me.AltAbilityReady[Armor of Experience]}) {
				/call Cast "Armor of Experience" ALT
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience" r "Active"
				/return				
				}
			/if (${Me.AltAbilityReady[ Third Spire of Holiness]}) {
				/call Cast " Third Spire of Holiness" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "First Spire" r "Active"
				/return			
				}
			}
	}
	/return

Sub SKDefense
	/if (!${Me.ActiveDisc.ID}) {
		/if (${Me.PctEndurance}>5) {
			/if (${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]}) {
				/if (${Spell[${Spell[Cursed Guardian Discipline].RankName}].ID}) {
					/disc ${Spell[${Spell[Cursed Guardian Discipline].RankName}].ID}
					} Else {
					/disc ${Spell[${Spell[Unholy Guardian Discipline].RankName}].ID}
					}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Guardian Discipline, Defense line 1" r "Active"
				/return				
				}
			/if (!${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]} && ${Me.CombatAbilityReady[${Spell[Ichor Guard].RankName}]}) {
				/if (${Spell[${Spell[Restless Mantle].RankName}].ID}) {
					/disc ${Spell[Restless Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Krellnakor Mantle].RankName}].ID}) {
					/disc ${Spell[Krellnakor Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Doomscale Mantle].RankName}].ID}) {
					/disc ${Spell[Doomscale Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Bonebrood Mantle].RankName}].ID}) {
					/disc ${Spell[Bonebrood Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Gorgon Mantle].RankName}].ID}) {
					/disc ${Spell[Gorgon Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Malarian Mantle].RankName}].ID}) {
					/disc ${Spell[Malarian Mantle].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Umbral Carapace].RankName}].ID}) {
					/disc ${Spell[Umbral Carapace].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Soul Carapace].RankName}].ID}) {
					/disc ${Spell[Soul Carapace].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Soul Shield].RankName}].ID}) {
					/disc ${Spell[Soul Shield].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Soul Guard].RankName}].ID}) {
					/disc ${Spell[Soul Guard].RankName}
					/goto :SKMantleEnd
					}
				/if (${Spell[${Spell[Ichor Guard].RankName}].ID}) {
					/disc ${Spell[Ichor Guard].RankName}
					/goto :SKMantleEnd
					}					
				:SKMantleEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "SK Mantle Defense, Defense line 2 r "Active"
				/return
				}
			/if (!${Me.CombatAbilityReady[${Spell[Ichor Guard].RankName}]} && ${Me.CombatAbilityReady[${Spell[Gorgon Carapace].RankName}]}) {
				/if (${Spell[${Spell[Cadcane's Carapace].RankName}].ID}) {
					/disc ${Spell[Cadcane's Carapace].RankName}
					/goto :SKArmorEnd
					}			
				/if (${Spell[${Spell[Tylix's Carapace].RankName}].ID}) {
					/disc ${Spell[Tylix's Carapace].RankName}
					/goto :SKArmorEnd
					}
				/if (${Spell[${Spell[Vizat's Carapace].RankName}].ID}) {
					/disc ${Spell[Vizat's Carapace].RankName}
					/goto :SKArmorEnd
					}
				/if (${Spell[${Spell[Grelleth's Carapace].RankName}].ID}) {
					/disc ${Spell[Grelleth's Carapace].RankName}
					/goto :SKArmorEnd
					}
				/if (${Spell[${Spell[Sholothian Carapace].RankName}].ID}) {
					/disc ${Spell[Sholothian Carapace].RankName}
					/goto :SKArmorEnd
					}
				/if (${Spell[${Spell[Gorgon Carapace].RankName}].ID}) {
					/disc ${Spell[Gorgon Carapace].RankName}
					/goto :SKArmorEnd
					}					
				:SKArmorEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Carapace Discipline, Defense line 3" r "Active"
				/return				
				}
			}
		/if (!${Me.CombatAbilityReady[${Spell[Gorgon Mantle].RankName}]}) {
			/if (${Me.AltAbilityReady[Spire of the Reaver]}) {
				/call Cast "Spire of the Reaver" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Third Spire" r "Active"
				/return			
				}
			/if (!${Me.AltAbilityReady[Spire of the Reaver]} && ${Me.AltAbilityReady[Armor of Experience]}) {
				/call Cast "Armor of Experience" ALT
				/if (${castReturn.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience"  r "Active"
				/return			
				}	
		}
	}
/return
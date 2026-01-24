|Wait4rez.inc v2.22
Sub Wait4Rez
/echo Awaiting rez: auto-accepting, and looting my corpse
	/call CreateTimer RezTimer
	/if (${Me.State.Equal[HOVER]}) /varset RezTimer 180s
	:rezmeffs
	/doevents
	/delay 10s ${Window[ConfirmationDialogBox].Open}
|	/docommand /${ChatChannel} rez me pls
	/if (!${Window[ConfirmationDialogBox].Open} && ${RezTimer}>0) /goto :rezmeffs
	/if (!${Window[ConfirmationDialogBox].Open} && !${RezTimer}) {
		/nomodkey /notify RespawnWnd RW_OptionsList listselect 1 
		/delay 1s
		/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
		/delay 2s
		}
	/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/delay 2s
	/if (${Window[RespawnWnd].Open}) {
		/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
		/delay 1s
		/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
		/delay 2s 
		}
	/if (${Me.State.Equal[HOVER]}) /goto :rezmeffs
	/return

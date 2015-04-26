#include <YSI\y_hooks>

#define PHONESYS

#define PHONE_STATE_NONE            (1)
#define PHONE_STATE_OFF             (2)
#define PHONE_STATE_ON              (3)
#define PHONE_STATE_CALLING         (4)
#define PHONE_STATE_PICKEDUP        (5)
#define PHONE_STATE_HANGUP          (6)

// Dialogs

#define DIALOG_REGISTER             (7)
#define DIALOG_LOGIN                (8)


native WP_Hash(buffer[], len, const str[]);



enum e_PHONEINFO {
        p_Ownership,
        p_Number, 
        p_State,
};
//When you use static instead of new, the array is visible only in this particular file. It's useful, but you want player info available everywhere
new stock 
	p_Info[MAX_PLAYERS][e_PHONEINFO],
	p_OnPhone[MAX_PLAYERS] = { INVALID_PLAYER_ID, ... }
;


hook OnGameModeInit() 
{
	
//	Command_AddAltNamed("pickup", "p");
//	Command_AddAltNamed("hangup", "h");

	Command_AddAltNamed("whisper", "w");
   	Command_AddAltNamed("shout", "s");

	return 1;
}

hook OnPlayerConnect(playerid)
{
	p_Info[playerid][p_Ownership] = 0;
	p_Info[playerid][p_State] = PHONE_STATE_NONE;
}


CMD:buyphone(playerid, params[]) {
    new string[52];
    if(p_Info[playerid][p_Ownership] == 1) return SendClientMessage(playerid, LIGHT_GRAY, "You have already bought a phone.");
    if(GetPlayerMoney(playerid) < 750) return SendClientMessage(playerid, LIGHT_GRAY, "You do not have enough money to purchase a cellphone.");
    
    
    else {
	    p_Info[playerid][p_Ownership] = 1;
        p_Info[playerid][p_Number] = GenerateRandomPhoneNumber();
	    GivePlayerMoney(playerid, -750);
	    format(string, sizeof(string), "You have bought a phone. Your number is: %d.", p_Info[playerid][p_Number]);
	    SendClientMessage(playerid, LIGHT_GRAY, string);
    }
    return 1;
}
new PhoneRingingTimer[MAX_PLAYERS];
CMD:call(playerid, params[]) {
	new number;
    if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /call [number]");
    if(p_Info[playerid][p_Ownership] == 0) return SendClientMessage(playerid, LIGHT_GRAY, "You do not have a phone.");
    if(sscanf(params, "i", number)) return SendClientMessage(playerid, LIGHT_GRAY, "You did not enter a valid number.");
 	if(number == p_Info[playerid][p_Number]) return SendClientMessage(playerid, LIGHT_GRAY, "You cannot call yourself.");
	else {
		switch(p_Info[playerid][p_State]) {
			case PHONE_STATE_OFF: return SendClientMessage(playerid, LIGHT_GRAY, "Your phone is off.");
			case PHONE_STATE_PICKEDUP: return SendClientMessage(playerid, LIGHT_GRAY, "You are in a call.");
			case PHONE_STATE_CALLING: return SendClientMessage(playerid, LIGHT_GRAY, "You are already calling someone.");
		}
	}
    foreach(new i: Player) {
        new string[128];
        if(number == p_Info[i][p_Number]) {
            switch(p_Info[i][p_State]) {
      			case PHONE_STATE_OFF: return SendClientMessage(playerid, LIGHT_GRAY, "The player's phone is off.");
         		case PHONE_STATE_PICKEDUP: return SendClientMessage(playerid, LIGHT_GRAY, "Busy line.");
			}
			p_Info[i][p_State] = PHONE_STATE_CALLING;
			p_Info[playerid][p_State] = PHONE_STATE_CALLING;
			if (p_Info[playerid][p_State] == PHONE_STATE_CALLING) {
				SendClientMessage(playerid, LIGHT_GRAY, "You are calling someone. Use '/hangup' to hang up your phone.");
			}
			if(p_Info[i][p_State] == PHONE_STATE_CALLING) {
			    format(string, sizeof(string), "You are being called by number: %d. Use {EE7181}'/(p)ickup'{C0C0C0} or {EE7181}'/(h)angup'{C0C0C0}.", p_Info[playerid][p_Number]);
			    SendClientMessage(i, 0xC0C0C0AA, string);
			    PhoneRingingTimer[i] = SetTimerEx("PhoneRinging", 4000, true, "i", i);
			}
		}
	}
	return 1;
}



CMD:pickup(playerid, params[]) {

	foreach(new i: Player) {
		if (p_Info[playerid][p_State] != PHONE_STATE_CALLING && p_Info[i][p_State] != PHONE_STATE_CALLING) return SendClientMessage(playerid, LIGHT_GRAY, "You do not have any incoming calls.");
		if(p_Info[playerid][p_State] == PHONE_STATE_CALLING && p_Info[i][p_State] == PHONE_STATE_CALLING) {
		    p_Info[playerid][p_State] = PHONE_STATE_PICKEDUP;
		    p_Info[i][p_State] = PHONE_STATE_PICKEDUP;
		    p_OnPhone[playerid] = i;
		    p_OnPhone[i] = playerid;
		    SendClientMessage(playerid, LIGHT_GRAY, "You picked up the phone.");
		    SendClientMessage(i, LIGHT_GRAY, "They picked up the phone.");
		    KillTimer(PhoneRingingTimer[playerid]);
		    
		}

	}
	return 1;
	
}

CMD:hangup(playerid, params[]) {
    
	
	foreach(new i: Player) {
	    if (p_Info[playerid][p_State] != PHONE_STATE_CALLING && p_Info[i][p_State] != PHONE_STATE_CALLING  ) return SendClientMessage(playerid, LIGHT_GRAY, "You do not have any incoming calls.");
		if(p_Info[playerid][p_State] == PHONE_STATE_CALLING && p_Info[i][p_State] == PHONE_STATE_CALLING) {
		    p_Info[playerid][p_State] = PHONE_STATE_NONE;
		    p_Info[i][p_State] = PHONE_STATE_NONE;
		
		    p_OnPhone[playerid] = INVALID_PLAYER_ID;
		    p_OnPhone[i] = INVALID_PLAYER_ID;

		    SendClientMessage(i, LIGHT_GRAY, "They hung up.");
		    SendClientMessage(playerid, LIGHT_GRAY, "You hung up.");
      		KillTimer(PhoneRingingTimer[playerid]);
		}
		if(p_OnPhone[playerid] == i && p_OnPhone[i] == playerid) {
		    p_Info[playerid][p_State] = PHONE_STATE_NONE;
		    p_Info[i][p_State] = PHONE_STATE_NONE;
		

		
		    p_OnPhone[playerid] = INVALID_PLAYER_ID;
		    p_OnPhone[i] = INVALID_PLAYER_ID;

		    SendClientMessage(playerid, LIGHT_GRAY, "You hung up.");
		    SendClientMessage(i, LIGHT_GRAY, "They hung up.");
      		KillTimer(PhoneRingingTimer[playerid]);
		}
	}
	return 1;
}

CMD:togphone(playerid, params[]) {
	if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /togphone [on/off]");
 	if(p_Info[playerid][p_Ownership] == 0) return SendClientMessage(playerid, LIGHT_GRAY, "You do not have a phone.");
    if(!strcmp(params, "on", true)) {
        SendClientMessage(playerid, LIGHT_GRAY, "You have turned your phone on.");
        p_Info[playerid][p_State] = PHONE_STATE_ON;
    }
    else if (!strcmp(params, "off", true)) {
        SendClientMessage(playerid, LIGHT_GRAY, "You have turned your phone off.");
        p_Info[playerid][p_State] = PHONE_STATE_OFF;
	}
    else {
        SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /togphone [on/off]");
    }
    return 1;
}


// EXAMPLE


// Not-native functions

GenerateRandomPhoneNumber()
{
	new RandomPhoneNumber = 1000000 + random(8999999);
    return RandomPhoneNumber;
}

SendRangedMessage(playerid, color, message[], Float:range) {
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x,y,z);
	foreach(new i:Player) {
		if(GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
			if(IsPlayerInRangeOfPoint(i, range, x,y,z)) {
				SendClientMessage(i, color, message);
			}
		}
	}
}

GetPlayerNameEx(playerid) { //Arguments of a function are (implicitly declared) local variables for that function.
	new NAME[MAX_PLAYER_NAME +1];
	GetPlayerName(playerid, NAME, sizeof(NAME));
	strreplace(NAME, "_", " ");
	return NAME;
}
forward PhoneRinging(playerid);
public PhoneRinging(playerid) {
	new string[25+17];
	format(string, sizeof(string), "%s's phone rings.", GetPlayerNameEx(playerid));
	SendRangedMessage(playerid, PURPLE, string, 5.0);
	PlayerPlaySound(playerid, 20600, 0, 0, 0);
	return 1;
}

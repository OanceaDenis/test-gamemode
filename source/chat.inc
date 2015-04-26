#if defined _inc_chat
	#endinput
#endif
#define _inc_chat


CMD:me(playerid, params[]) {
	new str[128];
	if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /me [action]");
	format(str, sizeof(str), "* %s %s", GetPlayerNameEx(playerid), params);
	SendRangedMessage(playerid, PURPLE, str, 14.0);
	return 1;
}

CMD:do(playerid, params[]) {
	new str[128];
	if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /do [action]");
	format(str, sizeof(str), "* %s (( %s ))", params, GetPlayerNameEx(playerid));
	SendRangedMessage(playerid, PURPLE, str, 14.0);
	return 1;
}

CMD:shout(playerid, params[]) {

	new str[128];
    if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /(s)hout [text]");
    format(str, sizeof(str), "%s shouts: %s", GetPlayerNameEx(playerid), params);
	SendRangedMessage(playerid, WHITE, str, 20.0);
	return 1;
}

CMD:whisper(playerid, params[]) {
	new targetid, text[128], string[144];
	
	if (sscanf(params, "us[128]", targetid, text)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /(w)hisper [ID/PartOfName] [text]");
	if (targetid == playerid) return SendClientMessage(playerid, LIGHT_GRAY, "You cannot whisper to yourself.");
    format(string, sizeof(string), "%s whispers: %s", GetPlayerNameEx(playerid), text);
    if (targetid != INVALID_PLAYER_ID) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if(IsPlayerInRangeOfPoint(targetid, 2.0, x,y,z)) {
            if(IsPlayerConnected(targetid)) {
            	format(string, sizeof(string), "%s whispers: %s", GetPlayerNameEx(playerid), text);
			   	SendClientMessage(targetid, YELLOW, string);
			}
		}
		else if (!IsPlayerConnected(targetid)) {
		    SendClientMessage(playerid, LIGHT_GRAY, "Player is not connected.");
        }
		else if (!IsPlayerInRangeOfPoint(targetid, 2.0, x,y,z)) {
		    SendClientMessage(playerid, LIGHT_GRAY, "The player is too far away.");
		}
	}
	return 1;
}

CMD:pm(playerid, params[]) {
	new targetid, str[128], str1[128], text[128];
	if(sscanf(params, "us[128]", targetid, text)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /pm [ID/PartOfName] [text]");
	if(targetid == playerid) return SendClientMessage(playerid, LIGHT_GRAY, "You cannot send a private message to yourself.");
	if(targetid != INVALID_PLAYER_ID) {
		if(IsPlayerConnected(targetid)) {
		    format(str, sizeof(str), "(( PM from %s[%d]: %s ))", GetPlayerNameEx(playerid), playerid, text);
			SendClientMessage(targetid, YELLOW, str);
            format(str1, sizeof(str1), "(( PM sent to %s[%d]: %s ))", GetPlayerNameEx(targetid), targetid, text);
            SendClientMessage(playerid, YELLOW, str1);
		}
	}
	else if (!IsPlayerConnected(targetid)) {
	    SendClientMessage(playerid, LIGHT_GRAY, "Player is not connected.");
	}
	return 1;
}

CMD:b(playerid, params[]) {
	new str[128];
	if(isnull(params)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /b [text]");
	format(str, sizeof(str), "(( %s[%i]: %s ))", GetPlayerNameEx(playerid), playerid, params);
	SendRangedMessage(playerid, LIGHT_GRAY, str, 14.0);
	return 1;
}
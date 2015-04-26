// to be updated
#if defined _inc_house
	#endinput
#endif
#define _inc_house

#include 	<YSI\y_hooks>
#include    	<a_mysql>

#define MAX_HOUSES       		(10)
#define MAX_HOUSE_NAME			
new houseid;

enum e_House 
{
	hdbID, 
	hName,
	hOwner[MAX_PLAYER_NAME +1],
	hPrice,
	Float:hInteriorX,
	Float:hInteriorY,
	Float:hInteriorZ,
	hUniverseID, 
	Float:hExteriorX,
	Float:hExteriorY,
	Float:hExteriorZ,
	hPickup
};

new h_Info[MAX_HOUSES][e_House];



CMD:tp(playerid, params[]) 
{
	new Float:x, Float:y, Float:z, interiorid; 
	if(sscanf(params, "fffi", x, y, z, interiorid)) return SendClientMessage(playerid, LIGHT_GRAY, "{EE7181}USAGE{C0C0C0}: /tp [x] [y] [z] [interior]");
	SetPlayerPos(playerid, x, y, z);
	SetPlayerInterior(playerid, interiorid); 
	return 1;
}

CMD:createexterior(playerid, params[])
{
	new query[129]; 
	GetPlayerPos(playerid, h_Info[houseid][hExteriorX], h_Info[houseid][hExteriorY], h_Info[houseid][hExteriorZ]); 
	mysql_format(db, query, sizeof(query), "INSERT INTO  Houses  ( hExteriorX ,  hExteriorY ,  hExteriorZ, hUniverseID) VALUES (%f, %f, %f, %i)", 
																h_Info[houseid][hExteriorX], h_Info[houseid][hExteriorY], h_Info[houseid][hExteriorZ], GetPlayerInterior(playerid)); 													
	mysql_tquery(db, query, "OnHouseExteriorCreated");
	SendClientMessage(playerid, LIGHT_GRAY, "Exterior created.");
	return 1; 
}

CMD:createinterior(playerid, params[])
{
	new query[129];
	GetPlayerPos(playerid, h_Info[houseid][hInteriorX], h_Info[houseid][hInteriorY], h_Info[houseid][hInteriorZ]); 
	mysql_format(db, query, sizeof(query), "UPDATE Houses SET hInteriorX = %f, hInteriorY = %f,  hInteriorZ = %f WHERE hdbID = %i",
																h_Info[houseid][hInteriorX], h_Info[houseid][hInteriorY], h_Info[houseid][hInteriorZ], houseid);
	mysql_tquery(db, query, ""); 
	SendClientMessage(playerid, LIGHT_GRAY, "Interior created.");
	return 1;
}

forward OnHouseExteriorCreated();
public OnHouseExteriorCreated()
{
	cache_get_row(0, 
	return 1; 
}


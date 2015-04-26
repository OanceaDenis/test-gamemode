/*  House system design

I'm looking to create a simple house system.

Server owners and chosen administrators will be able to create, finish, and delete houses at any time using some commands:
/hcreateexterior - they have to be in front of the house's front door and type that command in order to set the entrance location (for /enter command);
/teleport - a command that teleports you anywhere (in this case, house interior), provided that the x, y, z coordinates are given;
/hcreateinterior - after a fitting interior has been chosen, you stand at the exit door and type that command (for /exit command);
/hcreatefinish - it should teleport you back to the house's porch where a green house pickup should appear (below or above the pickup, the house's price and name should not be missing);
	params: NAME
	        PRICE
/hinfo - use that command in case you want to be guided;

People can only own one house, unless there is a way to allow them to own up to three houses.
Houses should be enterable if they aren't owned by anyone. That gives our players the possibility to roleplay as estate agents. (More on estate agencies coming soon)
If they like the interior and have enough money to purchase a certain house, they will have to use the '/buyhouse' command while standing at the porch. The house pickup will turn blue which means the house is owned.
* Other players can not see the house owners due to roleplay related reasons.
If they want to sell the house, they will have to use the '/sellhouse' command which basically refunds you with the money you paid for the house, from which a small tax will be deducted.
I have also thought of other commands, but at the moment, the framework is what matters the most.


*/

#if defined _inc_house
	#endinput
#endif
#define _inc_house

#include 		<YSI\y_hooks>
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

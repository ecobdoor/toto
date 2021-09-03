;
/*/////////////////////////////////////////////////////////////////
	CONTROLEUR D'ETATS
=================================================================*/
function _runAPP(MODULES, PILOTE, SOKROV, SOKBAS, FILSYS, DATIM, LEDS, MOTORS, JOBGOAL, DRIVER) {
	var SELF = this;
	var cntQUERIES = 0;
	var _Pctrl = 256;
	//var _Pdtms	=-1;
	var _oldTms = _TMS_();
	var _Open = false;
	/*
		Appel des fonctions après initialisation des éléments d'interface homme machine (App)
		habillage 
		__selectNosendevents
		__selectDefaultevents
		__textDefaultevents
	*/
	PILOTE.launchEvents(SELF);
	SOKROV.launchEvents(SELF);
	SOKBAS.launchEvents(SELF);
	FILSYS.launchEvents(SELF);
	DATIM.launchEvents(SELF);
	LEDS.launchEvents(SELF);
	MOTORS.launchEvents(SELF);
	JOBGOAL.launchEvents(SELF);
	DRIVER.launchEvents(SELF);
}
//////////////////////////////////////////////////////////////

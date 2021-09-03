function _PilotUmi(CTRL, CURSOR, CFG) {
	//*******************************************************
	//  CONSTRUCTEUR
	//
	//=======================================================
	var SELF = this;
	this.CTRL = CTRL;
	this.CURSOR = CURSOR;
	this._CtrL = document.getElementById(CTRL);
	this._CtrLbounds = SELF._CtrL.getBoundingClientRect();
	this._Cursor = document.getElementById(CURSOR);
	this.CtrL_Height = this._CtrL.offsetHeight - this._Cursor.offsetHeight;
	this.CtrL_Width = this._CtrL.offsetWidth - this._Cursor.offsetWidth;
	this.Cd2_Height = this._Cursor.offsetHeight / 2;
	this.Cd2_Width = this._Cursor.offsetWidth / 2;
	this._Listening = false;
	this._MIN = { 'DIR': CFG.DIR.MIN, 'VIT': CFG.VIT.MIN };
	this._VAL = { 'DIR': CFG.DIR.CLR, 'VIT': CFG.VIT.CLR };
	this._MAX = { 'DIR': CFG.DIR.MAX, 'VIT': CFG.VIT.MAX };;
	//-------------------------------------------------------
	//  PLACER LE CURSEUR
	//
	this.placeCursor = function() {
		SELF._Cursor.style.marginTop = SELF.CtrL_Height * (1 - (SELF._VAL.VIT - SELF._MIN.VIT) / (SELF._MAX.VIT - SELF._MIN.VIT)) + "px";
		SELF._Cursor.style.marginLeft = SELF.CtrL_Width * ((SELF._VAL.DIR - SELF._MIN.DIR) / (SELF._MAX.DIR - SELF._MIN.DIR)) + "px";
	}
	this.placeCursor();
	events();// Anonyme, lancé une seule fois sans variable !!!
	//-------------------------------------------------------
	//  LIRE LES VALEURS
	//
	this.getVal = function() {
		return SELF._VAL;
	}
	this.setFunctions = function(MIN, VAL, MAX, SET) {
		SELF._MIN = MIN;
		SELF._VAL = VAL;
		SELF._MAX = MAX;
		SELF._SET = SET;
		SELF.placeCursor();
	}
	//-------------------------------------------------------
	//  GERER LE MOUVEMENT
	//
	function moveCursor(XX, YY) {
		// suivi de la position de la souris dans la console
		console.log("Position de la souris :  X=" + XX + "   Y=" + YY, joystickBounds);
		if (SELF._Listening) {
			var newMargLeft = XX - SELF._CtrLbounds.left - SELF.Cd2_Width;
			if (newMargLeft < 0) newMargLeft = 0
			else if (SELF.CtrL_Width < newMargLeft) newMargLeft = SELF.CtrL_Width;
			SELF._Cursor.style.marginLeft = newMargLeft + "px";
			SELF._VAL.DIR = SELF._MAX.DIR - (SELF._MAX.DIR - SELF._MIN.DIR) * (1 - (newMargLeft) / SELF.CtrL_Width);

			var newMargTop = YY - SELF._CtrLbounds.top - SELF.Cd2_Height;
			if (newMargTop < 0) newMargTop = 0
			else if (SELF.CtrL_Height < newMargTop) newMargTop = SELF.CtrL_Height;
			SELF._Cursor.style.marginTop = newMargTop + "px";
			SELF._VAL.VIT = SELF._MIN.VIT + (SELF._MAX.VIT - SELF._MIN.VIT) * (1 - (newMargTop) / SELF.CtrL_Height);

			//SELF._VAL.VIT=SELF._SET(SELF._VAL.VIT);   .... CB
			//placeCursor();
			console.log(SELF._VAL, newMargLeft, newMargTop);
		}
	}
	//-------------------------------------------------------
	//  events
	//
	function events() {

		SELF._CtrL.onmousemove = function(e) {
			e.preventDefault();
			moveCursor(e.clientX, e.clientY);
		};
		SELF._CtrL.ontouchmove = function(e) {
			e.preventDefault();
			//            var touch = e.originalEvent.touches[0] || e.originalEvent.changedTouches [0];
			var touch = e.touches[0] || e.changedTouches[0];
			moveCursor(touch.pageX, touch.pageY);
		};
		SELF._Cursor.onmousedown = function() {
			console.log("SELF._Cursor.onmousedown : SELF._Listening ", SELF._Listening);
			SELF._Listening = true;
			console.log("SELF._Cursor.onmousedown : SELF._Listening ", SELF._Listening);
		};
		SELF._Cursor.ontouchstart = function() {
			console.log("SELF._Cursor.ontouchstart : SELF._Listening ", SELF._Listening);
			SELF._Listening = true;
			console.log("SELF._Cursor.ontouchstart : SELF._Listening ", SELF._Listening);
		};
		SELF._Cursor.onmouseup = function() {
			console.log("SELF._Cursor.onmouseup : SELF._Listening ", SELF._Listening);
			SELF._Listening = false;
			console.log("SELF._Cursor.onmouseup : SELF._Listening ", SELF._Listening);
			SELF.stop();
		};
		SELF._Cursor.ontouchend = function() {
			console.log("SELF._Cursor.ontouchend : SELF._Listening ", SELF._Listening);
			SELF._Listening = false;
			console.log("SELF._Cursor.ontouchend : SELF._Listening ", SELF._Listening);
			SELF.stop();
		};
		return;
		/*
				$( "#"+SELF.CTRL).click(function(event){
					console.log(SELF._Listening,"click ",SELF.CTRL,MV2.pageY);
					SELF._Listening=true;
					SELF.move(event);
					SELF._Listening=false;
				});
				*/
	}
	//-------------------------------------------------------
	//  stop à affiner, sécurité dans le main: onmouseup over etc...
	//
	this.stop = function() {
		SELF._Listening = false;
		SELF._VAL = { 'DIR': CFG.DIR.CLR, 'VIT': CFG.VIT.CLR };
		SELF.placeCursor();
		console.log(".stop", SELF._VAL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////

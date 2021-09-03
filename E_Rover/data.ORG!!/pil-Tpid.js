;
function _TpidMmi(CFG) {
	//*******************************************************
	//  CONSTRUCTEUR
	//
	var SELF = this;
	SELF.PROF = CFG.PROF;
	this._MIN = CFG.MIN;
	this._MAX = CFG.MAX;
	this._KP = CFG.KP;
	this._KI = CFG.KI;
	this._KD = CFG.KD;
	this.ERRORS = [];
	this.TPS = [];
	for (var i = 0; i < SELF.PROF; i++) {
		this.ERRORS.push(0);
		this.TPS.push(0);
	}
	this.PTR = -1;
	this.CNT = 0;
	//-------------------------------------------------------
	//  ACCUMULATION DES ERREURS
	//
	function pushError(ERROR, TPS) {
		SELF.PTR = (SELF.PTR + 1) % SELF.PROF;
		SELF.CNT++;
		if (SELF.CNT > SELF.PROF) SELF.CNT = SELF.PROF;
		SELF.ERRORS[SELF.PTR] = ERROR;
		SELF.TPS[SELF.PTR] = TPS;
	}
	//-------------------------------------------------------
	//  NEXT
	//
	function next(I, MOD) {
		var i = I + 1;
		if (i == MOD) return 0; else return i;
	}
	//-------------------------------------------------------
	//  PREV
	//
	function prev(I, MOD) {
		var i = I - 1;
		if (i < 0) return MOD - 1; else return i;
	}
	//-------------------------------------------------------
	//  INTEGRATION
	//
	function integrale() {
		var integ = 0;
		var cur = SELF.PTR;
		for (var i = 1; i < SELF.CNT; i++) {
			var prv = prev(cur, SELF.PROF);
			integ += SELF.ERRORS[cur] * (SELF.TPS[cur] - SELF.TPS[prv]);
			cur = prev(cur, SELF.PROF);
		}
		return integ;
	}
	//-------------------------------------------------------
	//  DERIVATION
	//
	function derivee() {
		var deriv = 0;
		var cur = SELF.PTR;
		for (var i = 1; i < SELF.CNT; i++) {
			var prv = prev(cur, SELF.PROF);
			deriv += (SELF.ERRORS[cur] - SELF.ERRORS[prv]) / (SELF.TPS[cur] - SELF.TPS[prv]);
			cur = prev(cur, SELF.PROF);
		}
		return deriv;
	}
	//-------------------------------------------------------
	//  asservissement
	//
	this.feedback = function(ERROR, TPS) {
		pushError(ERROR, TPS);
		var consigne = SELF._KP * ERROR + SELF._KI * integrale() + SELF._KD * derivee();
		if (this._MAX < consigne) consigne = this._MAX;
		if (consigne < this._MIN) consigne = this._MIN;
		return consigne;
	};
}

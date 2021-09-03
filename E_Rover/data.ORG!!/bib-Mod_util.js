/**
 * REMPLACER TOUTES LES OCCURENCES
 */
function strReplace(CHN, OLD, NEW) {
	var re = new RegExp(OLD, 'gi');// 
	return CHN.replace(re, NEW);
}
const pokeState = {
	poked : 0,
	fifoFull_ : 1
};
const pikeState = {
	piked : 0,
	timeOut_ : 1,
	notFound_ : 2
};
var jctManager = function() {
	{
		slot: 2
	}
}

var FifoAck = function(FIFOMAX) {
	// private:var...
	var SELF = this;
	var _fifo = new Array(FIFOMAX);
	this._latence = 0;
	this.clear = function() {
		for (var i = 0; i < FIFOMAX; i++) {
			_fifo[i] = {
				TIK : 0,
				MSG : "",
				TMS : 0
			};
		}
	}
	this.clear();
	// public: via SELF... for events
	this.waitingACK = function() {
		var l = 0;
		for (var i = 0; i < FIFOMAX; i++)
			if (_fifo[i].TIK != 0)
				l++;
		return l;
	}
	this.pokeForACK = function(CHN) {
		do {
			for (var i = 0; i < FIFOMAX; i++)
				if (_fifo[i].TIK == 0) {
					_fifo[i].TMS = Date.now();
					_fifo[i].MSG = CHN;
					_fifo[i].TIK = parseInt(
							CHN.substring(1 + CHN.indexOf("}")), 10);
					;
					return pokeState.poked;
				}
		} while (true);
		return pokeState.fifoFull_;
	}
	this.pikeForACK = function(CHN) {
		var found = pikeState.notFound_;
		var indexOfapptik = 1 + CHN.indexOf("}");
		var tik = parseInt(CHN.substring(indexOfapptik), 10);
		for (var i = 0; i < FIFOMAX; i++) {
			if (_fifo[i].TIK != 0)
				console.log(`${i} ${_fifo[i].TIK}\t${_fifo[i].MSG}`);
			if (_fifo[i].TIK == Math.abs(tik)) {
				found = pikeState.piked;
				this._latence = Date.now() - _fifo[i].TMS;
				if (tik > 0) {// Case of multiple REPLY
					_fifo[i].MSG = "";
					_fifo[i].TIK = 0;
					_fifo[i].TMS = 0;
				}
			}
		}
		return found;
	}
}
// ===================================================================
// On renvoie un entier aléatoire entre une valeur min (incluse)
// et une valeur max (incluse).
// Attention : si on utilisait Math.round(), on aurait une distribution
// non uniforme !
function getRandomIntInclusive(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(Math.random() * (max - min + 1)) + min;
}

;
function _Canvas2DUmi(CANVAS, XMIN, XMAX, YMIN, YMAX, LFT, RGT, TOP, BOT,
	TITLE, LEGENDS_COLORS/* LEGEND_A,COLOR_A,LEGEND_B,COLOR_B */) {
	// *******************************************************
	// CONSTRUCTEUR
	//
	// =======================================================

	var SELF = this;
	var Csiz = 5;
	this._CANVAS = document.getElementById(CANVAS);
	this.Gwidth = SELF._CANVAS.width - LFT - RGT;
	this.Gheight = SELF._CANVAS.height - TOP - BOT;
	if (!this._CANVAS) {
		alert("Impossible de récupérer le canvas");
		return;
	}
	this._CTX = this._CANVAS.getContext('2d');
	if (!this._CTX) {
		alert("Impossible de récupérer le contexte du canvas");
		return;
	}
	this._CTX.fillStyle = "#ffffff";
	this._CTX.fillRect(LFT, TOP, this.Gwidth, this.Gheight);

	this._CTX.strokeStyle = '#880000';
	var s = 2;
	this._CTX.beginPath(); // On démarre un nouveau tracé.
	this._CTX.moveTo(LFT - s, TOP - s);// On se déplace au coin inférieur
	// gauche
	this._CTX.lineTo(LFT - s, TOP + this.Gheight + s);
	this._CTX.lineTo(LFT + this.Gwidth + s, TOP + this.Gheight + s);
	this._CTX.lineTo(LFT + this.Gwidth + s, TOP - s);
	this._CTX.lineTo(LFT - s, TOP - s);
	this._CTX.stroke();// On trace seulement les lignes.
	this._CTX.font = "15px Arial";
	this._CTX.fillText(TITLE, 10, 20);
	for (var i = 0; i < LEGENDS_COLORS.length; i++) {
		this._CTX.fillStyle = LEGENDS_COLORS[i].COLOR;
		this._CTX.fillText(LEGENDS_COLORS[i].LEGEND, 100 + 50 * i, 20);
		LEGENDS_COLORS[i].oldCross = false;

	}
	;
	/*
	 * this._CTX.fillStyle=COLOR_A; this._CTX.fillText(LEGEND_A,100,20);
	 * this._CTX.fillStyle=COLOR_B; this._CTX.fillText(LEGEND_B,200,20);
	 */
	this._CTX.fillStyle = "#000000";
	this._CTX.font = "10px Arial";
	this._CTX.fillText(SF_float(YMIN, 7, 1), Abscisse(XMIN) - 30,
		Ordinate(YMIN) + s);
	this._CTX.fillText(SF_float(YMAX, 7, 1), Abscisse(XMIN) - 30,
		Ordinate(YMAX) + s);
	this._CTX.fillText(SF_float(XMIN, 7, 1), Abscisse(XMIN) - 10,
		Ordinate(YMIN) + 15);
	this._CTX.fillText(SF_float(XMAX, 7, 1), Abscisse(XMAX) - 30,
		Ordinate(YMIN) + 15);
	this._CTX.fillText("ABS", Abscisse(0), Ordinate(YMIN) + 15);
	this._CTX.translate(Abscisse(0), Ordinate(0));

	this._CTX.rotate(-Math.PI / 2);

	this._CTX.fillText("ORD", 00, -140);

	// this._CTX.restore();

	this._CTX.rotate(Math.PI / 2);
	this._CTX.translate(-Abscisse(0), -Ordinate(0));

	// this._CTX.fill(); //On utilise la méthode fill(); si l'on veut une forme
	// pleine
	this._CTX.closePath();
	this._CTX.arc(10, 100, 50, 0, Math.PI); // On trace la courbe délimitant
	// notre forme
	this._CTX.stroke();// On trace seulement les lignes.

	// -------------------------------------------------------
	// CERCLE
	//
	this.Circle = function() {
		// this._CTX.beginPath(); //On démarre un nouveau tracé.
		this._CTX.fillStyle = "#0000ff";
		this._CTX.arc(10, 100, 20, 0, 2 * Math.PI); // On trace la courbe
		// délimitant notre forme
		this._CTX.stroke();// On trace seulement les lignes.
		// this._CTX.fill(); // si l'on veut une forme pleine
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// TEXTE
	//
	this.Text = function(TXT, X, Y) {
		this._CTX.font = "30px Arial";
		this._CTX.fillText(TXT, X, Y);
	}
	// -------------------------------------------------------
	// POINT
	//
	this.Point = function(X, Y, COLOR) {
		this._CTX.beginPath(); // On démarre un nouveau tracé.
		this._CTX.strokeStyle = COLOR;
		this._CTX.moveTo(X, Y);// On se déplace au coin inférieur gauche
		this._CTX.lineTo(X - 1, Y - 1);
		this._CTX.stroke();// On trace seulement les lignes.
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// Xcross
	//
	this.Xcross = function(X, Y, COLOR) {
		this._CTX.beginPath(); // On démarre un nouveau tracé.
		this._CTX.strokeStyle = COLOR;
		this._CTX.moveTo(X - Csiz, Y - Csiz);
		this._CTX.lineTo(X + Csiz, Y + Csiz);
		this._CTX.moveTo(X - Csiz, Y + Csiz);
		this._CTX.lineTo(X + Csiz, Y - Csiz);
		this._CTX.stroke();// On trace seulement les lignes.
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// Line
	//
	this.Line = function(Xa, Ya, Xb, Yb, COLOR) {
		this._CTX.beginPath(); // On démarre un nouveau tracé.
		this._CTX.strokeStyle = COLOR;
		this._CTX.moveTo(Xa, Ya);// On se déplace au coin inférieur gauche
		this._CTX.lineTo(Xb, Yb);
		this._CTX.stroke();// On trace seulement les lignes.
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// placeLine
	//
	this.placeLine = function(ID, X1, Y1, X2, Y2) {
		var Xa = Abscisse(X1);
		var Ya = Ordinate(Y1);
		var Xb = Abscisse(X2);
		var Yb = Ordinate(Y2);
		this.Line(Xa, Ya, Xb, Yb, LEGENDS_COLORS[ID].COLOR);
	}
	// -------------------------------------------------------
	// Abscissa
	//
	function Abscisse(X) {
		var abs = LFT + SELF.Gwidth - SELF.Gwidth * (XMAX - X) / (XMAX - XMIN);
		return abs;
	}
	// -------------------------------------------------------
	// Ordinate
	//
	function Ordinate(Y) {
		var ord = TOP + SELF.Gheight - SELF.Gheight * (Y - YMIN)
			/ (YMAX - YMIN);
		return ord;
	}
	this.placePoint = function(ID, X, Y) {
		// this._CTX.globalCompositeOperation = 'xor';
		var A = Math.round(Abscisse(X));
		var B = Math.round(Ordinate(Y));

		this.Point(A, B, LEGENDS_COLORS[ID].COLOR);
		if (LEGENDS_COLORS[ID].oldCross) {
			SELF._CTX.putImageData(LEGENDS_COLORS[ID].savedCross,
				LEGENDS_COLORS[ID].oldA - Csiz, LEGENDS_COLORS[ID].oldB - Csiz);
			SELF.Line(A, B, LEGENDS_COLORS[ID].oldA, LEGENDS_COLORS[ID].oldB,
				LEGENDS_COLORS[ID].COLOR);
		}
		LEGENDS_COLORS[ID].savedCross = SELF._CTX.getImageData(A - Csiz, B
			- Csiz, 2 * Csiz + 1, 2 * Csiz + 1);
		LEGENDS_COLORS[ID].oldCross = true;
		LEGENDS_COLORS[ID].oldA = A;
		LEGENDS_COLORS[ID].oldB = B;

		// this._CTX.globalCompositeOperation = 'copy';
		this.Xcross(A, B, LEGENDS_COLORS[ID].COLOR);
	}
}

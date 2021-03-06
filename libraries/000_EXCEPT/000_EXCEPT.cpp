#include <000_LOGS.h>

///////////////////////////////////////////////////////////////////////
/*
 void Throw(const e_STR STR_COD ,const  char * FUNCTION,const int LINE,const  char * FILE,...){
 va_list args;
 va_start(args, FILE);
 throw EXCEPT(new Scratch(STR_COD,FUNCTION,LINE,FILE,&args));
 va_end(args);
 }
 */
Scratch* rtnThrow(const e_STR STR_COD, const char *FUNCTION, const int LINE, const char *FILE, ...){
	va_list args;
	va_start(args, FILE);
	Scratch *rtn = new Scratch(STR_COD, FUNCTION, LINE, FILE, &args);
	va_end(args);
	return rtn;
}
void ThrowErr(TaskHandle_t *TSKHDL, const char *FUNCTION, const int LINE, const char *FILE,
	const e_STR STR_COD, ...){
	s_tskInfo_X *ti = findTask(TSKHDL);
	if (ti != nullptr) {
		va_list args;
		va_start(args, STR_COD);
		ti->scratch = new Scratch(STR_COD, FUNCTION, LINE, FILE, &args);
		throw EXCEPT(ti->scratch);
		va_end(args);
	}
	else
		inString("NOT REGISTRED sdjkhskdjfhskdfhskdfhskdfh ???");
}

Scratch::Scratch(const e_STR ERRCOD, const char *FUNCTION, const int LINE, const char *FILE,
	va_list *ARGS){
	//_SERIAL_0("\nERRCOD %i `%s` `%i` `%s`",(int)ERRCOD,FUNCTION,LINE,FILE);
	m_number = ERRCOD;
	//label=LABEL(ERRCOD);
	string errLvl;
	Lang.inc(ERRCOD); // increment cnt & compare with threshold
	if (0 == Lang.thr(ERRCOD)) {
		m_level = _INFOS;
		errLvl = "INFOS";
	}
	else if (Lang.cnt(ERRCOD) < Lang.thr(ERRCOD)) {
		m_level = _RECOV;
		errLvl = "RECOV";
	}
	else {
		m_level = _FATAL;
		errLvl = "FATAL";
	}
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "### %s(%i/%i) ", errLvl.c_str(), Lang.cnt(ERRCOD),
		Lang.thr(ERRCOD));
	VSPrintF(sizeof(buffer), cnt, buffer, Lang.CST(ERRCOD), ARGS);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t%s(%3i) in `%s` at line %i of %s", Lang.LABEL(ERRCOD),
		m_number, FUNCTION, LINE, FILE);
	//_SERIAL_0("\n%p %s",this,buffer);
}
/**
 * @fn void Scratch::fixError()
 * @brief Fixes the error determining its severity level (abort, pass...)
 *
 * Each @ref s_CSTR has a invoked count incremented by fixError. When the count reaches
 * the maximum acceptable threshold, the error becomes fatal, and the application restarts the rover.
 * Below it is considered as recoverable, ie fixError returns in the catch block and it is up to
 * the application to deal with the problem.
 *
 * @param FUNCTION from where FIX_ERROR() is inserted
 * @param LINE from where FIX_ERROR() is inserted
 * @param FILE from where FIX_ERROR() is inserted
 */
void Scratch::fixError(const char *FUNCTION, const int LINE, const char *FILE){
	if (m_level == _FATAL)
		c_linkISR::detach_ISR(); // out side of throw !!!!!
	int16_t cnt = strlen(buffer);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\tERROR caught on `%s`  at line %i of %s this=(%p)",
		FUNCTION, LINE, FILE, this);
	//Serial.printf("\n%s\n", buffer); 
	Serial.printf("\n%s", dump_TASKS().c_str());
	Serial.printf("\n##############################################");
	if (m_level == _FATAL) {
		char x = inKey("\nFATAL ERROR. Wait for hit key (Enter=restart, Space=continue)...");
		Serial.printf("\nINKEY ='%c'", x);
		if (x == '\n')
			ESP.restart();
		/*vTaskSuspendAll();*/
		while (true) {
			NOP();
		}
	}
}
void FIX_ERRORS(){
	for (int e = 0; e < static_cast<int>(e_tasks::END); e++) {
		s_tskInfo_X &ti = tskMON[static_cast<int>(e)];
		if (ti.scratch != nullptr) {
			if (ti.scratch->m_level == Scratch::_FATAL)
				c_linkISR::detach_ISR(); // out side of throw !!!!!
		}
	}

}
//---------------------------------------------------------------------
char* charAlloc(const size_t SIZE){
	void *ptr;
	try {
		ptr = malloc(SIZE);
		if (!ptr) {
			_SERIAL_0(Lang.CST(BAD_ALLOC), ptr, SIZE, ESP.getMaxAllocHeap(),
				ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getMinFreeHeap());
			THROWERR(BAD_ALLOC, ptr, SIZE, ESP.getMaxAllocHeap(),
				ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getMinFreeHeap());
		}
	} catch (EXCEPT const &e) {
//		FIX_ERROR();
		THROW_UP();
	}
	return (char*)ptr;
}
///////////////////////////////////////////////////////////////////////
/*
 Status code 	Nom 	Description
 0???999 	  	Reserv?? et inutilis??.
 1000 		CLOSE_NORMAL 			Fermeture normale ; la connexion s'est termin??e normalement.
 1001 		CLOSE_GOING_AWAY 		Re??u lorsque une erreur est apparue sur le serveur ou le navigateur quitte la page ayant ouvert la connexion.
 1002 		CLOSE_PROTOCOL_ERROR 	L'autre partie de la connexion terminte la connexion ?? cause d'une erreur de protocole.
 1003 		CLOSE_UNSUPPORTED 		La connexion est termin??e car le serveur ?? re??u des donn??es d'un type qu'il ne peut pas accepter (par exemple des donn??es binaire plut??t que du texte).
 1004 	  	R??serv??. 				Une utilisation pourrait ??tre d??finie dans le futur.
 1005 		CLOSE_NO_STATUS 		R??serv??. Indique qu'aucun code n'est donn?? bien qu'un code ??tait attendu.
 1006 		CLOSE_ABNORMAL 			R??serv??. Utilis?? pour indiquer que la connexion a ??t?? termin??e anormalement (sans paquet indiquant la fermeture).
 1007 			  					Le serveur termine la connexion car un message contenant des donn??es incoh??rentes a ??t?? re??u (e.g., des donn??es qui ne sont pas au format UTF-8 dans un message texte).
 1008 			  					Le serveur termine la connexion car un message ne respectant pas la politique du serveur est re??u. C'est un code d'??tat g??n??rique qui est utilis?? lorsque les codes 1003 et 1009 ne correspondent pas ?? la situation.
 1009 		CLOSE_TOO_LARGE 		Le serveur termine la connexion car les donn??es re??ues sont trop grosses.
 1010 	  							Le client termine la connexion car il souhaitait n??gocier une ou plusieurs extensions mais le serveur ne l'a pas fait.
 1011 	  							Le serveur termine la connexion car il a rencontr?? un probl??me qui l'emp??che de traiter la requ??te.
 1014 	  							R??serv?? pour une utilisation future par le standard WebSocket.
 1015 	  	R??serv??. 				Indique que la connexion a ??t?? ferm??e ?? cause d'une erreur durant le handshake TLS (e.g., le certificat du serveur ne peut pas ??tre v??rifi??).
 1016???1999 	  						R??serv?? pour une utilisation future par le standard WebSocket.
 2000???2999 	  						R??serv?? pour une utilisation future par le standard WebSocket.
 3000???3999 	  						Disponible pour ??tre utilis?? par des biblioth??ques ou des frameworks. Ces codes ne doivent pas ??tre utilis??s par des applications.
 4000???4999 	  						Disponible pour ??tre utilis??s par des applications.
 */

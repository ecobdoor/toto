#include "esp32_SMS_client.h"
#include "esp32_GOODIES.h"
#include <WiFiClientSecure.h>
/*
 #include <string>
 #include <iomanip>
 #include <sstream>
 using namespace std;
 */
/**
 * Pour récupérer le cerficat, envoyer un sms via firefox
 * https://smsapi.free-mobile.fr/sendmsg?user=xxxxxxxx&pass=xxxxxxxxxxxxxx&msg=firefox...
 * Cliquer sur le cadenas / Connexion sécurisée / Plus d'information / Afficher le certificat
 * Télécharger PEM(chain) et copier le second certificat
 */
///////////////////////////////////////////////////////////////////////
WiFiClientSecure client;
const char *server;
const char *user;
const char *pass;
const char *ca_cert =
	// Le bon c'est le second certificat de smsapi-free-mobile-fr-chain.pem
	"-----BEGIN CERTIFICATE-----\n"
	"MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/\n"
	"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
	"DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow\n"
	"MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT\n"
	"AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs\n"
	"jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp\n"
	"Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB\n"
	"U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7\n"
	"gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel\n"
	"/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R\n"
	"oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E\n"
	"BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p\n"
	"ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE\n"
	"p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE\n"
	"AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu\n"
	"Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0\n"
	"LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf\n"
	"r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B\n"
	"AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH\n"
	"ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8\n"
	"S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL\n"
	"qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p\n"
	"O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw\n"
	"UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==\n"
	"-----END CERTIFICATE-----\0";
///////////////////////////////////////////////////////////////////////
String url_encode(const char *CHN){
	char escaped[128];
	size_t j = 0;
	for (size_t i = 0; i < strlen(CHN); i++) {
		char c = CHN[i];
		//Serial.printf("\n%2i %2i %c", i, j, c);
		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
			sprintf(&escaped[j], "%c", c);
		else { // Any other characters are percent-encoded
			int n = sprintf(&escaped[j], "%%%02X", c);
			//Serial.printf(":%% %02x(%i)%i %i %i", c, n, escaped[j], escaped[j + 1], escaped[j + 2]);
			j = j + 2;
		}
		j++;
	}
	escaped[j] = 0;
	return String(escaped);
}
//---------------------------------------------------------------------
void test_url_encode(const char *TEXT){
	Serial.printf("\nTest TXT MSG=[%s]", TEXT);
	Serial.printf("\n     URL MSG=[%s]", url_encode(TEXT).c_str());
}
//---------------------------------------------------------------------
bool SETUP_SMS(const char *SERVER, const char *USER, const char *PSWD){
	Serial.print("\n-----------------------------\n");
	Serial.print(ca_cert);
	Serial.print("\n-----------------------------\n");
	/* set SSL/TLS certificate */
	client.setCACert(ca_cert);
	user = USER;
	pass = PSWD;
	server = SERVER;
	//test_url_encode("K K'K\rK+KïKà\"");
	//test_url_encode("-B_onjour.");
	uint32_t startHeap = ESP.getFreeHeap();
	int ok = client.connect(server, 443);
	char *buf[128];
	int x = client.lastError((char*)buf, 128);
	Serial.printf("\n$$$$ SMS %i %i %s", ok, x, buf);
	uint32_t nowHeap = ESP.getFreeHeap();
	Serial.printf("\n$$$$ SMS needs %7.3f kB heap.", (startHeap - nowHeap) / 1000.0);
	client.stop();
	nowHeap = ESP.getFreeHeap();
	Serial.printf("\n$$$$ SMS lets  %7.3f kB heap after stop.", nowHeap / 1000.0);
	return ok != 0;
}
//---------------------------------------------------------------------
bool SEND_SMS(const char *TEXT){
	Serial.println("\nConnect to server via port 443");
	printSysState();
	if (!client.connect(server, 443)) {
		char bbuuff[256];
		int err = client.lastError(bbuuff, sizeof(bbuuff));
		Serial.printf("%i:%s", err, bbuuff);
		Serial.println("Connection failed!");
		return false;
	} else {
		printSysState();
		/*
		 const char* fingerprint = "EB 49 4D F9 6A 8A 87 FC 7D D0 CA 7E 99 20 15 47 37 FC AD 4E";
		 */
		Serial.printf("\nConnected to server to send");
		test_url_encode(TEXT);
		/* create HTTP request */
		client.println(String("GET /sendmsg?user=") + user +
			String("&pass=") + pass +
			String("&msg=") + url_encode(TEXT).c_str() +
			//                        String("&msg=") +String(TEXT) +
			String(" HTTP/1.0"));
		client.println(String("Host: ") + server);
		client.println("Connection: close");
		client.println();

		Serial.printf("\nWaiting for response ");
		while (!client.available()) {
			delay(50); //
			Serial.print(".");
		}
		Serial.println();
		/* if data is available then receive and print to Terminal */
		while (client.available()) {
			char c = client.read();
			Serial.write(c);
		}

		/* if the server disconnected, stop the client */
		if (!client.connected()) {
			Serial.println();
			Serial.println("Server disconnected");
			client.stop();
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////
/*
 while( (c = getchar()) != EOF ){
 if( ('a' <= c && c <= 'z')
 || ('A' <= c && c <= 'Z')
 || ('0' <= c && c <= '9') ){
 putchar(c);
 } else {
 putchar('%');
 putchar(hex[c >> 4]);
 putchar(hex[c & 15]);
 }

 */
/*
 std::string url_encode(const std::string &value) {
 std::ostringstream escaped;
 escaped.fill('0');
 escaped << hex;
 for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
 string::value_type c = (*i);
 // Keep alphanumeric and other accepted characters intact
 if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
 escaped << c;
 continue;
 }
 // Any other characters are percent-encoded
 escaped << uppercase;
 escaped << '%' << setw(2) << int((unsigned char) c);
 escaped << nouppercase;
 }
 return escaped.str();
 }
 */
/*
 j++;
 escaped[j]=toupper(escaped[j]);
 j++;
 escaped[j]=toupper(escaped[j]);
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>
#include <sys/param.h>
#include <esp_http_server.h> //because FreeRTOS.h must appear before event_groups.h ???
#include "esp_ota_ops.h"
#include "freertos/event_groups.h"
#include "E_Motion_ota.h"
#include "esp32_HTTP_server.h"
#include "ota_index_html_gz.h"
#include "esp32_GOODIES.h"
#include "CAM_FILSYS.h"
///////////////////////////////////////////////////////////////////////
static const char *APPNAME = "unknown.................................."; //long enough!!
httpd_handle_t OTA_server = nullptr;
int8_t flash_status = 0;

EventGroupHandle_t reboot_event_group;
const int REBOOT_BIT = BIT0;
//---------------------------------------------------------------------
/*****************************************************
 
 systemRebootTask()
 
 NOTES: This had to be a task because the web page needed
 an ack back. So i could not call this in the handler
 
 *****************************************************/
void systemRebootTask(void *parameter){

	// Init the event group
	reboot_event_group = xEventGroupCreate();

	// Clear the bit
	xEventGroupClearBits(reboot_event_group, REBOOT_BIT);
	Serial.printf("\nOTA: creating the Reboot task");

	for (;;) {
		// Wait here until the bit gets set for reboot
		EventBits_t staBits = xEventGroupWaitBits(reboot_event_group, REBOOT_BIT, pdTRUE, pdFALSE,
		portMAX_DELAY);

		// Did portMAX_DELAY ever timeout, not sure so lets just check to be sure
		if ((staBits & REBOOT_BIT) != 0) {
			Serial.printf("\nOTA: Reboot Command, Restarting");
			vTaskDelay(2000 / portTICK_PERIOD_MS);

			esp_restart();
		}
	}
}
//---------------------------------------------------------------------
/* Send index.html Page */
esp_err_t OTA_index_html_handler(httpd_req_t *req){
	browser.On();
	ESP_LOGI("OTA", "index.html Requested");

	// Clear this every time page is requested
	flash_status = 0;

	httpd_resp_set_type(req, "text/html");
	httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
	browser.Off();

	return httpd_resp_send(req, (const char*)ota_index_html_gz, ota_index_html_gz_len);

	return ESP_OK;
}
//---------------------------------------------------------------------
/* Send .ICO (icon) file  */
esp_err_t OTA_favicon_ico_handler(httpd_req_t *req){
	browser.On();
	ESP_LOGI("OTA", "favicon_ico Requested");

	httpd_resp_set_type(req, "image/x-icon");
	browser.Off();

//	httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_end - favicon_ico_start);

	return ESP_OK;
}
//---------------------------------------------------------------------
char* skipStartUri(const char *uri){
	return strchr((char*)&uri[1], '/');
}
/* Copies the full path into destination buffer and returns
 * pointer to path (skipping the preceding base path) */
/**
 *
 * @param dest to recieve the filename preceded from local server path
 * @param base_path
 * @param uri
 * @param destsize size of dest char * buffer
 * @return pointer to filename in dest without local server path
 */
const char* get_path_from_uri(char *dest, const char *base_path,
	const char *uri, size_t destsize){
	const size_t base_pathlen = strlen(base_path);
	size_t pathlen = strlen(uri);

	const char *quest = strchr(uri, '?');
//	OTA_getstatus
	if (quest) {
		pathlen = MIN(pathlen, quest - uri);
	}
	const char *hash = strchr(uri, '#');
	if (hash) {
		pathlen = MIN(pathlen, hash - uri);
	}

	if (base_pathlen + pathlen + 1 > destsize) {
		/* Full path string won't fit into destination buffer */
		return NULL;
	}

	/* Construct full path (base + path) */
	strcpy(dest, base_path);
	strlcpy(dest + base_pathlen, uri, pathlen + 1);

	/* Return pointer to path, skipping the base */
	return dest + base_pathlen;
}
//---------------------------------------------------------------------
/**
 * https://github.com/espressif/esp-idf/blob/master/examples/protocols/http_server/file_serving/main/file_server.c
 * @param req
 * @return
 */
#include <sys/stat.h>
#include<unistd.h> // unlink
/* Max length a file path can have on storage */
#define FILE_PATH_MAX (15+32) //(ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

/* Max size of an individual file. Make sure this
 * value is same as that set in upload_script.html */
#define MAX_FILE_SIZE   (200*1024) // 200 KB
#define MAX_FILE_SIZE_STR "200KB"

/* Scratch buffer size */
#define SCRATCH_BUFSIZE  4096 //8192
struct file_server_data {
	/* Base path of file storage */
	char base_path[15 + 1]; //[ESP_VFS_PATH_MAX + 1];

	/* Scratch buffer for temporary storage during file transfer */
	char scratch[SCRATCH_BUFSIZE];
};
//---------------------------------------------------------------------
/* list Directory */
esp_err_t OTA_dirList_get_handler(httpd_req_t *req){
	browser.On();
	_DEBUG_Others_(3, "\n@@@ '%s'", (char* )req->uri);
	_DEBUG_Others_(3, "\nreq->user_ctx='%s'", (char* )req->user_ctx);
	_DEBUG_Others_(3, "\nserver_data='%s'", (struct file_server_data* )req->user_ctx);
	const char *param = skipStartUri(req->uri);
	std::string volume;
	std::string path;
	if (!split_Up_Before(2,param, volume, path)) {
		_DEBUG_Others_(3, "\n\t>Uri parameter '%s' incorrect", param);
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Volume name empty");
		return browser.Off(ESP_FAIL);
	}
	_DEBUG_Others_(3, "\n\tParam='%s' Volume='%s' Path='%s'",
		param, volume.c_str(), path.c_str());

	char *ledJSON = new char[15000]; // On the heap !!!
	char *buffer = ledJSON;

	//Serial.printf("\nOTA: %p", ledJSON);	Serial.printf("\nOTA: %p", buffer);
	buffer += sprintf(buffer, "{\"dirList\":\"");
	//Serial.printf("\nOTA: %p", buffer);
	listDir("HTML", &buffer, volume.c_str(), path.c_str());
	buffer += sprintf(buffer, "\"}");
	//Serial.printf("\nOTA: %p", buffer);

	Serial.printf("\nOTA> [%s]", ledJSON);
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, ledJSON, strlen(ledJSON));
	delete[] ledJSON; // don't forget to free after finished using it
	return browser.Off();
}
//---------------------------------------------------------------------
/* Status */
esp_err_t OTA_update_status_handler(httpd_req_t *req){

	browser.On();
	char ledJSON[256];

	Serial.printf("\nOTA< Status Requested");
	int16_t cnt = 0;
	cnt += sprintf((char*)&ledJSON + cnt, "{");
	cnt += sprintf((char*)&ledJSON + cnt, "\"status\":%d,", flash_status);
	cnt += sprintf((char*)&ledJSON + cnt, "\"bin_file\":\"%s\",", APPNAME);
	cnt += sprintf((char*)&ledJSON + cnt, "\"bin_time\":\"%s\",", __TIME__);
	cnt += sprintf((char*)&ledJSON + cnt, "\"bin_date\":\"%s\",", __DATE__);

	uint64_t totalBytes;
	uint64_t usedBytes;
	volSizes("/FFS", totalBytes, usedBytes);
	cnt += sprintf((char*)&ledJSON + cnt, "\"ffs_free\":%llu,", totalBytes - usedBytes);
	volSizes("/SSD", totalBytes, usedBytes);
	cnt += sprintf((char*)&ledJSON + cnt, "\"ssd_free\":%llu,", totalBytes - usedBytes);
	cnt += sprintf((char*)&ledJSON + cnt, "\"eoj\":\"end\"");
	cnt += sprintf((char*)&ledJSON + cnt, "}");
	Serial.printf("\nOTA> %s", ledJSON);
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, ledJSON, strlen(ledJSON));

	// This gets set when upload is complete
	if (flash_status == 1) {
		// We cannot directly call reboot here because we need the 
		// browser to get the ack back. 
		xEventGroupSetBits(reboot_event_group, REBOOT_BIT);
		return ESP_OK; // to avoid main loop runs with browser.Off()!!!
	}
	return browser.Off();
}
//---------------------------------------------------------------------
/* Receive .Bin file */
esp_err_t OTA_update_post_handler(httpd_req_t *req){
	browser.On();
	Serial.printf("\nOTA UPDATE req->content_len    '%i'", req->content_len);
	Serial.printf("\nOTA UPDATE req->uri            '%s'", req->uri);
	esp_ota_handle_t ota_handle;

	char ota_buff[1024];
	int content_length = req->content_len;
	int content_received = 0;
	int recv_len;
	bool is_req_body_started = false;
	const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);

	// Unsucessful Flashing
	flash_status = -1;

	do {
		/* Read the data for the request */
		if ((recv_len = httpd_req_recv(req, ota_buff, MIN(content_length, sizeof(ota_buff)))) < 0) {
			if (recv_len == HTTPD_SOCK_ERR_TIMEOUT) {
				ESP_LOGI("OTA", "Socket Timeout");
				/* Retry receiving if timeout occurred */
				continue;
			}
			ESP_LOGI("OTA", "OTA Other Error %d", recv_len);
			return ESP_FAIL;
		}

		Serial.printf("\nOTA RX: %d of %d\r", content_received, content_length);

		// Is this the first data we are receiving
		// If so, it will have the information in the header we need. 
		if (!is_req_body_started) {
			is_req_body_started = true;
			char ss = ota_buff[1000];
			ota_buff[1000] = 0;
			Serial.printf("\nOTA UPDATE '%s'", ota_buff);
			ota_buff[1000] = ss;

			// Lets find out where the actual data staers after the header info		
			char *body_start_p = strstr(ota_buff, "\r\n\r\n") + 4;
			int body_part_len = recv_len - (body_start_p - ota_buff);
			Serial.printf("\nOTA UPDATE body_part_len '%i'", body_part_len);

			//int body_part_sta = recv_len - body_part_len;
			//Serial.printf("\nOTA File Size: %d : Start Location:%d - End Location:%d\r\n", content_length, body_part_sta, body_part_len);
			Serial.printf("\nOTA File Size: %d\r\n", content_length);

			esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);
			if (err != ESP_OK) {
				Serial.printf("\nError With OTA Begin, Cancelling OTA\r\n");
				return ESP_FAIL;
			}
			else {
				Serial.printf("\nWriting to partition subtype %d at offset 0x%x\r\n",
					update_partition->subtype, update_partition->address);
			}

			// Lets write this first part of data out
			esp_ota_write(ota_handle, body_start_p, body_part_len);
		}
		else {
			// Write OTA data
			esp_ota_write(ota_handle, ota_buff, recv_len);

			content_received += recv_len;
		}

	} while (recv_len > 0 && content_received < content_length);

	// End response
	//httpd_resp_send_chunk(req, NULL, 0);

	if (esp_ota_end(ota_handle) == ESP_OK) {
		// Lets update the partition
		if (esp_ota_set_boot_partition(update_partition) == ESP_OK) {
			const esp_partition_t *boot_partition = esp_ota_get_boot_partition();
			// Webpage will request status when complete 
			// This is to let it know it was successful
			flash_status = 1;

			ESP_LOGI("OTA", "Next boot partition subtype %d at offset 0x%x",
				boot_partition->subtype, boot_partition->address);
			ESP_LOGI("OTA", "Please Restart System...");
		}
		else {
			ESP_LOGI("OTA", "\r\n\r\n !!! Flashed Error !!!");
		}

	}
	else {
		ESP_LOGI("OTA", "\r\n\r\n !!! OTA End Error !!!");
	}
	browser.Off();
	return ESP_OK;
}
//---------------------------------------------------------------------

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename)
	{
	if (IS_FILE_EXT(filename, ".pdf")) {
		return httpd_resp_set_type(req, "application/pdf");
	} else if (IS_FILE_EXT(filename, ".html")) {
		return httpd_resp_set_type(req, "text/html");
	} else if ((IS_FILE_EXT(filename, ".jpeg")) || (IS_FILE_EXT(filename, ".jpg"))) {
		return httpd_resp_set_type(req, "image/jpeg");
	} else if (IS_FILE_EXT(filename, ".ico")) {
		return httpd_resp_set_type(req, "image/x-icon");
	}
	/* This is a limited set only */
	/* For any other type always set as plain text */
	return httpd_resp_set_type(req, "text/plain");
}
//---------------------------------------------------------------------
/* Handler to delete a file from the server */
esp_err_t OTA_delete_get_handler(httpd_req_t *req){
	browser.On();
	char filepath[FILE_PATH_MAX];
	struct stat file_stat;

	_DEBUG_Others_(3, "\nreq->uri='%s'", (char* )req->uri);
	_DEBUG_Others_(3, "\nreq->user_ctx='%s'", (char* )req->user_ctx);
	_DEBUG_Others_(3, "\nserver_data='%s'", (struct file_server_data* )req->user_ctx);
	/* Skip leading "/delete" from URI to get filename */
	/* Note sizeof() counts NULL termination hence the -1 */
	const char *filename = get_path_from_uri(filepath,
		((struct file_server_data*)req->user_ctx)->base_path,
		req->uri + sizeof("/ota_delete") - 1, sizeof(filepath));
	_DEBUG_Others_(3, "\nFilename='%s'", filename);

	if (!filename) {
		/* Respond with 500 Internal Server Error */
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
		return browser.Off(ESP_FAIL);
	}

	/* Filename cannot have a trailing '/' */
	if (filename[strlen(filename) - 1] == '/') {
		ESP_LOGE(TAG, "Invalid filename : %s", filename);
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
			"Invalid filename: it looks like a folder name");
		return browser.Off(ESP_FAIL);
	}
	/*
	 if (stat(filepath, &file_stat) == -1) {
	 ESP_LOGE(TAG, "File does not exist : %s", filename);
	 // Respond with 400 Bad Request
	 httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File does not exist");
	 return browser.Off(ESP_FAIL);
	 }
	 */
	ESP_LOGI(TAG, "Deleting file : %s", filename);
	/* Delete file */
	if (!deleteFile(filename)) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
			" file doesn't exist.\nMaybe space or other in name?");
		return browser.Off(ESP_FAIL);
	}

	/* Redirect onto root to see the updated file list */
	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "/ota");
	/*
	 #ifdef CONFIG_EXAMPLE_HTTPD_CONN_CLOSE_HEADER
	 httpd_resp_set_hdr(req, "Connection", "close");
	 #endif
	 */
	httpd_resp_sendstr(req, "File deleted successfully");
	return browser.Off();
}

//---------------------------------------------------------------------
/* Handler to download a file kept on the server */
esp_err_t OTA_download_get_handler(httpd_req_t *req){
	browser.On();
	_DEBUG_Others_(3, "\n@@@ '%s'", (char* )req->uri);
	const char *volPathFilename = skipStartUri(req->uri);
	_DEBUG_Others_(3, "\n\tvolPathFilename='%s'", volPathFilename);
	if (!volPathFilename) {
		_DEBUG_Others_(3, "\n\t>Filename is empty");
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename is empty");
		return browser.Off(ESP_FAIL);
	}
	if (volPathFilename[strlen(volPathFilename) - 1] == '/') {
		_DEBUG_Others_(3, "\n\t>Filename is a folder");
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename is a folder");
		return browser.Off(ESP_FAIL);
	}
	struct stat file_stat;
	if (stat(volPathFilename, &file_stat) == -1) {
		// If file not present on the volume
		Serial.printf("\n\t>Err: file doesn't exist");
		// Respond with 404 Not Found
		httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File doesn't exist");
		return browser.Off(ESP_FAIL);
	}
	FILE *fd = nullptr;
	fd = fopen(volPathFilename, "r");
	if (!fd) {
		Serial.printf("\n\t>Failed reading file");
		/* Respond with 500 Internal Server Error */
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, " Failed to read existing file");
		return browser.Off(ESP_FAIL);
	}

	set_content_type_from_file(req, volPathFilename);

	/* Retrieve the pointer to scratch buffer for temporary storage */
	char *chunk = ((struct file_server_data*)req->user_ctx)->scratch;
	size_t chunksize;
	uint32_t sent = 0;
	_DEBUG_Others_(2, "\n\t File size %ld bytes", file_stat.st_size);
	do {
		chunksize = fread(chunk, 1, SCRATCH_BUFSIZE, fd);/* Read file in chunks into the scratch buffer */
		if (chunksize > 0) {
			_DEBUG_Others_(3, "\n\t %10ld bytes sent", sent);
			if (httpd_resp_send_chunk(req, chunk, chunksize) != ESP_OK) {/* Send the buffer contents as HTTP response chunk */
				fclose(fd);
				Serial.printf("\n\t>File sending failed!");
				httpd_resp_sendstr_chunk(req, NULL);/* Abort sending file */
				httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");/* Respond with 500 Internal Server Error */
				return browser.Off(ESP_FAIL);
			}
			sent += chunksize;
		}
	} while (chunksize != 0);/* Keep looping till the whole file is sent */
	_DEBUG_Others_(3, "\n\t %10ld bytes sent", sent);
	fclose(fd);/* Close file after sending complete */
	Serial.printf("\n\tDownload complete");

	/* Respond with an empty chunk to signal HTTP response completion */
	/*
	 #ifdef CONFIG_EXAMPLE_HTTPD_CONN_CLOSE_HEADER
	 httpd_resp_set_hdr(req, "Connection", "close");
	 #endif
	 */
	httpd_resp_send_chunk(req, NULL, 0);
	return browser.Off();
}
//---------------------------------------------------------------------
/* Handler to upload a file onto the server */
/**
 * @fn esp_err_t OTA_upload_post_handler(httpd_req_t *req)
 * @brief upload a file on the server file system (FFS or SSD)
 *
 * Unlike update xhr.responseType is not "blob" vs xhr.send(file) and not formdata,
 * there is no need to do a result.arrayBuffer().then((buffer) => {chn = "blob:" + String.fromCharCode.apply (null, new Uint8Array (buffer));});
 * @param req http request
 * @return esp status
 */
esp_err_t OTA_upload_post_handler(httpd_req_t *req){
	browser.On();
	const char *volPathFilename = skipStartUri(req->uri);
	/* Skip leading "/upload" from URI to get volPathFilename */
	/* Note sizeof() counts NULL termination hence the -1 */
	_DEBUG_Others_(3, "\n@@@ '%s'", (char* )req->uri);
	_DEBUG_Others_(3, "\n\tvolPathFilename='%s'", volPathFilename);
	if (!volPathFilename) {
		/* Respond with 500 Internal Server Error */
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename empty!!!");
		return browser.Off( ESP_FAIL);
	}

	/* Filename cannot have a trailing '/' */
	if (volPathFilename[strlen(volPathFilename) - 1] == '/') {
		_DEBUG_Others_(3, "\n\t>Invalid volPathFilename : %s", volPathFilename);
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Invalid volPathFilename");
		return browser.Off( ESP_FAIL);
	}
	struct stat file_stat;
	if (stat(volPathFilename, &file_stat) == 0) {
		_DEBUG_Others_(3, "\n\t>File already exists : %s", volPathFilename);
		/* Respond with 400 Bad Request */
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File already exists");
		return browser.Off( ESP_FAIL);
	}

	/* File cannot be larger than a limit */
	if (req->content_len > volFreeSize(volPathFilename)) {
		_DEBUG_Others_(3, "\n\t>File too large : %d bytes", req->content_len);
		/* Respond with 400 Bad Request */
		//httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
		//httpd_resp_set_hdr(req, "toto","blabla");
		//Serial.printf("\n%i HTTPD_400_BAD_REQUEST",
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File too large"); //);
		return browser.Off( ESP_FAIL); // Close underlying connection else the incoming file content will keep the socket busy
		/*
		 httpd_resp_sendstr(req,"File too large");
		 return ESP_FAIL;
		 */
	}
	FILE *fd = nullptr;
	fd = fopen(volPathFilename, "w");
	if (!fd) {
		_DEBUG_Others_(3, "\n\t>Failed to create file : %s", volPathFilename);
		/* Respond with 500 Internal Server Error */
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to create file");
		return browser.Off( ESP_FAIL);
	}

	_DEBUG_Others_(3, "\n\t-Receiving file : %s...", volPathFilename);

	/* Retrieve the pointer to scratch buffer for temporary storage */
	char *buf = ((struct file_server_data*)req->user_ctx)->scratch;
	int received;

	// content_len of the request gives the size headers + the file length being uploaded
	int remaining = req->content_len;
	while (remaining > 0) {
		_DEBUG_Others_(3, "\n\t Remaining size : %d", remaining);
		/* Receive the file part by part into a buffer */
		if ((received = httpd_req_recv(req, buf, MIN(remaining, SCRATCH_BUFSIZE))) <= 0) {
			if (received == HTTPD_SOCK_ERR_TIMEOUT) {
				_DEBUG_Others_(3, " timeOut");
				continue;/* Retry if timeout occurred */
			}
			/* In case of unrecoverable error,
			 * close and delete the unfinished file*/
			fclose(fd);
			int ferr = unlink(volPathFilename);
			if (ferr) {
				perror("unlink(Error receiving)");
			}
			_DEBUG_Others_(3, "\n\t>File reception failed!");
			/* Respond with 500 Internal Server Error */
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive file");
			return browser.Off( ESP_FAIL);
		}
		/* Write buffer content to file on storage */
		if (received && (received != fwrite(buf, 1, received, fd))) {
			/* Couldn't write everything to file!
			 * Storage may be full? */
			fclose(fd);
			unlink(volPathFilename);

			_DEBUG_Others_(3, "\n\t>File write failed!");
			/* Respond with 500 Internal Server Error */
			/*
			 httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
			 "Failed to write file to storage");
			 return ESP_FAIL;
			 */
			httpd_resp_sendstr(req, "Failed to write file to storage");
			return browser.Off( ESP_OK);

		}
		remaining -= received; // Keep track of remaining size of the file left to be uploaded
	}

	/* Close file upon upload completion */
	fclose(fd);
	_DEBUG_Others_(3, "\n\t-File reception complete");

	/* Redirect onto root to see the updated file list
	 httpd_resp_set_status(req, "303 See Other");
	 httpd_resp_set_hdr(req, "Location", "/ota");*/
	/*
	 #ifdef CONFIG_EXAMPLE_HTTPD_CONN_CLOSE_HEADER
	 httpd_resp_set_hdr(req, "Connection", "close");
	 #endif
	 */
	httpd_resp_sendstr(req, "File uploaded successfully");
	return browser.Off( ESP_OK);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
/**
 *
 * @param OTA_server handler
 * @param fsBasePath "/spiffs" or "/spiffs"
 */
void start_OTA_webservice(httpd_handle_t OTA_server, const char *PROGFILE){ //, const char *fsBasePath){
	// Create the reboot task
	xTaskCreate(&systemRebootTask, "rebootTask", 2048, NULL, 5, NULL);
	APPNAME = PROGFILE + String(PROGFILE).lastIndexOf("/") + 1;

	//Create the server context
	static struct file_server_data *server_data = nullptr;
	/*
	 // Validate file storage base path
	 if (!fsBasePath || strcmp(fsBasePath, "/spiffs") != 0) {
	 halt("\n File server presently supports only '/spiffs' as base path");
	 //return ESP_ERR_INVALID_ARG;
	 }
	 */
	if (server_data) {
		_DEBUG_Others_(3, "\nFile server already started");
		//return ESP_ERR_INVALID_STATE;
	}

	// Allocate memory for server data
	server_data = (file_server_data*)calloc(1, sizeof(struct file_server_data));
	if (!server_data) {
		halt("\nFailed to allocate memory for server data");
		//return ESP_ERR_NO_MEM;
	}
	/*
	 strlcpy(server_data->base_path, fsBasePath,
	 sizeof(server_data->base_path));
	 */server_data->base_path[0] = 0;

	// Set URI handlers

	_DEBUG_Others_(3, "\nRegistering URI handlers");
	httpd_uri_t OTA_home = {
		.uri = "/ota",
		.method = HTTP_GET,
		.handler = OTA_index_html_handler,
		.user_ctx = nullptr // Let's pass response string in user context to demonstrate it's usage
		};
	Serial.printf("\n%7x OTA_home", httpd_register_uri_handler(OTA_server, &OTA_home));

	httpd_uri_t OTA_update = {
		.uri = "/ota_update",
		.method = HTTP_POST,/* Handler to download a file kept on the server */
		.handler = OTA_update_post_handler,
		.user_ctx = nullptr
	};
	Serial.printf("\n%7x OTA_update", httpd_register_uri_handler(OTA_server, &OTA_update));

	httpd_uri_t OTA_upload = {
		.uri = "/ota_upload/*",
		.method = HTTP_POST,
		.handler = OTA_upload_post_handler,
		.user_ctx = server_data
	};
	Serial.printf("\n%7x OTA_upload", httpd_register_uri_handler(OTA_server, &OTA_upload));

	httpd_uri_t OTA_delete = {
		.uri = "/ota_delete/*",
		.method = HTTP_GET,
		.handler = OTA_delete_get_handler,
		.user_ctx = server_data // Pass server data as context
		};
	Serial.printf("\n%7x OTA_delete", httpd_register_uri_handler(OTA_server, &OTA_delete));

	httpd_uri_t OTA_listDir = {
		.uri = "/ota_dirList/*",
		.method = HTTP_GET,
		.handler = OTA_dirList_get_handler,
		.user_ctx = server_data // Pass server data as context
		};
	Serial.printf("\n%7x OTA_listDir", httpd_register_uri_handler(OTA_server, &OTA_listDir));

	httpd_uri_t OTA_download = {
		.uri = "/ota_download/*", // Match all URIs of type /path/to/file
		.method = HTTP_GET,
		.handler = OTA_download_get_handler,
		.user_ctx = server_data // Pass server data as context
		};
	Serial.printf("\n%7x OTA_download", httpd_register_uri_handler(OTA_server, &OTA_download));

	httpd_uri_t OTA_status = {
		.uri = "/ota_status",
		.method = HTTP_POST,
		.handler = OTA_update_status_handler,
		.user_ctx = nullptr
	};
	Serial.printf("\n%7x OTA_status", httpd_register_uri_handler(OTA_server, &OTA_status));
	/*
	 httpd_uri_t OTA_favicon_ico = {
	 uri = "/favicon.ico",
	 method = HTTP_GET,
	 handler = OTA_favicon_ico_handler,
	 user_ctx = nullptr // Let's pass response string in user context to demonstrate it's usage
	 };
	 httpd_register_uri_handler(OTA_server, &OTA_favicon_ico);

	 httpd_uri_t OTA_jquery_3_4_1_min_js = {
	 .uri = "/jquery-3.4.1.min.js",
	 .method = HTTP_GET,
	 .handler = jquery_3_4_1_min_js_handler,
	 .user_ctx = NULL // Let's pass response string in user context to demonstrate it's usage
	 };
	 httpd_register_uri_handler(OTA_server, &OTA_jquery_3_4_1_min_js);
	 */
}
///////////////////////////////////////////////////////////////////////
/*
 //---------------------------------------------------------------------
 // jquery GET handler
 esp_err_t jquery_3_4_1_min_js_handler(httpd_req_t *req){
 browser.On();
 ESP_LOGI("OTA", "jqueryMinJs Requested");

 httpd_resp_set_type(req, "application/javascript");
 browser.Off();

 //	httpd_resp_send(req, (const char *)jquery_3_4_1_min_js_start, (jquery_3_4_1_min_js_end - jquery_3_4_1_min_js_start)-1);

 return ESP_OK;
 }
 */
///////////////////////////////////////////////////////////////////////

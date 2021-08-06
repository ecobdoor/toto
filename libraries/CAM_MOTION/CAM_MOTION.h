#ifndef __000_MOTION__
#define __000_MOTION__
///////////////////////////////////////////////////////////////////////
#include "CAM_CAMERA.h"
#include "CAM_IMAGE.h"
///////////////////////////////////////////////////////////////////////
/**
 *
 *

 * gridLen for different gridStep					4²		5²		6²
 * RGB888 ( 640x480 )^3 OK buffer 0.921600 Mo	19200	12288
 * RGB888 ( 800x600 )^3 OK buffer 1.440000 Mo
 * RGB565 (1024x768 )^2 OK buffer 1.572864 Mo
 * RGB565 (1280x720 )^2 OK buffer 1.843200 Mo
 * MONO8  (1280×1024)^1 OK buffer 1.310720 Mo
 * MONO8  (1600x1200)^1 OK buffer 1.920000 Mo
 *
 */
/**
 * https://learn.upesy.com/en/programmation/psram.html
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/external-ram.html#
 */
bool SETUP_MOTION();
class c_Motion {
public:
	uint32_t cntloop = 0;
	uint32_t cnterror = 0;
	c_Motion(s_MOTIONctx *motionCTX) :
		ctx(*motionCTX){
	}
	bool Begin();
	inline uint8_t* curGrid() const{
		return _cur0;
	}
	inline uint8_t* prvGrid() const{
		return _prv0;
	}
	inline bool set_gridStep(const uint8_t STEP){
		ctx._gridStep = STEP;
		return 0;
	}
	inline uint8_t gridStep() const{
		return ctx._gridStep;
	}
	inline uint8_t get_minGridStep(const uint16_t WIDTH, const uint16_t HEIGHT){
		uint8_t sm = ceil(sqrt(((1.00 * WIDTH) * HEIGHT) / ctx._gridLen));
		//Serial.printf("\nminGridStep:%i=ceil(sqrt(%i x %i) / %i)", sm, WIDTH, HEIGHT, _gridLen);
		return sm;
	}
	inline uint8_t active() const{
		return ctx._active;
	}
	inline uint8_t set_active(const uint8_t ACTIVE){
		ctx._active = ACTIVE;
		return 0;
	}
	inline uint16_t gridLen() const{
		return ctx._gridLen;
	}
	inline bool set_absDelta(const uint8_t ABSDELTA){
		ctx._absDelta = ABSDELTA;
		return 0;
	}
	uint8_t absDelta() const{
		return ctx._absDelta;
	}
	inline bool set_relDelta(const float RELDELTA){
		ctx._relDelta = RELDELTA;
		return 0;
	}
	inline float relDelta() const{
		return ctx._relDelta;
	}
	inline bool set_ratioMax(const float RATIOMAX){
		ctx._ratioMax = RATIOMAX;
		return 0;
	}
	inline float ratioMax() const{
		return ctx._ratioMax;
	}
	/**
	 * @brief COUNTMAX € [0 65535]
	 * @param COUNTMAX
	 * @return 0 for server no error
	 */
	inline bool set_countMax(const uint16_t COUNTMAX){
		ctx._countMax = COUNTMAX;
		return 0;
	}
	inline uint16_t countMax() const{
		return ctx._countMax;
	}
	/**
	 * @brief SMOOTH € [0.0 > 1.0]
	 * @param SMOOTH
	 * @return 0 for server no error
	 */
	inline bool set_smooth(const float SMOOTH){
		ctx._smooth = SMOOTH;
		Serial.printf("\nset_smooth(%10.3f)", ctx._smooth);
		return 0;
	}
	inline float smooth() const{
//		Serial.printf("\nsmooth(%10.3f)", _smooth);
		return ctx._smooth;
	}
	inline bool set_marker(const t_tagMotion MARKER){
		ctx._marker = MARKER;
		return 0;
	}
	inline t_tagMotion marker() const{
		return ctx._marker;
	}
	inline bool set_dbgLevel(int8_t DBGLEVEL){
		ctx._dbgLevel = DBGLEVEL;
		return 0;
	}
	inline int8_t dbgLevel() const{
		return ctx._dbgLevel;
	}
	bool set_ftp(const bool FTP){
		ctx._sendFTP = FTP;
		return 0;
	}
	bool set_sms(const bool SMS){
		ctx._sendSMS = SMS;
		return 0;
	}
	bool sms(){
		return ctx._sendSMS;
	}
	bool ftp(){
		return ctx._sendFTP;
	}
private:
	uint8_t *_prv0 = nullptr;
	uint8_t *_cur0 = nullptr; // for Detect
	s_MOTIONctx &ctx;
};
void motionDetect(camera_fb_t *FRAME, const char *COMMENT = nullptr);
///////////////////////////////////////////////////////////////////////
/**
 * Eloquent class to detect motion in a stream of images
 *
 * @tparam _gridWidth width of the source image
 * @tparam _gridHeight height of the source image
 */
template<typename PIXL>
class MoveWatcher {
public:
	//---------------------------------------------------------
	/**
	 * @brief Instantiates the motion detector
	 * @param threshold <1 percent relative else absolute variation
	 * @param smooth transition smoothing factor
	 */
	/**
	 * @fn MoveWatcher(c_Image<PIXL> *IMAGE, const c_Motion *motionCtx)
	 * @param IMAGE to be processed
	 * @param motionCtx configuration parameters
	 */
	MoveWatcher(c_Image<PIXL> *IMAGE, const c_Motion *motionCtx) :
		_img(*IMAGE){
		const c_Motion &config = *motionCtx;
		_gridStep = config.gridStep();
		_marker = config.marker();
		_gridLen = config.gridLen();
		_gridWidth = _img.width() / _gridStep;
		_gridHeight = _img.height() / _gridStep;
		_gridArea = (uint32_t)_gridWidth * _gridHeight;
		_curr = config.curGrid();
		_prev = config.prvGrid();
		_smooth = config.smooth();
		_absThreshold = config.absDelta();
		_relThreshold = config.relDelta();
		/*
		 if (config.dbgLevel() > 3) {
		 printSysState();
		Serial.printf(
			"\n_gridStep %i,_marker %s, _img.width %i, _img.height %i, _gridWidth %i, _gridHeight %i, _gridArea %i",
			_gridStep, _marker ? "Yes" : "No", _img.width(), _img.height(), _gridWidth, _gridHeight,
			_gridArea);
		 }
		 */
		if (_gridLen < _gridArea)
			halt("Overflow _gridLen=%i<%i=_gridArea", _gridLen, _gridArea);
	}
//---------------------------------------------------------
	/**
	 * @fn uint8_t Decode()
	 * @brief Downsamples the image in a grid array according to a norm criterion
	 * @param image
	 * @return image medium brightness
	 */
	uint8_t Decode(){
//		printStackLeft(__FUNCTION__);
		// update previous
		dump(_prev, "PRV before");
		dump(_curr, "CUR before");
		uint8_t *prv = _prev;
		uint8_t *cur = _curr;
//		Serial.printf("\ndec %p %p", prv, cur);
		for (size_t i = 0; i < _gridArea; i++) {
			*prv = (1 - _smooth) * *cur + _smooth * *prv;
			cur++;
			prv++;
		}
//		printStackLeft(__FUNCTION__);
		uint8_t mediumBrightness = _img.Decode(_curr, _gridStep, _marker == tag_all); // always area<=Len
		dump(_prev, "PRV after");
		dump(_curr, "CUR after");
		return mediumBrightness;
	}
//---------------------------------------------------------
	/**
	 * @brief Detect how many pixels changed by at least a given threshold
	 * @param FRAME buffer, if !NULL draws changed areas on image
	 * @return
	 */
	int16_t Detect(){
		_absChanges = 0;
		_relChanges = 0;
		uint8_t *prv = _prev;
		uint8_t *cur = _curr;
//		for (uint16_t i = 0; i < _gridArea; i++) {in fact x,y to draw
		for (uint16_t y = 0; y < _gridHeight; y++)
			for (uint16_t x = 0; x < _gridWidth; x++) {
				float absErr = abs(*cur - *prv);
				float relErr = 0;
				if (0 < *prv)
					relErr = absErr / *prv; // €[0.00...255.0] ie. 0% 25500 %
				cur++;
				prv++;
				if (absErr >= _absThreshold) {
					_absChanges += 1;
					if (_marker >= tag_detect)
						_img.markRectangle(absErr, x * _gridStep, y * _gridStep, _gridStep,
							_gridStep);
				}
				if (relErr >= _relThreshold) {
					_relChanges += 1;
					if (_marker >= tag_detect) {
						relErr *= 255;
						_img.markCross(relErr, x * _gridStep, y * _gridStep, _gridStep,
							_gridStep);
					}
				}
			}
		return (_absChanges + _relChanges);
	}
//---------------------------------------------------------
	/**
	 * @brief Serial displays block matrix
	 * @param BLOCK
	 */
	void dump(uint8_t *grid, const char *COMMENT){
		return;
		Serial.printf("\n%s", COMMENT);
		Serial.printf("\n     ");
		for (size_t i = 0; i < _gridWidth; i++)
			Serial.printf("%4i", i);
		uint8_t *p = grid;
		for (size_t j = 0; j < _gridHeight; j++) {
			Serial.printf("\n %3i:", j);
			for (size_t i = 0; i < _gridWidth; i++)
				Serial.printf(" %3u", *p++);
		}
	}
//---------------------------------------------------------
	/**
	 * @brief Get the ratio of total( _absChanges + _relChanges) pixels that changed in the last frame
	 * @return
	 */
	inline float ratio(){
		return (1.0f * (_absChanges + _relChanges)) / _gridArea;
	}
//---------------------------------------------------------
	/**
	 * @brief Get total( _absChanges + _relChanges) number of pixels that changed
	 * @return
	 */
	inline uint16_t changes(){
		return (_absChanges + _relChanges);
	}
//---------------------------------------------------------
	/**
	 * @brief Get number of pixels which absolute change
	 * @return
	 */
	inline uint16_t absChanges(){
		return _absChanges;
	}
//---------------------------------------------------------
	/**
	 * @brief Get number of pixels which relative change
	 * @return
	 */
	inline uint16_t relChanges(){
		return _relChanges;
	}
//---------------------------------------------------------
protected:
	c_Image<PIXL> &_img;
	uint8_t _gridStep = 0;
	uint16_t _gridWidth = 0;
	uint16_t _gridHeight = 0;
	uint16_t _gridLen = 0;
	uint16_t _gridArea = 0;
	uint16_t _absChanges = 0;
	uint16_t _relChanges = 0;
	t_tagMotion _marker = tag_no;
	/**
	 * Current grid
	 */
	uint8_t *_curr;
	/**
	 * Previous grid
	 */
	uint8_t *_prev;
	/**
	 * Smoothing factor for transition from a frame to the next
	 * 0 means to completely discard the previous frame
	 * 1 means no update at all (useless)
	 */
	float _smooth;
	float _absThreshold;
	float _relThreshold;
};
///////////////////////////////////////////////////////////////////////
extern c_Motion motionCtx;
#endif

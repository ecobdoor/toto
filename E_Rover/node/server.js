///////////////////////////////////////////////////////////////////////
const http = require('http'),
    queryString = require('querystring');
const    fs = require('fs');
const path = require('path');
const os = require('os');

///////////////////////////////////////////////////////////////////////
/** *******************************************************************
 * @fn function context(ALLCONFIGS)
 * @param ALLCONFIGS json  file data
 * @return context object
 */
function context(ALLCONFIGS){
    const rover_NET=ALLCONFIGS.NET.rover_CFG;
    const pilot_NET=ALLCONFIGS.NET.pilot_CFG;
    console.log('========================')
    console.log('Current NET rover_NET : ',rover_NET)
    console.log('Current NET pilot_NET : ',pilot_NET)
    let ctxObj=    {
        "rover_ON"  :ALLCONFIGS.rover_ON,
        "pilot_comment" :ALLCONFIGS.NET[pilot_NET].pilot_comment,
        "pilot_URL" :ALLCONFIGS.NET[pilot_NET].pilot_URL,
        "pilot_PORT":ALLCONFIGS.NET[pilot_NET].pilot_PORT,
        "rover_comment" :ALLCONFIGS.NET[rover_NET].rover_comment,
        "rover_URL" :ALLCONFIGS.NET[rover_NET].rover_URL,
        "rover_PORT":ALLCONFIGS.NET[rover_NET].rover_PORT
    };
    console.log('------------------------')
    console.log(JSON.stringify(ctxObj,null,2))
    console.log('------------------------')
    var networkInterfaces = os.networkInterfaces();
//  console.log(networkInterfaces);
    for (var devName in networkInterfaces) {
        var iface = networkInterfaces[devName];
        for (var i = 0; i < iface.length; i++) {
            var alias = iface[i];
            if (alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal){
                console.log( alias.address);
                ctxObj.pilot_URL=alias.address;
            }
        }
    }
    console.log('========================')
    return ctxObj;
}
/** *******************************************************************
 * @fn const getContentType=function(URL)
 * @brief Gets the Content-Type of URL
 * @param URL request url
 * @return Content-Type (default: text/plain)
 */
const getContentType=function(URL){
	var ext=path.extname(URL);
    if(ext=='.html')
        return 'text/html';
    if(ext=='.css')
        return 'text/css';
    if(ext=='.js')
        return 'application/javascript';
    if(ext=='.mjs')
        return 'application/javascript';
	if(ext=='.json')
		return 'application/json';
	if(ext=='.gif')
		return 'image/gif';
    if(ext=='.jpg')
        return 'image/jpeg';
    if(ext=='.png')
        return 'image/png';
	if(ext=='.ico')
		return 'image/ico';
	return 'text/plain';
}
/** *******************************************************************
 * @fn const getParamsFromUrl = function(URL)
 * @brief Gets parameters of URL in an object {key:value [,key:value [,...]]}
 * 
 * Key '@' is the ressource sent in the response (generally a path to a file).
 * @n By decreasing priorities:
 * @n If the parameter pair  'req=<value>' exits, obj['@'] is overload by <value>
 * @n (useful in ccs file for style background-image: url("?req=/aFile.gif")
 * @n If ressource is empty or '/', index.html is sent.

 * @param URL request url
 * @return parameters object 
 */
const getParamsFromUrl = function(URL) {
    let obj = {};
    var url = decodeURI(URL);
    if (url.indexOf('?') >= 0) {
    //if (typeof url === 'string') {
        let params = url.split('?');
        if(params.length)
            obj['@']=params[0];
        let eachParamsArr = params[1].split('&');
        if (eachParamsArr && eachParamsArr.length) {
            eachParamsArr.map(param => {
                let keyValuePair = param.split('=')
                let key = keyValuePair[0];
                let value = keyValuePair[1];
                obj[key] = value;
            })
        }
        //  Test if a request is passed in parameters
        for (const [key, value] of Object.entries(obj)) {
            //console.log(`${key}: ${value}`);
            if(key==="req")
                obj['@']=value;
        }
    }
    else
        obj['@']=URL;
    if(obj['@']==='/')
        obj['@']='/index.html';
    console.log(URL,"   >> ",obj);
    return obj;
}
/** *******************************************************************
 *  @fn const requestListener = function (req, res)
 * @brief Listens the requests & sends the response to each one
 * 
 * Catch error 500 if there is an error
 * @n CORS is not authorized (Cf. https://gist.github.com/JohnEarnest/34b027c62eaf3c768b8963a4daa70457)
 * 
 * @param req request
 * @param res response
 * @return void
*/
const requestListener = function (req, res) {
    var params=getParamsFromUrl(req.url);
    var file=params['@'];

    res.setHeader('Access-Control-Allow-Origin', 'https://github.com');
    res.setHeader(
        "Access-Control-Allow-Headers",
        "Origin, X-Requested-With, Content-Type, Accept"
      );
      res.setHeader(
        "Access-Control-Allow-Methods",
        "GET, POST, PATCH, DELETE, OPTIONS"
      );
    console.log(file,">>",path.extname(file),'>>',getContentType(file));
    fs.promises.readFile(__dirname + file)
        .then(contents => {
            res.setHeader("Content-Type", getContentType(file));
            res.writeHead(200);
            res.end(contents);
        })
        .catch(err => {
            res.writeHead(500);
            res.end(err);
            return;
        });
};
/** *******************************************************************
 * @fn loadContext(FILENAME)
 * @brief Loads multi congiguration file
 * @param FILENAME
 * @return rawdata
 */
function loadContext(FILENAME) {
    console.log(FILENAME);
    try {
        const rawdata=  fs.readFileSync(FILENAME, 'utf8');
        console.log('"'+rawdata+'"');
        return rawdata
    } catch (err) {
        console.log('ERR: '+err);
//      return 'error loading '+FILENAME
    }
}
/** *******************************************************************
 * @fn saveJSconfig(FILENAME,OBJ)
 * @brief Saves congiguration needed by js pilot
 * @param FILENAME
 * @param OBJ congiguration
 * @return true, otherwise false if error
 */
function saveJSconfig(FILENAME,OBJ) {
    console.log(FILENAME);
    try {
        fs.writeFileSync(FILENAME, JSON.stringify(OBJ,null,2));
        console.log('"'+OBJ+'"');
        return true
    } catch (err) {
        console.log('ERR: '+err);
//      return 'error loading '+FILENAME
        return false
    }
}
///////////////////////////////////////////////////////////////////////
/**
 * @brief MAIN launches the PILOT node server listening at ${host}:${port}
 */
console.log('**********************************************');
let rawdata =loadContext('/media/DEV/ROB_000/GIT/TOTO/E_Rover/node/services.json')
console.log('**********************************************');

let allConfig = JSON.parse(rawdata);
console.log(allConfig);

const CTX=context(allConfig)
    if(saveJSconfig('rover.json',CTX)){
    const server = http.createServer(requestListener);
    server.listen(CTX.pilot_PORT, CTX.pilot_URL, () => {
        console.log(`##########################################`);
        console.log(`Server running at  ${CTX.pilot_URL}:${CTX.pilot_PORT}`);
        console.log(`  -ADMIN on http://${CTX.pilot_URL}:${CTX.pilot_PORT}`);
        console.log(`  -PILOT on http://${CTX.pilot_URL}:${CTX.pilot_PORT}/node_pil.html`);
        console.log(`##########################################`);
    });
}
///////////////////////////////////////////////////////////////////////

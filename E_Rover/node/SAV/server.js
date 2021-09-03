///////////////////////////////////////////////////////////////////////
console.log(`##########################################`);
const http = require('http'),
	queryString = require('querystring');;
const fs = require('fs').promises;
const path = require('path');
//const host = 'localhost';
const host = '192.168.13.12';
const port = 3001;
//=====================================================================
/*
    getContentType
*/
const getContentType=function(url){
	var ext=path.extname(url);
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
//=====================================================================
/*
    getParamsFromUrl

    if empty => /index.html
    if obj["req"] => obj['@']=obj["req"];
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
//=====================================================================
/*
    requestListener
   CORS:
    https://gist.github.com/JohnEarnest/34b027c62eaf3c768b8963a4daa70457
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
    fs.readFile(__dirname + file)
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

/*
        fs.readFile(__dirname + url, function(err, data) {
            if (err) {
                res.writeHead(404);
                return res.end("File not found.");
            }

            res.setHeader("Content-Type", mime.lookup(url)); //Solution!
            res.writeHead(200);
            res.end(data);
        });
    console.log(req);
    res.writeHead(200);
    res.end("<html><h1>Voilà la réponse du serveur !!!!!!!!!!!!!!!</h1></html>");
*/
};
//=====================================================================
/*
    MAIN
*/

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server running at  ${host}:${port}`);
    console.log(`  -ADMIN on http://${host}:${port}`);
    console.log(`  -PILOT on http://${host}:${port}/pil.html`);
    console.log(`##########################################`);
});

/*
const server = http.createServer((req, res) => {
    res.end('<html><h1>Voilà la réponse du serveur !!!!!!!!!!!!!!!</h1></html>');
});
server.listen(process.env.PORT || 3001);


##########################################
Server running on  localhost:3001
  -ADMIN on http://localhost:3001
  -PILOT on http://localhost:3001/pil.html
##########################################

*/
///////////////////////////////////////////////////////////////////////

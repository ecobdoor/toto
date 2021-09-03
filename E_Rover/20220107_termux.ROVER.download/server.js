///////////////////////////////////////////////////////////////////////
console.log('#############################');
const http = require('http'),
	queryString = require('querystring');;
const fs = require('fs').promises;
const path = require('path');
const host = 'localhost';
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
*/
const getParamsFromUrl = function(url) {
    url = decodeURI(url);
    if (typeof url === 'string') {
        let params = url.split('?');
        let eachParamsArr = params[1].split('&');
        let obj = {};
        if (eachParamsArr && eachParamsArr.length) {
            eachParamsArr.map(param => {
                let keyValuePair = param.split('=')
                let key = keyValuePair[0];
                let value = keyValuePair[1];
                obj[key] = value;
            })
        }
        return obj;
    }
}
//=====================================================================
/*
    requestListener
*/
const requestListener = function (req, res) {
    var oQueryParams;

    // get query params as object
    console.log(req.url);
    if (req.url.indexOf('?') >= 0) {
        oQueryParams = queryString.parse(req.url.replace(/^.*\?/, ''));

        // do stuff
        console.log(oQueryParams);
    }

	var url = /*convertURL*/(req.url);
    console.log(url,">>",path.extname(url),'>>',getContentType(url));
    fs.readFile(__dirname + url)
        .then(contents => {
            res.setHeader("Content-Type", getContentType(url));
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
    console.log(`Server is running on http://${host}:${port}`);
});

/*
const server = http.createServer((req, res) => {
    res.end('<html><h1>Voilà la réponse du serveur !!!!!!!!!!!!!!!</h1></html>');
});
server.listen(process.env.PORT || 3001);
*/
///////////////////////////////////////////////////////////////////////

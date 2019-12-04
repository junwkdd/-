var http = require('http');
var fs = require('fs');
const request = require('request');
var to_json = require('xmljson').to_json;

const KEY = '499B702221DEC1E4174FF462866A64DD';
var options;
var i;

for (i = 1; i <= 1000; i++) {
    options = {
        key : KEY,
        q: "나무",      // 소켓으로부터 건네받은 단
        pos: 1
    }
    
    request.get({
        uri: 'https://stdict.korean.go.kr/api/search.do',
        qs: options
    }, function(err, res, body) {
        if(err) {
            console.log("Error!");
        }
        to_json(body, function(err, data) {
            if(err) {
                console.log("tojson error!");
            }
            if(data.channel.total != 0) {       // 올바른 단어다
                
            }
        });
    });
}


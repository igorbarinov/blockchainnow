var express = require("express");
var sockio = require("socket.io");
var r = require("rethinkdb");


var app = express();
app.use(express.static(__dirname + "/public"));

var io = sockio.listen(app.listen(8099), {log: false});
console.log("Server started on port " + 8099);

var p = r.connect({host:'localhost', port:28015,db:'test'})
/*
p.then(function(conn) {
	console.log('Connection success')
	r.table('databricks').changes().run(conn, function(err, cursor) {
  		r.table('databricks').orderBy(r.desc('value')).limit(3).run(conn, function(err,cursor) {
  			cursor.each(console.log);
  		})
  		//cursor.each(console.log);
})
}).error(function(error) {
	console.log('error')
})*/

r.connect({db: "test"}).then(function(c) {
	r.table("databricks").get(100).changes().run(c)
		.then(function(cursor) {
			console.log('inside');
		});
	});

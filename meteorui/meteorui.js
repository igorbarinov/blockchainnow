Livenet = new Meteor.Collection("livenetnew4");

Livenet.initEasySearch(['txid'], {
  'limit' : 30,
  'use' : 'elastic-search',
  'query' : function (searchString) {
      var query = {"term" : { "txid" : searchString } };
      return query;
    } 
});


if (Meteor.isClient) {

  Template.bitcoinSearch.helpers({
    txsJSON: function() {
      return Session.get("txsJSON");
    }
  });

  
  Template.bitcoinSearch.created = function () {
    var instance = EasySearch.getComponentInstance(
      { id : 'search', index : 'livenetnew4' }
    );

    instance.on('searchingDone', function (searchingIsDone) {
      searchingIsDone && console.log('I am done!');
    });

    instance.on('currentValue', function (val) {
      console.log('The user searches for ' + val);
      var arr = [];

      if (val != undefined && val != null && val != "") {
 	EasySearch.search('livenetnew4', val, function (err, data) {
            if (data != undefined) {
              if (data.results.length > 0) {
                console.log("txid from Elastic search: " + data.results[0].txid);
                
                for (var i = 0; i < data.results.length; i++) {
                  console.log("#")
                  console.log(data.results[i])
                  console.log("#")
                  arr.push({"txid": JSON.stringify(data.results[i],null,4)});
                }
                console.log(arr);

		Session.set('txsJSON', arr);
              } else {
                console.log("Data length is 0");
                Session.set('txsJSON', []);
              }
            } else {
              console.log("Data is empty");
              Session.set('txsJSON', []);
            }
        });
       } else {
         console.log("Val is empty");
         Session.set('txsJSON', []);
       }
    });
  };
}

if (Meteor.isServer) {
  Meteor.startup(function () {
    // code to run on server at startup
    EasySearch.config({
        'host' : 'search.blocknotary.com:9200'
    });

  });
}

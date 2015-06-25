base58
======

An implementation of Base58 and Base58Check encodings for nodejs.  Note, the
implementation of Base58Check differs slightly from that described on Wikipedia
in that it does not prepend a version byte onto the data being encoded.  This
implementation uses a native module and big numbers from openssl. 

Installation
============

    npm install base58-native

Usage
=====

    var base58 = require('base58-native');
    base58.encode(base58.decode('mqqa8xSMVDyf9QxihGnPtap6Mh6qemUkcu'));

    var base58Check = require('base58-native').base58Check;
    base58Check.encode(base58Check.decode('mqqa8xSMVDyf9QxihGnPtap6Mh6qemUkcu'));

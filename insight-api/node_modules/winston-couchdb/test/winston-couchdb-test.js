/*
 * couchdb-test.js: Tests for instances of the Couchdb transport
 *
 * (C) 2011 Max Ogden
 * MIT LICENSE
 *
 */

var vows = require('vows'),
    transport = require('winston/test/transports/transport'),
    Couchdb = require('../lib/winston-couchdb').Couchdb;

vows.describe('winston/transports/couchdb').addBatch({
  'An instance of the Couchdb Transport': transport(Couchdb, {
    host: 'localhost',
    port: 5984,
    db: 'winston-couch-test'
  })
}).export(module);

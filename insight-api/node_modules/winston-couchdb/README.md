winston-couchdb
===============

A full featured CouchDB transport for winston

[![Build Status](https://travis-ci.org/indexzero/winston-couchdb.png)](https://travis-ci.org/indexzero/winston-couchdb)

## Install

```bash
npm i --save winston winston-couchdb
```

## Setup

```js
var winston = require('winston')
  , winstonCouch = require('winston-couchdb').Couchdb

winston.add(winstonCouch, {
  host: 'localhost'
  , port: 5984
  // optional
  , auth: {username: 'user', password: 'pass'}
  , secure: false
  , level: 'info'
})

```

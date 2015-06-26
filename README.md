Blockchain Now
==============

Agenda: End-to-end pipeline for the bitcoin analytics. Reach UI with reactive search and charts.

![UI](https://raw.githubusercontent.com/igorbarinov/blockchainnow/master/presentation/blockchain2.png)
Course project for Insight Data Engineering program

# How to install
TL;DR

Git clone the repository
```
git clone git@github.com:igorbarinov/blockchainnow.git
```
From **meteorui/** start meteor server
```
meteor
```
Open [http://localhost:3000](http://localhost:3000) in you favorite Google Chrome browser

When you will ready to publish to your hosting:
```
meteor publish $(echo "example")
```
change **example** to any desired (and free) hostname in *.meteor.com domain
e.g. [http://blockchainnow.meteor.com](http://blockchainnow.meteor.com)

# Technology Stack
- [Bitcoin Core](https://bitcoin.org/en/download) Bitcoin Core
- [Insight API](https://github.com/bitpay/insight-api) A bitcoin blockchain API for web wallets
- [Apache Kafka](http://kafka.apache.org) A high-throughput distributed messaging system
- [kafka-node](https://github.com/SOHU-Co/kafka-node) Node.js client with Zookeeper integration for Apache Kafka
- [Logstash](https://www.elastic.co/products/logstash) Collect, Parse, Transform Logs
- [Elastic Search](https://www.elastic.co/products/elasticsearch) Search & Analyze Data in Real Time
- [Meteor](http://www.meteor.com) The JavaScript App Platform
- [Meteor Easy Search](https://github.com/matteodem/meteor-easy-search) Plugin for Meteor

# Links
- [Blockchain Now](http://blockchainnow.meteor.com) "Blockchain Now" website
- [Slides](https://www.slideshare.net/secret/4Cb1dBG2iMlo2q)
- [Kafka Manager](http://bitcoind.blocknotary.com:9000/) 
- [Elastic Sample Query](http://search.blocknotary.com:9200/_search?q=txid:d5f2d21453a6f0e67b5c42959c9700853e4c4d46fa7519d1cc58e77369c893f2&pretty=true)


Important:
----------
Please star [Awesome Data Engineering](https://github.com/igorbarinov/awesome-data-engineering) repository

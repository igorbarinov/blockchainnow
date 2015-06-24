-- run once
create extension kafka;
-- insert broker information
insert into kafka.broker values ('localhost', 32768);
-- produce a message
select kafka.produce('test_topic', 'my message');
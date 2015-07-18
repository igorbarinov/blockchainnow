from pyspark import SparkContext, SparkConf
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils

# database
import rethinkdb as r
r.connect( "search.blocknotary.com", 28015).repl()

#color output
from clint.textui import colored, puts

def sendRecord(record):
	a = record.collect()
	for item in a:
		print "item[0]" + str(item[0])
		print "item[1]" + str(item[1])

		# run search
		cursor = r.table("databricks").filter(r.row['url'] == item[0]).run()
		# update or write new
		if cursor.items:
			r.table("databricks").filter(r.row['url'] == item[0]).update({"value":r.row['value']+1}).run()
		else:
			r.table("databricks").insert([{"url":item[0],"value":item[1]}]).run()


# create spark context
sc = SparkContext("local[2]",appName="databrickafterdark")
# create spark streamin context 10 sec
ssc = StreamingContext(sc, 1)

#lines = ssc.socketTextStream("localhost", 9999)
zkQuorum = "ec2-52-8-128-4.us-west-1.compute.amazonaws.com"
topic = "dark"
lines = KafkaUtils.createStream(ssc, zkQuorum, "spark-streaming-consumer", {topic: 1})

pairs = lines.map(lambda line: (line[1], 1))
wordCounts = pairs.reduceByKey(lambda x, y: x + y)


wordCounts.foreachRDD(sendRecord)




ssc.start()             # Start the computation
ssc.awaitTermination()  # Wait for the computation to terminate


"""
# zookeper 


lines = KafkaUtils.createStream(ssc, zkQuorum, "spark-streaming-consumer", {topic: 1})


#words = lines.flatMap(lambda line: line.split(" "))
pairs = lines.map(lambda word: (word, 1))
wordCounts = pairs.reduceByKey(lambda x, y: x + y)
print ("Counts: " + str(wordCounts)) 
wordCounts.pprint()

ssc.start()
ssc.awaitTermination()
"""
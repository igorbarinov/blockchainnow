import rethinkdb as r
import json
from pubnub import Pubnub

publish_key = "pub-c-231e447d-0590-482e-b601-ecbce1c97374"
subscribe_key = "sub-c-b0444770-28ee-11e5-af03-02ee2ddab7fe"
secret_key = "sec-c-NzYwMDQzM2QtNWEyYS00NjRkLWIwMDMtZjk4NGZiYTRkYjdi"
pubnub = Pubnub(publish_key=publish_key, subscribe_key=subscribe_key,
                secret_key=secret_key, pooling=False)

conn = r.connect( "search.blocknotary.com", 28015).repl()

feed = r.table('databricks').changes().run(conn)
for change in feed:
    #print change
    top = r.table("databricks").order_by(r.desc("value")).limit(4).run(conn)
    print pubnub.publish("first",(str(top[0]["url"]) + "," + str(top[0]["value"])))
    print pubnub.publish("second",(str(top[1]["url"]) + "," + str(top[1]["value"])))
    print pubnub.publish("third",(str(top[2]["url"]) + "," + str(top[2]["value"])))
    print pubnub.publish("fourth",(str(top[3]["url"]) + "," + str(top[3]["value"])))
    



# What we've done
## Text (Publishing and Subscribing)
We published 1000 messages using test.mosquitto.org as host and subscribed the messages in another terminal. We also created a counter to count messages received to make sure that no message is lost along the way.
![image](https://user-images.githubusercontent.com/87508144/132022442-01ccf664-9392-455c-b449-cbac216c5072.png)

## Pictures (Publishing and Subscribing) 
We did similiar thing, but this time we published 100 pictures instead. We observed that any picture with the size above 400kb won't reach the subscribing terminal.
**more updates soon**

## Cross-platform pub&sub
We tried publishing in Linux and subscribing in Windows. The result is that it can be cross-platformed.

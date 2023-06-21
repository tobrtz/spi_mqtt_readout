# spiReadout-mqtt-docker
## commands
<p>To pull the mosquitto - <i>docker pull eclipse-mosquitto:latest</i></p>
<p>To run the mosquitto on host -  <i>docker run --net=host -it -p 1883:1883 eclipse-mosquitto</i></p>
<p>To install mosquitto client - <i>sudo apt install mosquitto-clients</i></p>
<p>To subscribe the topic - <i>mosquitto_sub -h 127.0.0.1 -p 1883 -t iMotion/spi/data</i></p>

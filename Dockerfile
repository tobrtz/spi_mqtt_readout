FROM hypriot/rpi-node:latest
MAINTAINER Nandu Narayanan <nanduremo@gmail.com>
#get the source code from WiringPi github
RUN git clone https://github.com/WiringPi/WiringPi.git
#get the source code from spiReadout nandu's github
RUN rm -rf spiReadout-mqtt-docker
RUN git clone https://github.com/nandunarayanan/spiReadout-mqtt-docker.git
#build the WiringPi inside docker image
RUN cd WiringPi && ./build
#build the spiReadout program inside docker image
RUN cd spiReadout-mqtt-docker && g++ main.c mqtt.c -l wiringPi -o output
#set the working directory as final spiReadout executable directory
WORKDIR /spiReadout-mqtt-docker
#Run it !!!!
CMD ["./output"]

#Docker build instruction for this Dockerfile is as below;
#########################################################
#     docker build -t <image-name> <Dockerfile-path>    #
#     ex: docker build -t mywire .                      #     
#########################################################

#Docker run instruction (for image created above) is as below;
#######################################################################################################################
#     docker run --device /dev/ttyAMA0:/dev/ttyAMA0 --device /dev/mem:/dev/mem --privileged --net=host -ti mywire     #
#     ex: docker run --device /dev/ttyAMA0:/dev/ttyAMA0 --device /dev/mem:/dev/mem --privileged --net=host -ti mywire #     
#######################################################################################################################

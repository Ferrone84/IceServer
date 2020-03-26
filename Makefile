CXX=g++
CXXFLAGS=-std=c++11 -I cpp/headers -DICE_CPP11_MAPPING
LDFLAGS=-lIce++11 -lvlc -pthread

dir=cpp/
dirpy=python/
dirjava=java/
headers=headers/
src=sources/
obj=obj/
projet=serv

${projet} : ${dir}${obj}main.o ${dir}${obj}Server.o ${dir}${obj}IServer.o
	${CXX} -o ${projet} -g $^ ${LDFLAGS}

${dir}${obj}main.o : ${dir}${src}main.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $<

${dir}${obj}Server.o : ${dir}${src}Server.cpp ${dir}${headers}Server.h
	${CXX} ${CXXFLAGS} -o $@ -c $<

${dir}${obj}IServer.o : ${dir}${src}IServer.cpp ${dir}${headers}IServer.h
	${CXX} ${CXXFLAGS} -o $@ -c $<

ice:
	slice2cpp IServer.ice
	mv IServer.cpp ${dir}${src}
	mv IServer.h ${dir}${headers}
	slice2py IServer.ice
	mv IServer_ice.py ${dirpy}
	slice2java IServer.ice

pythonclient:
	python ${dirpy}client.py

javaclient:
	java -cp :./${dirjava}classes:./lib/ice-3.7.0.jar Client

makejava:
	javac -d ${dirjava}classes -cp ${dirjava}classes:./lib/ice-3.7.0.jar ${dirjava}*.java server/*.java

clean:
	rm -f ${dir}${obj}*
mrproper: clean
	rm ./${projet}

install:
	sudo apt-get install zeroc-ice-all-runtime zeroc-ice-all-dev
	sudo apt-get install libvlccore-dev libvlc-dev
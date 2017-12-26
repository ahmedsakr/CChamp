LIB_NAME= libcchamp.so.4
LIB_SOFT_LINK= libcchamp.so

HEADERS_DIR= include/cchamp
HEADERS_INSTALL_DIR= /usr/local/include
DYNAMIC_INSTALL_DIR= /usr/local/lib
SOFT_LINK_DIR=		 /usr/lib/x86_64-linux-gnu

compile-proper:
	gcc -Isrc -Iinclude -Llib -fpic -c `find src -name "*.c"` -lcurl

dynamic: compile-proper
	gcc -shared -fpic -Wl,-soname,${LIB_NAME} -o ${LIB_NAME} *.o -lc

headers-install: ${HEADERS_DIR}/*.h
	sudo cp -r ${HEADERS_DIR} ${HEADERS_INSTALL_DIR}

dynamic-install: dynamic
	sudo mv -f ${LIB_NAME} ${DYNAMIC_INSTALL_DIR}
	sudo ln -f -s ${DYNAMIC_INSTALL_DIR}/${LIB_NAME} ${SOFT_LINK_DIR}/${LIB_SOFT_LINK}

clean:
	rm -f *.so* *.o

.PHONY: headers-install dynamic-install

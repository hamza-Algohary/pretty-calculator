NAME = pretty-calculator
APP_NAME = "Pretty Calculator"
CXX = g++
CXX_FLAGS = `pkg-config --libs --cflags gtkmm-3.0`
build:
	-mkdir bin
	${CXX} src/main.cpp src/graph.cpp src/text_editor.cpp ${CXX_FLAGS} -o bin/${NAME}

build-pkgs:
	
	-mkdir packages
	-mkdir packages/debian/
	-mkdir packages/debian/${NAME}/
	-mkdir packages/debian/${NAME}/usr/
	-mkdir packages/debian/${NAME}/usr/bin
	-mkdir packages/debian/${NAME}/usr/share/
	-mkdir packages/debian/${NAME}/usr/share/applications
	-mkdir packages/debian/${NAME}/DEBIAN
	-mkdir packages/tarball
	-mkdir packages/tarball/${NAME}

	-rm packages/debian/${NAME}/DEBIAN/control
	-rm packages/tarball/${NAME}/install.sh
	-rm packages/tarball/${NAME}/install.sh
	-rm ${NAME}.desktop
	-rm /packages/debian/pretty-calculator/usr/share/applications/${NAME}.desktop
	-rm /packages/tarball/pretty-calculator/${NAME}.desktop

	touch packages/debian/${NAME}/DEBIAN/control
	touch packages/tarball/${NAME}/install.sh
	touch packages/tarball/${NAME}/install.sh
	touch ${NAME}.desktop
	touch packages/tarball/${NAME}/uninstall.sh
	touch packages/tarball/${NAME}/install.sh

	-echo "[Desktop Entry]" >> ${NAME}.desktop
	-echo "Name=${APP_NAME}"  >>  ${NAME}.desktop
	-echo "Comment=A simple and beautiful calculator." >>  ${NAME}.desktop
	-echo "Keywords=calculation;arithmetic;scientific;financial;" >>  ${NAME}.desktop
	-echo "Exec=${NAME}" >>  ${NAME}.desktop
	-echo "Icon=org.gnome.Calculator" >>  ${NAME}.desktop
	-echo "Terminal=false" >>  ${NAME}.desktop
	-echo "Type=Application" >>  ${NAME}.desktop

	-echo "Package: ${NAME}" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Version: 1.3" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Section: utilities" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Priority: optional" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Architecture: amd64" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Essential: no" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Installed-Size: 1024" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Maintainer: hamza" >> packages/debian/${NAME}/DEBIAN/control
	-echo "Description: A simple and beautiful calculator. " >> packages/debian/${NAME}/DEBIAN/control
	-echo "Depends: libgtkmm-3.0-dev , libboost-dev" >> packages/debian/${NAME}/DEBIAN/control


	-echo "cp ${NAME} /usr/bin/${NAME}" >> packages/tarball/${NAME}/install.sh
	-echo "cp ${NAME}.desktop /usr/share/applications/${NAME}.desktop" >> packages/tarball/${NAME}/install.sh 
	-echo "rm /usr/bin/${NAME}" >> packages/tarball/${NAME}/uninstall.sh
	-echo "rm /usr/share/applications/${NAME}.desktop" >> packages/tarball/${NAME}/uninstall.sh 


	-cp bin/${NAME} packages/debian/${NAME}/usr/bin/${NAME}
	-cp ${NAME}.desktop ./packages/debian/${NAME}/usr/share/applications/${NAME}.desktop
	dpkg-deb --build packages/debian/${NAME}


	-cp ./bin/${NAME} ./packages/tarball/${NAME}/${NAME}
	-cp ${NAME}.desktop ./packages/debian/${NAME}.desktop
	tar -czf packages/tarball/${NAME}.tar.gz packages/tarball/${NAME}

install: 
	cp ${NAME}.desktop /usr/share/applications
	cp bin/${NAME} /usr/bin/${NAME}

uninstall:
	rm ${NAME}.desktop
	rm bin/${NAME}

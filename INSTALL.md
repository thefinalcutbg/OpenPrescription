# How to build:
Cmake is not yet provided, so you have to setup everything by yourself (or use the included Visual Studio project file).
Add the src folder to your IDE of choice. Be sure to have Qt6 installed and integrated into your IDE.

## Building dependencies:

### SqLite3, TinyXML and JsonCpp

Download the sources of the libraries from their respective github repos and include them in your project. When manipulating the database, OpenPrescription assumes sqlite3 is compiled with "always-on" foreign key constraints, so you have to make that change in the sqlite3 header.

### OpenSsl

Provide Openssl for your system. OpenSsl is a dependency of XmlSec and Libp11. The project has been tested with OpenSsl 1.1.1 distributed by the Qt Maintaince tool.

### LibXml2

This library is a dependency of XmlSec and it is required for the canonnicalization of the xml file during the signing. Download the latest release from [https://github.com/GNOME/libxml2/](https://github.com/GNOME/libxml2/)
It is strongly advisable to build it without libxslt and iconv to reduce dependency hell. If you are on Windows, start configure.js from the win32 folder with cscript from the console and set the parameters and the desired directories using this command:
```
cscript configure.js iconv=no xslt=no include=c:\YourIncludeDir lib=c:\YourBinariesDir
```

Then type:
```
nmake install
```

Wait for the build process to finish. This generates several binaries, but the one you need is called libxml2. The binaries whose names end _a are not required.


### XmlSec

The library is required for generating Xml signatures of xml queries sent to NHIF and NHIS. Download the latest release from [https://github.com/lsh123/xmlsec](https://github.com/lsh123/xmlsec)

There is an optional dependency on LibXslt which again is preferable to be omitted. To build XmlSec on Windows, navigate to the win32 folder of the and start configure.js from the command prompt with:
```
cscript configure.js iconv=no xslt=no static=no include=C:\Dev\xmlTools\include lib=C:\Dev\xmlTools\lib 
```
The script forces you to place BOTH the openssl and libxml2 include and lib folders in the directories configured above. Watch out for the folder names. Libxml include files by default are found in include/libxml2/libxml, but Xmlsec will search only for the libxml folder, so copy only that one to the include dir which you have specidied with the cscript. After you've configured the directories correctly, build the xmlsec using nmake. As for the include files, I've configured the header files with the following macros:
```
#define XMLSEC_NO_SIZE_T //in xmls.h
#define XMLSEC_CRYPTO_OPENSSL //in crypto.h
#define XMLSEC_NO_XSLT //in transforms.h
```
### Libp11

This library is responsible for the PKCS11 interface. Go to [https://github.com/OpenSC/libp11](https://github.com/OpenSC/libp11) and download the latest release. Build the library following the instructions. After the build process is finished, there will be 2 binaries - libp11 and pkcs11



If you have followed everything  as described, you'll have all of the required binaries:

- libcrypto (OpenSSL)

- libssl (OpenSSL)

- libxml2 (LibXml2)

- libxmlsec (XmlSec)

- libxmlsec-openssl (XmlSec)

- libp11 (Libp11)

- pkcs11 (Libp11)


## Building OpenPrescription itself

Edit the additional include and link directories in your project settings and compile.

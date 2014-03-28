///////////////////////////////////////////////////////////
//  InitDamisServiceFile.h
//  Implementation of the Class InitDamisServiceFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(INITDAMISSERVICEFILE_H)
#define INITDAMISSERVICEFILE_H

#include "ArffFile.h"
#include <string>

struct FtpFile {
		std::string filename;
		FILE *stream;
	};


/**
 * Class that represents Damis service working file
 */
class InitDamisServiceFile : public ArffFile
{

public:

	InitDamisServiceFile(std::string fPath, std::string pref);
	InitDamisServiceFile(std::string pref);
	virtual ~InitDamisServiceFile();
	std::string getDownloadFileURI();



private:
    std::string downloadFileURI;
	bool checkForHttpError();
	bool downloadFile();
    bool initialize();
    static size_t fileWriteDelegeate(void *buffer, size_t size, size_t nmemb, void *stream);

};
#endif // !defined(INITDAMISSERVICEFILE_H)

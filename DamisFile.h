///////////////////////////////////////////////////////////
//  DamisFile.h
//  Implementation of the Class DamisFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (DAMISFILE_H)
#define DAMISFILE_H

#include <string>
    /**
	 * Class that describes each file (local, http paths and file name of the input/output file defined by the prefix that is passed by creating class object)
	 */
class DamisFile
{

public:
	virtual ~DamisFile();
	/**
	 * Constructor that accepts prefix of the file name to be generated
	 */
	DamisFile(std::string preffix);
    /**
	 * Method returns file name
	 */
	std::string getFileName();
    /**
	 * Method returns file name with the local path
	 */
	std::string getFilePath();
    /**
	 * Method returns file name with the http path
	 */
	std::string getHttpPath();


private:
	/**
	 * File name
	 */
	std::string fileName;
	/**
	 * File path
	 */
	std::string filePath;
	/**
	 * File http path
	 */
	std::string fileHttpPath;

};
#endif // DAMISFILE_H

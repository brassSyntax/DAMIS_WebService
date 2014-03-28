///////////////////////////////////////////////////////////
//  DamisFile.cpp
//  Implementation of the Class DamisFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#include "DamisFile.h"
#include "HelperMethods.h"
#include "ServiceSettings.h"

DamisFile::~DamisFile(){

}

/**
 * class constructor
 */
DamisFile::DamisFile(std::string preffix){
//Generate input file name with passed preffix
DamisFile::fileName = HelperMethods::generateFileName(preffix);
//Add generated file name to sorage path
DamisFile::filePath = ServiceSettings::localDataFileSavePath + DamisFile::fileName;
//Add generated file name to the http path
DamisFile::fileHttpPath =ServiceSettings::httpDataFileSavePath + DamisFile::fileName;


}

/**
 * Returns file name
 */
std::string DamisFile::getFileName(){

	return  fileName;
}
/**
 * Returns local file path
 */
std::string DamisFile::getFilePath(){

	return  filePath;
}
/**
 * Returns http link to file
 */
std::string DamisFile::getHttpPath(){

	return  fileHttpPath;
}


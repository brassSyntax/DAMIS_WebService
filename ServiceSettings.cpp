///////////////////////////////////////////////////////////
//  ServiceSettings.cpp
//  Implementation of the Class ServiceSettings
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#include "ServiceSettings.h"


ServiceSettings::ServiceSettings(){

}

ServiceSettings::~ServiceSettings(){

}

//Debug DAMIS
	std::string ServiceSettings::wsdlPath = "C:/inetpub/wwwroot/Damis/TestDamis.wsdl";
	std::string ServiceSettings::logingConfFilePath = "C:/inetpub/wwwroot/Damis/cgi-bin/testlogging.conf";
    std::string ServiceSettings::pathToAlgorithmsExe = "\"D:/Povilas Data/My Documents/CodeBlocks/ClusterAlgorithms/bin/Debug/ClusterAlgorithms.exe\"";
//*/

//Release DAMIS
/*	std::string ServiceSettings::wsdlPath = "C:/inetpub/wwwroot/Damis/Damis.wsdl";
	std::string ServiceSettings::logingConfFilePath = "C:/inetpub/wwwroot/Damis/cgi-bin/logging.conf";
	std::string ServiceSettings::pathToAlgorithmsExe = "\"D:/Povilas Data/My Documents/CodeBlocks/ClusterAlgorithms/bin/Release/ClusterAlgorithms.exe\"";
// */

	std::string ServiceSettings::localDataFileSavePath = "C:/inetpub/wwwroot/Damis/Data/";

	std::string ServiceSettings::httpDataFileSavePath = "http://158.129.140.146/Damis/Data/";

	std::string ServiceSettings::dataFileExtension = ".arff";

	int ServiceSettings::lenghtOfTmpFileName = 20;

	int ServiceSettings::keepFilesInDirectory = 2*24*60*60;

	int ServiceSettings::noOfProcessors = 3;

	int ServiceSettings::runDestination = 1;

	std::string ServiceSettings::pathToMPIExecutable = "\"D:/Povilas Data/My Documents/CodeBlocks/ClusterAlgorithms/mpi_x86/mpiexec.exe\"";

	std::string ServiceSettings::workingDirMPI = "\"C:/inetpub/wwwroot/Damis/Data/\"";


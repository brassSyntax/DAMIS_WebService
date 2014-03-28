///////////////////////////////////////////////////////////
//  ServiceSettings.h
//  Implementation of the Class ServiceSettings
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(SERVICESETTINGS_H)
#define SERVICESETTINGS_H
#include <string>

/**
 * Class holds static attributes with the service settings
 */
class ServiceSettings
{

public:
	ServiceSettings();
	virtual ~ServiceSettings();
	/**
	 * Local file path to store downloaded files
	 */
	static std::string localDataFileSavePath;
	/**
	 * Path to public HTTP directory
	 */
	static std::string httpDataFileSavePath;
	/**
	 * Default file extension
	 */
	static std::string dataFileExtension;
	/**
	 * Path to logging config file
	 */
	static std::string logingConfFilePath;
	/**
	 * Lenght of random file name
	 */
	static int lenghtOfTmpFileName;
	/**
	 * Time in seconds how long data files will be kept in directory
	 */
	static int keepFilesInDirectory;
	/**
	 * //Release DAMIS
	 * //std::string appSettings::wsdlPath = "C:/inetpub/wwwroot/Damis/Damis.wsdl";
	 */
	static std::string wsdlPath;
	/**
	 *Number of processors that mey be used by he algorithms
	 */
	 static int noOfProcessors;
	 /**
	 *Indicates where the algorithms must be run - 1 MII cluster, 2 - VU MIF cluster
	 */
	 static int runDestination;
	 /**
	 *Path to mpi.exe or mpiexec.exe
	 */
	 static std::string pathToMPIExecutable;
	 /**
	 *MPI working directory
	 */
	 static std::string workingDirMPI;
     /**
	 *Path to cluster algorithms implementation exe
	 */
	 static std::string pathToAlgorithmsExe;
};
#endif // !defined(SERVICESETTINGS_H)

///////////////////////////////////////////////////////////
//  CallCalculus.cpp
//  Implementation of the Class CallCalculus
//  Created on:      24-Vas-2014 16:29:02
//  Original author: Povilas
///////////////////////////////////////////////////////////
#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE


#include "CallCalculus.h"
#include "ServiceSettings.h"
#include "ServeRequest.h"
#include "HelperMethods.h"
#include "logging/easylogging++.h"
#include <iostream>
//#include <windows.h>
//#include <tchar.h>
#include <unistd.h>
#include <wait.h>
#include "ErrorResponse.h"

CallCalculus::CallCalculus(int p, int maxCalcTime, InitDamisService* initFile):ServeRequest(initFile)
{

    this->reqProcessors = p;
    this->maxCalculationTime = maxCalcTime; // not used if qsub is not called
}


CallCalculus::~CallCalculus()
{

}


/**
 * Implements run of the algorithm
 */
void CallCalculus::run()
{
    // create file for service calculations
    DamisFile *cFile = new DamisFile ("_forCalculus_");  // temporary data file that will be passed to cluster
    //DamisFile *algStat = new DamisFile ("_algRunStat_"); //temporary data file that hold data statistitics abaut algorithm run
    //variable ir ->>>>>statFile

    // std::vector<std::string> dummy;
    // dummy.reserve(0); //pass dummy vector to write function since no class attribute after  are left
    // save data to temporary file
    writeDataToFile(cFile->getFilePath(), prepareDataSection(serveFile->getDoubleData(), serveFile->getStringClass()), prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(), serveFile->getStringClassAttribute()));

    std::string callCalc;


    if (ServiceSettings::runDestination == 1) //run on MII cluster
    {


        //LOG(INFO) << "CALL: " << exeParams;
        //LOG(INFO)

        ///Implementation for windows run
        //    std::string callCalc = ServiceSettings::pathToMPIExecutable + " -n " + std::to_string(reqProcessors) + " -wdir " + ServiceSettings::workingDirMPI + " " + ServiceSettings::pathToAlgorithmsExe + exeParams + " -i " +cFile->getFilePath() + " " + " -o " + outFile->getFilePath() + " " + " -s " + statFile->getFilePath();

        /* STARTUPINFO si = { sizeof(si) };
         PROCESS_INFORMATION  pi;
         std::vector<TCHAR> V( callCalc.length() + 1);
         for (int i=0;i< (int) callCalc.length();i++)
             V[i] = callCalc[i];

         CreateProcess(NULL, &V[0],0, 0, FALSE, 0, 0, 0, &si, &pi);
         WaitForSingleObject(pi.hProcess, INFINITE);
         CloseHandle(pi.hProcess);
         CloseHandle(pi.hThread);*/
        ///Implementation for linux run
     //   char * tmpNoOfProc = const_cast<char*>(std::to_string(static_cast<long long>(reqProcessors)).c_str());
        //creating bash script with the parameters
        callCalc = ServiceSettings::pathToMPIExecutable + " -np " + std::to_string( static_cast<long long> (reqProcessors)) + " -wdir " + ServiceSettings::workingDirMPI + " " + ServiceSettings::pathToAlgorithmsExe + exeParams + " -i " +cFile->getFilePath() + " -o " + outFile->getFilePath() + " -s " + statFile->getFilePath();

        std::string fileName = HelperMethods::generateFileName("", false);
        std::string bashFile = ServiceSettings::workingDirMPI + "_bash_" + fileName + ".sh";
        std::string qsubOut = ServiceSettings::workingDirMPI + "_qsubOut_log_" + fileName + ".txt";
        std::string qsubOutO = ServiceSettings::workingDirMPI + "_qsubOut_log_Out_" + fileName + ".txt";
        std::string qsubOutE = ServiceSettings::workingDirMPI + "_qsubOut_log_Err_" + fileName + ".txt";
        std::string bashFileUpper = ServiceSettings::workingDirMPI + "_bash_run_" + fileName + ".sh";

        std::ofstream file(bashFile.c_str());

        file << "#!/bin/bash"<<std::endl;
        file<<"#"<<std::endl;
        file<<"#$ -j y"<<std::endl;
        file<<"#$ -o "<< qsubOutO << std::endl;
        file<<"#$ -e "<< qsubOutE << std::endl;
        file << "#$ -pe orte " << std::to_string( static_cast<long long> (reqProcessors)) << std::endl;
        file<<callCalc<<std::endl;
        file.close();

        std::ofstream fileUpper(bashFileUpper.c_str());
        fileUpper <<"export SGE_ROOT=/opt/gridengine"<<std::endl;
        fileUpper <<"export SGE_CELL=default"<<std::endl;
        fileUpper <<"export SGE_QMASTER_PORT=536"<<std::endl;
        fileUpper <<"export SGE_EXECD_PORT=537"<<std::endl;
        fileUpper <<"/opt/gridengine/bin/linux-x64/qsub "<< bashFile  << " > " <<qsubOut << std::endl;
        fileUpper.close();

        char * tmpBashFileUpper = const_cast<char*>(bashFileUpper.c_str());
        std::string laikUpper = "chmod 707 " + bashFileUpper;
        char * commandUpper =  const_cast<char*>(laikUpper.c_str());
        system (commandUpper);

        char* argList[] =
        {
            tmpBashFileUpper,
            NULL /*       The argument list must end with a NULL.*/
        };

        /* Spawn a child process running the "ls" command. Ignore the
           returned child process ID.  */
        LOG(INFO) << "Submiting job to qsub" << tmpBashFileUpper;
        spawn (tmpBashFileUpper, argList);

        //check qsub output if it has submitted then wait for calculus tu finish else return error codes as damis error
       //
            std::ifstream oFile(qsubOut);
            std::string line_from_file;
       // bool jobKilled = false;

            if (oFile.good())
            {
                std::getline(oFile, line_from_file);
                oFile.close();

                std::size_t foundJob = line_from_file.find("Your job ");

                if (foundJob!=std::string::npos)
                {
                    //find job id
                    std::size_t foundJobIDStart = line_from_file.find_first_of("0123456789");
                    std::size_t foundJobIDEnd  = line_from_file.find(" ", foundJobIDStart + 1);
                    std::string jobID = line_from_file.substr(foundJobIDStart, foundJobIDEnd - foundJobIDStart);
                    LOG(INFO) << "Job ID is " << jobID;

                   /* std::string runCommand = "qstat -j " + jobID + " 2>&-"; //force only stderr to null
                    char * command = const_cast<char*>(runCommand.c_str());

                    std::string qstatus = "";
                    do
                    {
                        system("sleep 10");
                        qstatus = getOutput(command);
                        if (qstatus == "ERR")
                        {
                            LOG(ERROR) << "Error getting pipe to "<< runCommand << " killing job";
                            ErrorResponse::setFaultDetail("Error job status monitoring, killing job");
                            //error getting pipe
                            runCommand = "qdel " + jobID + " &>/dev/null"; //forse all output to null
                            command = const_cast<char*>(runCommand.c_str());
                            system (command);
                            break;
                        }
                    }
                    while (qstatus!=""); // indicates that the job has been finished*/

                    bool stopChecking = false;
					std::string oF = outFile->getFilePath();
					std::string sF = statFile->getFilePath();

					do
					{
						system ("sleep 5");
						//checking if there are these files
						std::ifstream calcFile(oF.c_str());  //output from algorithms
						std::ifstream statFile(sF.c_str()); //statistics from the calculus
						std::ifstream qsubError(qsubOutE);
						std::ifstream qsublogOut(qsubOutO); //May occur errors from the Algorithms

						if (qsubError)
						{
							std::streampos current = qsubError.tellg();
							qsubError.seekg (0, qsubError.end);
							bool empty = !qsubError.tellg(); // true if empty file
							qsubError.seekg (current, qsubError.beg); //restore stream position

							if (!empty) //checking if the file is not empty
							{
								LOG(ERROR) << "Error while invoking calculus. Error file " << qsubOutE << " has been created with content. Returning content of the file.";
								ErrorResponse::setFaultDetail("Error while invoking calculus. Token:");
								ErrorResponse::setFaultDetail(fileName);

								//std::ifstream qsublogOut(qsubOutO);

								while (std::getline(qsubError, line_from_file))
								{
									ErrorResponse::setFaultDetail(line_from_file);
								}
								stopChecking = true;

								qsublogOut.close();
							}
							/*else if (empty && calcFile && statFile)
							{
								stopChecking = true;
							}*/
							else
							{
								ErrorResponse::setFaultDetail("Got unknown error while trying to mpirun Algorithms");
							}
						}
						if (qsublogOut)
                        {
                            qsublogOut.close();
                            std::ifstream qsublogOut(qsubOutO, std::ifstream::ate | std::ifstream::binary);
                            int fSize = qsublogOut.tellg();
                            qsublogOut.close();

                            if (fSize > 84) //if size greater than 84 means that there is more thant two lines and mpirun produced error starting from the third line
                            {
                                LOG(ERROR) << "Error while performing calculus. Error file " << qsubOutO << " size indicates error. Returning content of the file.";
								ErrorResponse::setFaultDetail("Error while performing calculus. Token:");
                                ErrorResponse::setFaultDetail(fileName);

                                std::ifstream qsublogOut(qsubOutO);
								//first two lines are standard, thus skip it
								std::getline(qsublogOut, line_from_file);
								std::getline(qsublogOut, line_from_file);

								while (std::getline(qsublogOut, line_from_file))
								{
									ErrorResponse::setFaultDetail(line_from_file);
								}
                                stopChecking = true;
                            }
                        }
						if (calcFile && statFile)
                            stopChecking = true;

					calcFile.close();
					statFile.close();
					qsubError.close();
					qsublogOut.close();
					}
					while (!stopChecking);

                    //chek if error file has been created if so return its content otherwise return links to result files
                  /*  std::ifstream outFileE(qsubOutE);
                   // LOG(INFO) << "Cheking if " << jobID << " generated error file";
                    if (outFileE.good())
                    {
                        LOG(ERROR) << "Error file "<< qsubOutE << " generated, returning error to the client";
                        ErrorResponse::setFaultDetail("Error runnig algorithm");
                        while (std::getline(outFileE, line_from_file))
                        {
                            ErrorResponse::setFaultDetail(line_from_file);
                        }
                      // parse content of the file qsubOutE to error strem and return run error
                    }
                    outFileE.close();*/
                }
                else
                {
                     std::ifstream oFile(qsubOut);
                     LOG(ERROR) << "Error submitting job " << tmpBashFileUpper << " to qsub, returning error";
                     ErrorResponse::setFaultDetail("Error submiting job to qsub. Token:");
                     ErrorResponse::setFaultDetail(fileName);

                     while (std::getline(oFile, line_from_file))
                        {
                            ErrorResponse::setFaultDetail(line_from_file);
                        }
                    oFile.close();
                    //return error (content of the qsubOut)
                }
            }
            else
            {
                LOG(ERROR) << "Error getting qsub output file " << qsubOut << " , returning error to the client";
                ErrorResponse::setFaultDetail("qsub output file not found. Token:");
                ErrorResponse::setFaultDetail(fileName);
                //bad qsub output file
            }

			if (!ErrorResponse::isFaultFound())
			{
				LOG(INFO) << "Job SUCESS";
			}
			else
            {
                //copy input data and bash file to err directory for further inspection
                LOG(INFO) << "Copying bash and data files to err directory";
                LOG(INFO) << "Job FAILURE";
                struct stat st;
                std::string pathStr(ServiceSettings::workingDirMPI + "err");
                const char *path = pathStr.c_str();  //ServiceSettings::localDataFileSavePath.c_str() + "\err";
                stat(path, &st);
                bool isdir = S_ISDIR(st.st_mode);
                if (isdir)
                {
                    std::string sorce = bashFile;
                    std::string destination = pathStr + "/_bash_" + fileName + ".sh";
                    HelperMethods::copyFile(sorce.c_str(), destination.c_str());
                    sorce = cFile->getFilePath();
                    destination = pathStr + "/" + cFile->getFileName();
                    HelperMethods::copyFile(sorce.c_str(), destination.c_str());
                }//copy files
            }

            HelperMethods::deleteFile(cFile->getFilePath());
            HelperMethods::deleteFile(bashFile.c_str());
            HelperMethods::deleteFile(qsubOut.c_str());
            HelperMethods::deleteFile(qsubOutO.c_str());
            HelperMethods::deleteFile(qsubOutE.c_str());
            HelperMethods::deleteFile(bashFileUpper.c_str());

    }
	else if (ServiceSettings::runDestination == 2) //run on VU MIF cluster
	{
	 //creating bash script with the parameters
        callCalc = ServiceSettings::pathToMPIExecutable + " -wdir " + ServiceSettings::workingDirMPI + " " + ServiceSettings::pathToAlgorithmsExe + exeParams + " -i " +cFile->getFilePath() + " -o " + outFile->getFilePath() + " -s " + statFile->getFilePath();

		std::string fileName = HelperMethods::generateFileName("", false);
        std::string bashFile = ServiceSettings::workingDirMPI + "_bash_" + fileName + ".sh";
		std::string qsubOut = ServiceSettings::workingDirMPI + "_slurmSubmit_log_" + fileName + ".txt";
		std::string qsubOutO = ServiceSettings::workingDirMPI + "_slurmRunOut_log_" + fileName + ".txt";
        std::string qsubOutE = ServiceSettings::workingDirMPI + "_slurmRunOut_Err_" + fileName + ".txt";

		std::string bashFileUpper = ServiceSettings::workingDirMPI + "_bash_run_" + fileName + ".sh";

		std::ofstream file(bashFile.c_str());
        file << "#!/bin/bash"<<std::endl;
        file<<"#SBATCH -p ";

		//according to http://mif.vu.lt/cluster/
		if (maxCalculationTime <= 2*60*60) //less than 2 hours then put to short queue
			{
				file<<"short"<<std::endl;
				file<<"#SBATCH -C alpha" <<std::endl;
			}
		else if ((maxCalculationTime > 2*60*60) && (maxCalculationTime <= 48*60*60))//less than 48 hours then put to short queue
			{
				file<<"long"<<std::endl;
				file<<"#SBATCH -C alpha" <<std::endl;
			}
		else if ((maxCalculationTime > 48*60*60) && (maxCalculationTime <= 168*60*60))//less than 7 days then put to short queue
			{
				file<<"verylong"<<std::endl;
				file<<"#SBATCH -C gamma" <<std::endl;

			}
		else
			{
				file<<"extralong"<<std::endl;
				file<<"#SBATCH -C beta" <<std::endl;
			}

		file<<"#SBATCH -n "<<reqProcessors<<std::endl;
        file<<"#$ -o "<< qsubOutO << std::endl;
        file<<"#$ -e "<< qsubOutE << std::endl;
        file<<callCalc<<std::endl;
        file.close();

		std::ofstream fileUpper(bashFileUpper.c_str());
		if (reqProcessors > 1)
			fileUpper << "srun true"<<std::endl; //if ve need more than one CPU require them all to be accessible. Otherwise 1CPU becomes accessible minute after minute
        fileUpper <<"sbatch -D "<<ServiceSettings::workingDirMPI << " -o " << qsubOutO << " -e "<< qsubOutE << " " << bashFile  << " > " <<qsubOut << std::endl;
        fileUpper.close();


		char * tmpBashFileUpper = const_cast<char*>(bashFileUpper.c_str());
        std::string laik = "chmod 707 " + bashFileUpper;
        char * command =  const_cast<char*>(laik.c_str());
        system (command);

		char* argList[] =
       {
            tmpBashFileUpper,
            NULL /*       The argument list must end with a NULL.*/
        };

        /* Spawn a child process running the command. Ignore the
           returned child process ID.  */
        LOG(INFO) << "Submitting job to slurm" << tmpBashFileUpper;
        spawn (tmpBashFileUpper, argList);

		std::ifstream oFile(qsubOut);
        std::string line_from_file;

		system("sleep 2");
		if (oFile.good())
        {
			std::getline(oFile, line_from_file);
            oFile.close();

            std::size_t foundJob = line_from_file.find("Submitted batch job ");

			if (foundJob!=std::string::npos)
            {                  //find job id
                    std::size_t foundJobIDStart = line_from_file.find_first_of("0123456789");
                    std::size_t foundJobIDEnd  = line_from_file.find(" ", foundJobIDStart + 1);
                    std::string jobID = line_from_file.substr(foundJobIDStart, foundJobIDEnd - foundJobIDStart);
                    LOG(INFO) << "Job ID is " << jobID;

					//end of calculations indicates either created error file either created calculus result file
					//thus check for these files and return either error description of link to the result file

					bool stopChecking = false;
					std::string oF = outFile->getFilePath();
					std::string sF = statFile->getFilePath();

					do
					{
						system ("sleep 5");
						//checking if there are these files
						std::ifstream calcFile(oF.c_str());  //output from algorithms
						std::ifstream statFile(sF.c_str()); //statistics from the calculus
						std::ifstream slurmError(qsubOutE);

						if (slurmError)
						{
							std::streampos current = slurmError.tellg();
							slurmError.seekg (0, slurmError.end);
							bool empty = !slurmError.tellg(); // true if empty file
							slurmError.seekg (current, slurmError.beg); //restore stream position

							if (!empty) //checking if the file is not empty
							{
								LOG(ERROR) << "Error while invoking calculus. Error file " << qsubOutE << " has been created with content. Returning content of the file.";
								ErrorResponse::setFaultDetail("Error while invoking calculus. Token:");
								ErrorResponse::setFaultDetail(fileName);
								while (std::getline(slurmError, line_from_file))
								{
									ErrorResponse::setFaultDetail(line_from_file);
								}
								stopChecking = true;
							}
							else if (empty && calcFile && statFile)
							{
								stopChecking = true;
							}
							/*else
							{
								ErrorResponse::setFaultDetail("Got unknown error while trying to mpirun Algorithms");
							}*/
						}
					calcFile.close();
					statFile.close();
					slurmError.close();
					}
					while (!stopChecking);
			}
			else
			{
			 std::ifstream oFile(qsubOut);
             LOG(ERROR) << "Error submitting job " << tmpBashFileUpper << " to slurm, returning error";
             ErrorResponse::setFaultDetail("Error submitting job to slurm. Token:");
             ErrorResponse::setFaultDetail(fileName);
             while (std::getline(oFile, line_from_file))
               {
                     ErrorResponse::setFaultDetail(line_from_file);
               }
             oFile.close();
             //return error (content of the _slurmSubmit_log_.txt)
			}
		}
		else
		{
			ErrorResponse::setFaultDetail("Error submitting job. Cannot find JOBID. Token:");
			ErrorResponse::setFaultDetail(fileName);
		}
		if (!ErrorResponse::isFaultFound())
		{
			LOG(INFO) << "Job finished SUCESSFULY";
		}
		else
            {
                //copy input data and bash file to err directory for further inspection
                LOG(INFO) << "Copying bash and data files to err directory";
                LOG(INFO) << "Job FAILURE";
                struct stat st;
                std::string pathStr(ServiceSettings::workingDirMPI + "err");
                const char *path = pathStr.c_str();  //ServiceSettings::localDataFileSavePath.c_str() + "\err";
                stat(path, &st);
                bool isdir = S_ISDIR(st.st_mode);
                if (isdir)
                {
                    std::string sorce = bashFile;
                    std::string destination = pathStr + "/_bash_" + fileName + ".sh";
                    HelperMethods::copyFile(sorce.c_str(), destination.c_str());
                    sorce = cFile->getFilePath();
                    destination = pathStr + "/" + cFile->getFileName();
                    HelperMethods::copyFile(sorce.c_str(), destination.c_str());
                }//copy files
            }

        HelperMethods::deleteFile(cFile->getFilePath());
        HelperMethods::deleteFile(bashFile.c_str());
        HelperMethods::deleteFile(qsubOut.c_str());
        HelperMethods::deleteFile(qsubOutO.c_str());
        HelperMethods::deleteFile(qsubOutE.c_str());
        HelperMethods::deleteFile(bashFileUpper.c_str());
	}

}

std::string CallCalculus::getOutput(char *command)
{
    FILE *pipe = popen(command, "r");
    if (!pipe) return "ERR";
    char buffer[128];
    std::string result = "";
    //freopen("/home/damis/apache/htdocs/damis/data/test.err", "w", stdout);
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe)!= NULL)
            {
                result +=buffer;
                break; //found response thus job is still submitted
            }
    }
    pclose (pipe);
   // fclose(stdout);
    return result;
}

pid_t CallCalculus::spawn(char* program, char** arg_list)
{
    pid_t child_pid, endID;
    /* Duplicate this process. */
    child_pid = fork();

    int status;

    if (child_pid > 0)
        /* This is the parent process. */
    {
        do
        {
            //https://support.sas.com/documentation/onlinedoc/sasc/doc/lr2/waitpid.htm
            endID = waitpid(child_pid, &status, WNOHANG|WUNTRACED); /* wait for child to exit, and store child's exit status */
            if (endID == -1)
            {
                /* error calling waitpid       */
                LOG(ERROR) << "Error waiting for algorithm run process.";
                exit(EXIT_FAILURE);
            }
             else if (endID == child_pid)
            {
                /* child ended                 */
              /*  if (WIFEXITED(status))
                  LOG(INFO) <<"Child ended normally. \n";*/
                if (WIFSIGNALED(status))
                   // LOG(ERROR) << "Algorithm computation process ended abnormal because of an uncaught signal. \n";
                   fprintf (stderr, "Algorithm computation process ended abnormal because of an uncaught signal. \n");
                else if (WIFSTOPPED(status))
                   //LOG(ERROR) << "Algorithm computation process has stopped abnormal. \n";
                   fprintf (stderr, "Algorithm computation process has stopped abnormal. \n");

              //  exit(EXIT_SUCCESS);
            }
           // system("sleep 1");
        }
        while (endID == 0);

        return child_pid;
    }
    else
    {
        /* Now execute PROGRAM, searching for it in the path.  */
       // LOG(INFO) << "Calling qsub: " << program << arg_list;

        int execReturn = execvp (program,  arg_list);
        //std::cout << program;
        if(execReturn == -1)
        {

            //LOG(ERROR)<< "Failure! execve error code = %d. \n" << errno;
            fprintf (stderr, "Failure! execve error code = %d. \n", errno);
            std::string err = getErrMsg(errno);
            fprintf(stderr, err.c_str());
           //LOG(ERROR) <<  err;
           // std::cout << getErrMsg(errno) << std::endl;
        }
        /* The execvp  function returns only if an error occurs.  */
        fprintf(stderr, ". An error occurred in execvp.\n");
        abort();
    }
}

std::string CallCalculus::getErrMsg(int errnum)
{

    switch ( errnum )
    {

#ifdef EACCES
    case EACCES :
    {
        return "EACCES Permission denied";
    }
#endif

#ifdef EPERM
    case EPERM :
    {
        return "EPERM Not super-user";
    }
#endif

#ifdef E2BIG
    case E2BIG :
    {
        return "E2BIG Arg list too long";
    }
#endif

#ifdef ENOEXEC
    case ENOEXEC :
    {
        return "ENOEXEC Exec format error";
    }
#endif

#ifdef EFAULT
    case EFAULT :
    {
        return "EFAULT Bad address";
    }
#endif

#ifdef ENAMETOOLONG
    case ENAMETOOLONG :
    {
        return "ENAMETOOLONG path name is too long     ";
    }
#endif

#ifdef ENOENT
    case ENOENT :
    {
        return "ENOENT No such file or directory";
    }
#endif

#ifdef ENOMEM
    case ENOMEM :
    {
        return "ENOMEM Not enough core";
    }
#endif

#ifdef ENOTDIR
    case ENOTDIR :
    {
        return "ENOTDIR Not a directory";
    }
#endif

#ifdef ELOOP
    case ELOOP :
    {
        return "ELOOP Too many symbolic links";
    }
#endif

#ifdef ETXTBSY
    case ETXTBSY :
    {
        return "ETXTBSY Text file busy";
    }
#endif

#ifdef EIO
    case EIO :
    {
        return "EIO I/O error";
    }
#endif

#ifdef ENFILE
    case ENFILE :
    {
        return "ENFILE Too many open files in system";
    }
#endif

#ifdef EINVAL
    case EINVAL :
    {
        return "EINVAL Invalid argument";
    }
#endif

#ifdef EISDIR
    case EISDIR :
    {
        return "EISDIR Is a directory";
    }
#endif

#ifdef ELIBBAD
    case ELIBBAD :
    {
        return "ELIBBAD Accessing a corrupted shared lib";
    }
#endif

    default :
    {
        std::string errorMsg(strerror(errnum));
        if ( errnum ) return errorMsg;
    }
    }
}

///////////////////////////////////////////////////////////
//  CallCalculus.cpp
//  Implementation of the Class CallCalculus
//  Created on:      24-Vas-2014 16:29:02
//  Original author: Povilas
///////////////////////////////////////////////////////////

#include "CallCalculus.h"
#include "ServiceSettings.h"
#include "HelperMEthods.h"
#include "logging\easylogging++.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

CallCalculus::CallCalculus(int p, int maxCalcTime, InitDamisServiceFile* initFile):DamisService(initFile){

    this->reqProcessors = p;
    this->maxCalculationTime = maxCalcTime; // not used if qsub is not called
}


CallCalculus::~CallCalculus(){

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

    std::vector<std::string> dummy;
    dummy.reserve(0); //pass dummy vector to write function since no attributre after  are left
    // save data to temporary file
    writeDataToFile(cFile->getFilePath(), prepareDataSection(DamisService::getDataDoubleFormat(), dummy), prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(), dummy));

if (ServiceSettings::runDestination == 1) //run on MII cluster
{

    std::string callCalc = ServiceSettings::pathToMPIExecutable + " -n " + std::to_string(reqProcessors) + " -wdir " + ServiceSettings::workingDirMPI + " " + ServiceSettings::pathToAlgorithmsExe + exeParams + " -i " +cFile->getFilePath() + " " + " -o " + outFile->getFilePath() + " " + " -s " + statFile->getFilePath();

    //LOG(INFO) << "CALL: " << exeParams;
    //LOG(INFO)

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION  pi;
    std::vector<TCHAR> V( callCalc.length() + 1);
    for (int i=0;i< (int) callCalc.length();i++)
        V[i] = callCalc[i];

    CreateProcess(NULL, &V[0],0, 0, FALSE, 0, 0, 0, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}
//finally delete file that was used for calculus
HelperMethods::deleteFile(cFile->getFilePath());
//HelperMethods::deleteFile(statFile->getFilePath());

}

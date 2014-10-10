///////////////////////////////////////////////////////////
//  DimensionReduction.cpp
//  Implementation of the Class DimensionReduction
//  Created on:      05-Vas-2014 17:36:47
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "logging/easylogging++.h"
#include "DimensionReduction.h"
#include <math.h>


DimensionReduction::DimensionReduction(int p, int maxCalcTime, InitDamisService* initFile):CallCalculus(p, maxCalcTime, initFile)
{
    LOG (INFO) << "Dimension reduction has been called";
    this->exeParams = "";
}



DimensionReduction::~DimensionReduction(){

}

void DimensionReduction::runPCA(bool projType, double d)
{
    char bufferd[32];
    snprintf(bufferd, sizeof(bufferd), "%g", d);

    this->exeParams = " -al PCA -projType " + std::to_string(static_cast<long long>(projType)) + " -d " + bufferd;
    this->run();
}
void DimensionReduction::runDMA(int d, int maxIteration, double eps, double neighbour)
{
    char buffereps[32];
    snprintf(buffereps, sizeof(buffereps), "%g", eps);

    this->exeParams = " -al DMA -maxIter " + std::to_string(static_cast<long long>(maxIteration)) + " -d " + std::to_string(static_cast<long long>(d)) + " -eps " + buffereps + " -neighbour " + std::to_string(static_cast<long long>(int(ceil(neighbour / 100.0 * serveFile->getNumberOfObjects()))));
    this->run();
}
void DimensionReduction::runRELATIVEMDS(int d,int maxIteration,double eps, double noOfBaseVectors, int selStrategy)
{
    char buffereps[32];
    snprintf(buffereps, sizeof(buffereps), "%g", eps);

    this->exeParams = " -al RELATIVEMDS -maxIter " + std::to_string(static_cast<long long>(maxIteration)) + " -d " + std::to_string(static_cast<long long>(d)) + " -eps " + buffereps + " -noOfBaseVectors " + std::to_string(static_cast<long long>(int(ceil(noOfBaseVectors / 100.0 * serveFile->getNumberOfObjects())))) + " -selStrategy " + std::to_string(static_cast<long long>(selStrategy));
    this->run();
}
void DimensionReduction::runSMACOFMDS(int d, int maxIteration, double eps, bool zeidel)
{

    char buffereps[32];
    snprintf(buffereps, sizeof(buffereps), "%g", eps);

    this->exeParams = " -al SMACOFMDS -maxIter " + std::to_string(static_cast<long long>(maxIteration)) + " -d " + std::to_string(static_cast<long long>(d)) + " -eps " + buffereps + " -zeidel " + std::to_string(static_cast<long long>(zeidel));
    this->run();
}
void DimensionReduction::runSAMANN(int d, int maxIteration, double mTrain, int nNeurons, double eta)
{
    char buffereta[32];
    snprintf(buffereta, sizeof(buffereta), "%g", eta);

    this->exeParams = " -al SAMANN -maxIter " + std::to_string(static_cast<long long>(maxIteration)) + " -d " + std::to_string(static_cast<long long>(d)) + " -eta " + buffereta + " -mTrain " + std::to_string(static_cast<long long>(int(ceil(mTrain /100.0 * serveFile->getNumberOfObjects())))) + " -nNeurons " + std::to_string(static_cast<long long>(nNeurons));
    this->run();
}
void DimensionReduction::runSOMMDS(int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection)
{
    char buffereps[32];
    snprintf(buffereps, sizeof(buffereps), "%g", eps);

    this->exeParams = " -al SOMMDS -rows " + std::to_string(static_cast<long long>(rows)) + " -columns " + std::to_string(static_cast<long long>(columns)) + " -eHat " + std::to_string(static_cast<long long>(eHat)) + " -mdsIteration " + std::to_string(static_cast<long long>(mdsIteration)) + " -eps " + buffereps + " -mdsProjection " + std::to_string(static_cast<long long>(mdsProjection));
    this->run();
}

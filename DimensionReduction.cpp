///////////////////////////////////////////////////////////
//  DimensionReduction.cpp
//  Implementation of the Class DimensionReduction
//  Created on:      05-Vas-2014 17:36:47
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "logging\easylogging++.h"
#include "DimensionReduction.h"


DimensionReduction::DimensionReduction(int p, int maxCalcTime, InitDamisServiceFile* initFile):CallCalculus(p, maxCalcTime, initFile)
{
    LOG (INFO) << "Dimension reduction has been called";
    this->exeParams = "";
}



DimensionReduction::~DimensionReduction(){

}

void DimensionReduction::runPCA(bool projType, double d)
{
    this->exeParams = " -al PCA -projType " + std::to_string(projType) + " -d " + std::to_string(d);
    this->run();
}
void DimensionReduction::runDMA(int d, int maxIteration, double eps, double neighbour)
{
    this->exeParams = " -al DMA -maxIter " + std::to_string(maxIteration) + " -d " + std::to_string(d) + " -eps " + std::to_string(eps) + " -neighbour " + std::to_string(int(ceil(neighbour / 100.0 * this->noOfDataRows)));
    this->run();
}
void DimensionReduction::runRELATIVEMDS(int d,int maxIteration,double eps, double noOfBaseVectors, int selStrategy)
{
    this->exeParams = " -al RELATIVEMDS -maxIter " + std::to_string(maxIteration) + " -d " + std::to_string(d) + " -eps " + std::to_string(eps) + " -noOfBaseVectors " + std::to_string(int(ceil(noOfBaseVectors / 100.0 * this->noOfDataRows))) + " -selStrategy " + std::to_string(selStrategy);
    this->run();
}
void DimensionReduction::runSMACOFMDS(int d, int maxIteration, double eps, bool zeidel)
{
    this->exeParams = " -al SMACOFMDS -maxIter " + std::to_string(maxIteration) + " -d " + std::to_string(d) + " -eps " + std::to_string(eps) + " -zeidel " + std::to_string(zeidel);
    this->run();
}
void DimensionReduction::runSAMANN(int d, int maxIteration, double mTrain, int nNeurons, double eta)
{
    this->exeParams = " -al SAMANN -maxIter " + std::to_string(maxIteration) + " -d " + std::to_string(d) + " -eta " + std::to_string(eta) + " -mTrain " + std::to_string(int(ceil(mTrain /100.0 * this->noOfDataRows))) + " -nNeurons " + std::to_string(nNeurons);
    this->run();
}
void DimensionReduction::runSOMMDS(int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection)
{
    this->exeParams = " -al SOMMDS -rows " + std::to_string(rows) + " -columns " + std::to_string(columns) + " -eHat " + std::to_string(eHat) + " -mdsIteration " + std::to_string(mdsIteration) + " -eps " + std::to_string(eps) + " -mdsProjection " + std::to_string(mdsProjection);
    this->run();
}

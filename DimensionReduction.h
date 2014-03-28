///////////////////////////////////////////////////////////
//  DimensionReduction.h
//  Implementation of the Class DimensionReduction
//  Created on:      05-Vas-2014 17:36:47
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(DIMENSIONREDUCTION_H)
#define DIMENSIONREDUCTION_H

#include "CallCalculus.h"

/**
 * Class that implements constructing parameter set for dimmension reduction cluster algoritms
 */
class DimensionReduction : public CallCalculus
{

public:
	/**
	*Class constructor
	*/
	DimensionReduction(int p, int maxCalcTime, InitDamisServiceFile* initFile);
    /**
	*Class destructor
	*/
	virtual ~DimensionReduction();
    /**
	*Method runs PCA
	*/
	void runPCA(bool projType, double d);
    /**
	*Method runs SMACOFMDS
	*/
	void runSMACOFMDS(int d, int maxIteration, double eps, bool zeidel);
	/**
	*Method runs DMA
	*/
	void runDMA(int d, int maxIteration, double eps, double neighbour);
	/**
	*Method runs RELATIVEMDS
	*/
	void runRELATIVEMDS(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy);
	/**
	*Method runs SAMANN
	*/
	void runSAMANN(int d, int maxIteration, double mTrain, int nNeurons, double eta);
	/**
	*Method runs SOMMDS
	*/
	void runSOMMDS(int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection);
};
#endif // !defined(DIMENSIONREDUCTION_H)

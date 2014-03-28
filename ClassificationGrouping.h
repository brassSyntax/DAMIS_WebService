///////////////////////////////////////////////////////////
//  ClassificationGrouping.h
//  Implementation of the Class ClassificationGrouping
//  Created on:      05-Vas-2014 17:36:50
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(CLASSIFICATIONGROUPING_H)
#define CLASSIFICATIONGROUPING_H

#include "CallCalculus.h"

/**
 * Class that implements classification and grouping cluster algorithms
 */
class ClassificationGrouping : public CallCalculus
{

public:
    /**
	*Class constructor
	*/
	ClassificationGrouping(int p, int maxCalcTime, InitDamisServiceFile* initFile);
    /**
	*Class destructor
	*/
	virtual ~ClassificationGrouping();
    /**
	*Method runs SOM
	*/
	void runSOM(int rows, int columns, int eHat);
    /**
	*Method runs MLP
	*/
	void runMLP(int h1pNo, int h2pNo, int h3pNo, double dL, double dT, double dV, int maxIteration);
    /**
	*Method runs C45
	*/
	void runC45(double q, double dL, double dT);
    /**
	*Method runs KMEANS
	*/
	void runKMEANS(int maxIteration, int kMax);
};
#endif // !defined(CLASSIFICATIONGROUPING_H)

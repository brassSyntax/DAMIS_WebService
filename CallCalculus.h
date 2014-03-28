///////////////////////////////////////////////////////////
//  CallCalculus.h
//  Implementation of the Class CallCalculus
//  Created on:      24-Vas-2014 16:29:02
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(CALLCALCULUS_H)
#define CALLCALCULUS_H

#include "DamisService.h"

/**
 * Abstract class that implements algorithm run on a cluster
 */
class CallCalculus : public DamisService
{

public:
    /**
	*Class constructor
	*/
	CallCalculus(int p, int maxCalcTime, InitDamisServiceFile* initFile);
    /**
	*Class destructor
	*/
	virtual ~CallCalculus();

protected:
    /**
	* virtual method that calls cluster algorithm
	*/
	virtual void run();
	/**
	* Holds passed parameter string to bash script (string will be splitted as argc and argv structure at the service algorithm side)
 	*/
	std::string exeParams;
private:
    /**
	* Holds required processor count (will be passed to qsub)
	*/
    int reqProcessors;
    /**
	* Holds required calculation time (will be passed to qsub for job sequencing)
	*/
    int maxCalculationTime;
};

#endif // !defined(CALLCALCULUS_H)

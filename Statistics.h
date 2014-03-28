///////////////////////////////////////////////////////////
//  Statistics.h
//  Implementation of the Class Statistics
//  Created on:      05-Vas-2014 17:36:46
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(STATISTICS_H)
#define STATISTICS_H

#include "DamisService.h"

/**
 * Class that implements statistical calculations
 */
class Statistics : public DamisService
{

public:
	Statistics(InitDamisServiceFile*);
	void statPrimitives();


	virtual ~Statistics();
};
#endif // !defined(STATISTICS_H)

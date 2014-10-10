///////////////////////////////////////////////////////////
//  Statistics.h
//  Implementation of the Class Statistics
//  Created on:      05-Vas-2014 17:36:46
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(STATISTICS_H)
#define STATISTICS_H

#include "ServeRequest.h"

/** \brief Class that implements statistical calculations
 *
 */
class Statistics : public ServeRequest
{

public:
    /** \brief Constructor
     *
     * \param InitDamisService* pointer to the InitDamisService object
     *
     */
    Statistics(InitDamisService*);

    /** \brief Calulates statistics of the data section
     *
     * \return void
     *
     */
	void statPrimitives();
    /** \brief Destructor
     */
	virtual ~Statistics();
};
#endif //!defined(STATISTICS_H)

///////////////////////////////////////////////////////////
//  CallCalculus.h
//  Implementation of the Class CallCalculus
//  Created on:      24-Vas-2014 16:29:02
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(CALLCALCULUS_H)
#define CALLCALCULUS_H

#include "ServeRequest.h"


 /** \brief Class implements call of the algorithms on the cluster
  *
  */

class CallCalculus : public ServeRequest
{

public:
    /** \brief Class constructor.
     *
     * \param int p number of processors.
     * \param int maxCalcTime predictive calculation time in seconds.
     * \param InitDamisService* initFile pointer to the InitDamisService object.
     *
     */
	CallCalculus(int p, int maxCalcTime, InitDamisService* initFile);
    /** \brief Class destructor
	*
	*/
	virtual ~CallCalculus();

protected:
    /** \brief Virtual method that calls run algorithm on the cluster
     *
     * \return void
     */
	virtual void run();

	std::string exeParams; /**<  Holds passed parameter string to bash script (string will be splitted as argc and argv structure at the service algorithm side) */
private:

    int reqProcessors; /**< Holds required processor count (will be passed to qsub or slurm) */

    int maxCalculationTime; /**< Holds required calculation time (will be passed to qsub or slurm for job sequencing) */
    /** \brief Function forks the process ant starts new process indicated by keyword program and predefined arqlList
     *
     * \param char* program array of chars of the program name
     * \param char** arg_list aray of crhar array that describes the arguments
     * \return pid_t child process ID
     *
     */
    pid_t spawn(char* program, char** arg_list);
    /** \brief Function handles and retruns child process execution errors
     *
     * \param int errnum
     * \return std::string
     *
     */

    std::string getErrMsg(int errnum);
    /** \brief Function gets messages from the pipe
     *
     * \param  char* command command to run
     * \return std::string response from the pipe
     *
     */
    std::string getOutput(char* command);

};

#endif //!defined(CALLCALCULUS_H)

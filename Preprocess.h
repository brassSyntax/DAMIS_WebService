///////////////////////////////////////////////////////////
//  Preprocess.h
//  Implementation of the Class Preprocess
//  Created on:      05-Vas-2014 17:36:45
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(PREPROCESS_H)
#define PREPROCESS_H

#include "DamisService.h"
#include <string>

/**
 * Implements all components described in Preprocessing gropup
 */
class Preprocess : public DamisService
{

public:
	Preprocess(InitDamisServiceFile*);

    /**
     *Normalize data either by mean and std or by mapping to interval
     */
	void normData(bool, double, double);
    /**
     *Function that performs data transpose
     */
	void transposeData();
    /**
     *Function that performs data cleaning
     */
	void cleanData();
	 /**
     *Function that performs data cleaning
     */
	void splitData(bool, double, double);
    /**
     *Function that performs data filtering according to sigma rule
     */
	void filterData(bool, double, int);
	 /**
     *Variable that holds alternative output path ir needed (by default NULL)
     */
    DamisFile *altOutFile = NULL;

	virtual ~Preprocess();

private:
   /* std::string httpPath;
    std::string localPath;*/
    /**
     *Variable that holds object classes to be written in output file
     */
    std::vector<std::string> writeClass;
    /*std::vector<std::vector<double>> writeData;
    std::vector<double> tmpDataVector;
    int noOfDataRows;
    int noOfDataAttr;*/

};
#endif // !defined(PREPROCESS_H)

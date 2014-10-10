/* soapDAMISService.h
   Generated by gSOAP 2.8.16 from damis.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapDAMISService_H
#define soapDAMISService_H
#include "soapH.h"
class SOAP_CMAC DAMISService
{ public:
	struct soap *soap;
	bool own;
	/// Constructor
	DAMISService();
	/// Constructor to use/share an engine state
	DAMISService(struct soap*);
	/// Constructor with engine input+output mode control
	DAMISService(soap_mode iomode);
	/// Constructor with engine input and output mode control
	DAMISService(soap_mode imode, soap_mode omode);
	/// Destructor, also frees all deserialized data
	virtual ~DAMISService();
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to defaults
	virtual	void reset();
	/// Initializer used by constructor
	virtual	void DAMISService_init(soap_mode imode, soap_mode omode);
	/// Create a copy
	virtual	DAMISService *copy() SOAP_PURE_VIRTUAL;
	/// Close connection (normally automatic)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Return sender-related fault to sender
	virtual	int soap_senderfault(const char *string, const char *detailXML);
	/// Return sender-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Return receiver-related fault to sender
	virtual	int soap_receiverfault(const char *string, const char *detailXML);
	/// Return receiver-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Run simple single-thread iterative service on port until a connection error occurs (returns error code or SOAP_OK), use this->bind_flag = SO_REUSEADDR to rebind for a rerun
	virtual	int run(int port);
	/// Bind service to port (returns master socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET bind(const char *host, int port, int backlog);
	/// Accept next request (returns socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET accept();
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
	/// Then accept SSL handshake, when SSL is used
	virtual	int ssl_accept();
#endif
	/// Serve this request (returns error code or SOAP_OK)
	virtual	int serve();
	/// Used by serve() to dispatch a request (returns error code or SOAP_OK)
	virtual	int dispatch();

	///
	/// Service operations (you should define these):
	/// Note: compile with -DWITH_PURE_VIRTUAL for pure virtual methods
	///

	/// Web service operation 'PCA' (returns error code or SOAP_OK)
	virtual	int PCA(std::string X, bool projType, double d, int maxCalcTime, struct Damis__PCAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SMACOFMDS' (returns error code or SOAP_OK)
	virtual	int SMACOFMDS(std::string X, int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime, struct Damis__SMACOFMDSResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'DMA' (returns error code or SOAP_OK)
	virtual	int DMA(std::string X, int d, int maxIteration, double eps, double neighbour, int maxCalcTime, struct Damis__DMAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'RELMDS' (returns error code or SOAP_OK)
	virtual	int RELMDS(std::string X, int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime, struct Damis__RELMDSResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SAMANN' (returns error code or SOAP_OK)
	virtual	int SAMANN(std::string X, int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime, struct Damis__SAMANNResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SOM' (returns error code or SOAP_OK)
	virtual	int SOM(std::string X, int rows, int columns, int eHat, int p, int maxCalcTime, struct Damis__SOMResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SOMMDS' (returns error code or SOAP_OK)
	virtual	int SOMMDS(std::string X, int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection, int maxCalcTime, struct Damis__SOMMDSResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'MLP' (returns error code or SOAP_OK)
	virtual	int MLP(std::string X, int h1pNo, int h2pNo, int h3pNo, double dL, double dT, double dV, int maxIteration, int p, int maxCalcTime, struct Damis__MLPResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'C45' (returns error code or SOAP_OK)
	virtual	int C45(std::string X, double q, double dL, double dT, int maxCalcTime, struct Damis__C45Response &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'KMEANS' (returns error code or SOAP_OK)
	virtual	int KMEANS(std::string X, int kMax, int maxIteration, int maxCalcTime, struct Damis__KMEANSResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'STATPRIMITIVES' (returns error code or SOAP_OK)
	virtual	int STATPRIMITIVES(std::string X, int maxCalcTime, struct Damis__STATPRIMITIVESResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'CLEANDATA' (returns error code or SOAP_OK)
	virtual	int CLEANDATA(std::string X, int maxCalcTime, struct Damis__CLEANDATAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'FILTERDATA' (returns error code or SOAP_OK)
	virtual	int FILTERDATA(std::string X, bool retFilteredData, double zValue, int attrIndex, int maxCalcTime, struct Damis__FILTERDATAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SPLITDATA' (returns error code or SOAP_OK)
	virtual	int SPLITDATA(std::string X, bool reshufleObjects, double firstSubsetPerc, double secondSubsetPerc, int maxCalcTime, struct Damis__SPLITDATAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'TRANSPOSEDATA' (returns error code or SOAP_OK)
	virtual	int TRANSPOSEDATA(std::string X, int maxCalcTime, struct Damis__TRANSPOSEDATAResponse &_param_1) SOAP_PURE_VIRTUAL;

	/// Web service operation 'NORMDATA' (returns error code or SOAP_OK)
	virtual	int NORMDATA(std::string X, bool normMeanStd, double a, double b, int maxCalcTime, struct Damis__NORMDATAResponse &_param_1) SOAP_PURE_VIRTUAL;
};
#endif

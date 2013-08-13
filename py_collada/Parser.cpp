#include <boost/python.hpp>
#include <boost/foreach.hpp>
#include <iostream>

#include "Parser.hpp"
#include "PyReceiver.hpp"

using std::cout;
using namespace boost::python;

namespace PyCollada {

string Parser::file = "";
recv_map Parser::receivers = recv_map();

BOOST_PYTHON_MODULE(Receivers) 
{
	// TODO why expose it as class? kinda unecessary...
	// it could also be just functions, coulnd't it?
	class_<PyReceiver>("PyReceiver")
		.def("getFile",      &PyReceiver::getFile)
		.def("getReceivers", &PyReceiver::getReceivers)
		.def("setVertices",  &PyReceiver::setVertices)
		.def("setNormals",   &PyReceiver::setNormals)
		.def("setTexels",    &PyReceiver::setTexels)
		.def("setTexture",   &PyReceiver::setTexture)
		.def("setTransformationMatrix", &PyReceiver::setTransformationMatrix);
}

void Parser::parse(string file, flat_map<string, Receiver*> recvs)
{

	Parser::file = file;
	Parser::receivers = recvs;

	Py_Initialize();

	try {
		initReceivers();

		FILE *file = fopen("import.py", "r");

		PyRun_SimpleFile(file, "import.py");

		fclose(file);
	} catch (error_already_set) {
		PyErr_Print();
	}

	Py_Finalize();
}


}

#ifndef PYRECEIVER_HPP

#define PYRECEIVER_HPP

#include <string>
#include <boost/python.hpp>

#include "Parser.hpp"

using std::string;

using namespace boost::python;

namespace PyCollada {

class PyReceiver {
	public:
		string objectName;

		string getFile();

		boost::python::list getReceivers();

		void setVertices(
			string name,
			boost::python::list data,
			int span
		);

		void setNormals( 
			string name,
			boost::python::list normals,
			int span
		);

		void setTexels(
				string name,
				boost::python::list texels,
				int span
		);

		void setTexture(
				string name,
				string path
		);

		void setColor(
				string name,
				string colorName,
				boost::python::list color
		);

		void setTransformationMatrix(
			string name,
			boost::python::list matrix
		);

	private:
		float* convertListToFloat(boost::python::list,
				unsigned int *size);
};




}

#endif /* end of include guard: PYRECEIVER_HPP */

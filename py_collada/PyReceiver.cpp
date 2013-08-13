#include "PyReceiver.hpp"

#include <iostream>
#include <exception>
#include <boost/foreach.hpp>

using namespace std;

namespace PyCollada {

	string PyReceiver::getFile() {
		return Parser::file;
	}

	/**
	 * Set Normals
	 *
	 * Turn normals into a float array
	 * The size has to be divided by the span because
	 * the receiver expects the amount of data chunks
	 */
	void PyReceiver::setNormals(
			string name,
			boost::python::list normals,
			int span
			) {
		unsigned int size;
		float *data = convertListToFloat(normals, &size);

		Parser::receivers[name]->setNormals(data, size / span, span);

	}
	/**
	 * Set vertices
	 *
	 * Turn vertices into a float array
	 * The size has to be divided by the span because
	 * the receiver expects the amount of data chunks
	 */
	void PyReceiver::setVertices(
			string name,
			boost::python::list vertices,
			int span
	) {
		unsigned int size;
		float * data = convertListToFloat(vertices, &size);

		Parser::receivers[name]->setVertices(data, size / span, span);
	}

	void PyReceiver::setTexels(
			string name,
			boost::python::list texels,
			int span
	) {
		unsigned int size;
		float *data = convertListToFloat(texels, &size);

		Parser::receivers[name]->setTexels(data, size / span, span);
	}

	void PyReceiver::setTexture(
			string name,
			string path
	) {
		Parser::receivers[name]->setTexture(path);
	}

	/**
	 * Convert a python list into a float array
	 */
	float* PyReceiver::convertListToFloat(boost::python::list data, 
			unsigned int *size) {
		*size = boost::python::len(data);
		GLfloat *array = new GLfloat[*size];

		for (unsigned int i = 0; i < *size; i++) {
			array[i] = boost::python::extract<GLfloat>(data[i]);
		}

		return array;
	}

	/**
	 * Set transformation matrix
	 *
	 * Turn the transformation matrix into a float array
	 */
	void PyReceiver::setTransformationMatrix(
			string name,
			boost::python::list matrix
			) {

		unsigned int size = boost::python::len(matrix);

		if (size != 16)  {
            throw new std::exception();
		}

		GLfloat *data = new GLfloat[size];
		
		for (unsigned int i = 0; i < size; i++) {
			data[i] = boost::python::extract<GLfloat>(matrix[i]);
		}

		Parser::receivers[name]->setTransformationMatrix(
			data
		);
	}

	/**
	 * Return the receiver names
	 */
	boost::python::list PyReceiver::getReceivers() {
		boost::python::list recvlist;

		BOOST_FOREACH(recv_map::value_type &recv, Parser::receivers)  {
			recvlist.append<string>(recv.first);
		}

		return recvlist;
	}


}

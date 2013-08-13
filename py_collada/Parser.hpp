#ifndef PYCOLLADA_PARSER_HPP

#define PYCOLLADA_PARSER_HPP


#include <string>
#include <boost/container/flat_map.hpp>
#include <boost/python.hpp>

#include "Receiver.hpp"

using std::string;

using namespace boost::container;

namespace PyCollada {

typedef flat_map<string, Receiver *> recv_map;

class Parser {

	public:
		static void parse(string file, recv_map receivers);

		static string file;
		static recv_map receivers;

	private:

};

}

#endif /* end of include guard: PYCOLLADA_PARSER_HPP */

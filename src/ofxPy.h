#pragma once

#include "ofMain.h"

#include <pybind11/pybind11.h>

#define OFXPY_BEGIN_NAMESPACE namespace ofx { namespace Py {
#define OFXPY_END_NAMESPACE } }

OFXPY_BEGIN_NAMESPACE

namespace py = pybind11;

using namespace pybind11;

class Context
{
public:
	
	Context();
	virtual ~Context();
	
	bool setup();
	void dispose();
	
	//
	
	py::object eval(const std::string& code);
	
	template <typename Ret>
	Ret eval(const std::string& code);
	
	void exec(const std::string& code);
	void run(const std::string& path);
	
	bool import(py::module m);
	
	void appendPath(const std::string& path);
	
protected:
	
	bool inited;
	
	py::module py_main;
	py::dict py_global;
};

//

template <typename T, typename ContainerType, typename KeyType>
inline T get(ContainerType a, KeyType key)
{
	return ((py::object)a[key]).cast<T>();
}

//

template <typename Ret>
inline Ret Context::eval(const std::string& code)
{
	try {
		return eval(code).cast<Ret>();
	} catch(...) {
		throw;
	}
}

OFXPY_END_NAMESPACE

namespace ofxPy = ofx::Py;
#include "ofxPy.h"

OFXPY_BEGIN_NAMESPACE

static int _num_contexts = 0;

//

Context::Context()
	: inited(false)
{}

Context::~Context()
{
	dispose();
}

bool Context::setup()
{
	if (inited)
		return false;
	
	if (_num_contexts == 0)
		Py_Initialize();
	
	py_main = py::object(PyImport_AddModule("__main__"), false);
	py_global = py::object(PyModule_GetDict(py_main.ptr()), true);
	
	inited = true;
	
	_num_contexts++;
	
	return true;
}

void Context::dispose()
{
	if (!inited)
		return;
	
	_num_contexts--;
	
	if (_num_contexts == 0)
		Py_Finalize();
	
	inited = false;
}

//

py::object Context::eval(const std::string& code)
{
	py::object o(PyRun_String(code.c_str(),
							  Py_eval_input,
							  py_global.ptr(),
							  py_global.ptr()),
				 false);
	
	if (PyErr_Occurred() != NULL)
	{
		PyErr_Print();
		PyErr_Clear();
		
		return py::object(Py_None, true);
	}
	
	return o;
}

template <>
std::string Context::eval<std::string>(const std::string& code) {
	try {
		return (const char*)eval(code).str();
	} catch(...) {
		throw;
	}
}

void Context::exec(const std::string& code)
{
	PyRun_SimpleString(code.c_str());
}

void Context::run(const std::string& path)
{
	std::ifstream ifs(path.c_str());
	if (ifs.fail())
		return py::object();
	
	std::string code((std::istreambuf_iterator<char>(ifs)),
					 std::istreambuf_iterator<char>());
	
	exec(code);
}

bool Context::import(py::module m)
{
	if (!m)
		return false;
	
	string name = PyModule_GetName(m.ptr());
	if (!name.empty())
	{
		py_global[name.c_str()] = m;
	}
	
	return true;
}

void Context::appendPath(const std::string& path)
{
	const std::string p = "'" + ofToDataPath(path, true) + "'";
	
	const string& s = ""
	"import sys\n"
	"if not " + p + " in sys.path:\n"
	"	sys.path.append(" + p + ")\n"
	;
	
	exec(s);
}

OFXPY_END_NAMESPACE

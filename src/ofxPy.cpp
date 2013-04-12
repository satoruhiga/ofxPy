#include "ofxPy.h"

#include "python-external-runtime.h"
#include "ofxpy_swigutils.h"

#include "Poco/Base64Decoder.h"

#include "ofxpy_lib_base64.h"

extern "C" void of_openlibs(void);

using namespace ofxPy;

static Object py_main, py_global;

void ofxPy::setup(const string& module_path)
{
	if (Py_IsInitialized()) return;
	
	setenv("PYTHONPATH", ofToDataPath(module_path).c_str(), 0);
	
	Py_Initialize();
	
	py_main = PyImport_AddModule("__main__");
    py_global = PyModule_GetDict(py_main);
	
	of_openlibs();

	stringstream ss;
	ss << of_py_base64;
	Poco::Base64Decoder b64in(ss);
	
	ofBuffer lib;
	b64in >> lib;
	
	PyRun_SimpleString("__file__ = 'openFrameworks'");
	PyRun_SimpleString(lib.getText().c_str());
}

Object& ofxPy::global()
{
	return py_global;
}

Object ofxPy::eval(const string &code)
{
	PyObject *ret = PyRun_String(code.c_str(), Py_single_input, py_global, py_global);

	if(PyErr_Occurred() != NULL){
		ofLogError("ofxPy") << "===";
		PyErr_Print();
		PyErr_Clear();
	}

	if (ret)
	{
		Object obj(ret);
		Py_DECREF(ret);
		return obj;
	}
	else
	{
		Py_RETURN_NONE;
	}
}

void ofxPy::run(const string& path)
{
	ofBuffer buffer = ofBufferFromFile(path);
	PyRun_SimpleString(buffer.getText().c_str());
}

#pragma mark - Object

Object::Object() { obj = Py_None; Py_INCREF(obj); }
Object::Object(PyObject *obj) : obj(obj) { Py_INCREF(obj); }
Object::Object(const Object &copy) : obj(NULL) { *this = copy; }
Object::~Object() { Py_DECREF(obj); obj = NULL; }

Object::Object(int v) : obj(PyInt_FromLong(v)) {}
Object::Object(float v) : obj(PyFloat_FromDouble(v)) {}
Object::Object(bool v) : obj(PyBool_FromLong(v)) {}
Object::Object(const string& v) : obj(SWIG_From_std_string(v)) {}

Object& Object::operator=(const Object &copy)
{
	if (obj) Py_DECREF(obj);
	obj = copy.obj;
	Py_INCREF(obj);
}

bool Object::isNone() const
{
	return obj == Py_None;
}

double Object::asNumber() const
{
	double ret = 0;
	if (SWIG_AsVal_double(obj, &ret) == SWIG_OK)
		return ret;
	return 0;
}

bool Object::asBool() const
{
	bool ret = 0;
	if (SWIG_AsVal_bool(obj, &ret) == SWIG_OK)
		return ret;
	return 0;
}

string Object::asString() const
{
	if (PyString_Check(obj))
	{
		return PyString_AsString(obj);
	}
	else
	{
		PyObject* repr = PyObject_Repr(obj);
		string ret = PyString_AsString(repr);
		Py_DECREF(repr);
		return ret;
	}
}

size_t Object::size() const
{
	return PyObject_Size(obj);
}

Object Object::get(const string& key)
{
	PyObject *obj0 = PyString_FromString(key.c_str());
	PyObject *obj1 = PyObject_GetItem(obj, obj0);
	Py_DECREF(obj0);
	
	if (obj1)
	{
		Object ret = obj1;
		Py_DECREF(obj1);
		return ret;
	}
	
	return Object();
}

Object Object::get(size_t index)
{
	PyObject *obj0 = PyInt_FromSize_t(index);
	PyObject *obj1 = PyObject_GetItem(obj, obj0);
	Py_DECREF(obj0);
	
	if (obj1)
	{
		Object ret = obj1;
		Py_DECREF(obj1);
		return ret;
	}
	
	return Object();
}

const Object& Object::set(const string& key, const Object &v)
{
	PyObject *obj0 = PyString_FromString(key.c_str());
	PyObject_SetItem(obj, obj0, v);
	Py_DECREF(obj0);
	return v;
}

const Object& Object::set(size_t index, const Object &v)
{
	PyObject *obj0 = PyInt_FromSize_t(index);
	PyObject_SetItem(obj, obj0, v);
	Py_DECREF(obj0);
	return v;
}

Object Object::call()
{
	PyObject *ret = PyObject_CallObject(obj, NULL);
	
	if(PyErr_Occurred() != NULL){
		ofLogError("ofxPy") << "===";
		PyErr_Print();
		PyErr_Clear();
	}
	
	if (ret)
	{
		Object obj(ret);
		Py_DECREF(ret);
		return obj;
	}
	else
	{
		Py_RETURN_NONE;
	}
}

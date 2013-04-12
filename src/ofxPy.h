#pragma once

#include "ofMain.h"

#include <Python.h>

namespace ofxPy {
	class Object;
	
	void setup(const string& module_path = "");
	
	Object& global();
	
	Object eval(const string &code);
	void run(const string& path);
}

class ofxPy::Object
{
public:
	
	Object();
	Object(PyObject *obj);
	Object(const Object &copy);
	virtual ~Object();
	
	Object(int v);
	Object(float v);
	Object(bool v);
	Object(const string& v);
	
	Object& operator=(const Object &copy);
	
	bool isNone() const;
	
	double asNumber() const;
	
	inline float asFloat() const { return asNumber(); }
	inline int asInt() const { return asNumber(); }
	
	bool asBool() const;
	string asString() const;
	
	size_t size() const;
	Object get(const string& key);
	Object get(size_t index);
	
	const Object& set(const string& key, const Object &v);
	const Object& set(size_t index, const Object &v);
	
	Object call();
	
	inline operator PyObject* () { return obj; }
	inline operator PyObject* const () const { return obj; }

	friend ostream& operator<<(ostream &os, const ofxPy::Object &obj)
	{
		os << obj.asString();
		return os;
	}
	
protected:
	
	PyObject *obj;
};



#pragma once

SWIGINTERN int
SWIG_AsVal_double (PyObject *obj, double *val)
{
	int res = SWIG_TypeError;
	if (PyFloat_Check(obj)) {
		if (val) *val = PyFloat_AsDouble(obj);
		return SWIG_OK;
	} else if (PyInt_Check(obj)) {
		if (val) *val = PyInt_AsLong(obj);
		return SWIG_OK;
	} else if (PyLong_Check(obj)) {
		double v = PyLong_AsDouble(obj);
		if (!PyErr_Occurred()) {
			if (val) *val = v;
			return SWIG_OK;
		} else {
			PyErr_Clear();
		}
	}
#ifdef SWIG_PYTHON_CAST_MODE
	{
		int dispatch = 0;
		double d = PyFloat_AsDouble(obj);
		if (!PyErr_Occurred()) {
			if (val) *val = d;
			return SWIG_AddCast(SWIG_OK);
		} else {
			PyErr_Clear();
		}
		if (!dispatch) {
			long v = PyLong_AsLong(obj);
			if (!PyErr_Occurred()) {
				if (val) *val = v;
				return SWIG_AddCast(SWIG_AddCast(SWIG_OK));
			} else {
				PyErr_Clear();
			}
		}
	}
#endif
	return res;
}

#include <float.h>
#include <math.h>

SWIGINTERNINLINE int
SWIG_CanCastAsInteger(double *d, double min, double max) {
	double x = *d;
	if ((min <= x && x <= max)) {
		double fx = floor(x);
		double cx = ceil(x);
		double rd =  ((x - fx) < 0.5) ? fx : cx; /* simple rint */
		if ((errno == EDOM) || (errno == ERANGE)) {
			errno = 0;
		} else {
			double summ, reps, diff;
			if (rd < x) {
				diff = x - rd;
			} else if (rd > x) {
				diff = rd - x;
			} else {
				return 1;
			}
			summ = rd + x;
			reps = diff/summ;
			if (reps < 8*DBL_EPSILON) {
				*d = rd;
				return 1;
			}
		}
	}
	return 0;
}


SWIGINTERN int
SWIG_AsVal_unsigned_SS_long (PyObject *obj, unsigned long *val)
{
#if PY_VERSION_HEX < 0x03000000
	if (PyInt_Check(obj)) {
		long v = PyInt_AsLong(obj);
		if (v >= 0) {
			if (val) *val = v;
			return SWIG_OK;
		} else {
			return SWIG_OverflowError;
		}
	} else
#endif
		if (PyLong_Check(obj)) {
			unsigned long v = PyLong_AsUnsignedLong(obj);
			if (!PyErr_Occurred()) {
				if (val) *val = v;
				return SWIG_OK;
			} else {
				PyErr_Clear();
			}
		}
#ifdef SWIG_PYTHON_CAST_MODE
	{
		int dispatch = 0;
		unsigned long v = PyLong_AsUnsignedLong(obj);
		if (!PyErr_Occurred()) {
			if (val) *val = v;
			return SWIG_AddCast(SWIG_OK);
		} else {
			PyErr_Clear();
		}
		if (!dispatch) {
			double d;
			int res = SWIG_AddCast(SWIG_AsVal_double (obj,&d));
			if (SWIG_IsOK(res) && SWIG_CanCastAsInteger(&d, 0, ULONG_MAX)) {
				if (val) *val = (unsigned long)(d);
				return res;
			}
		}
	}
#endif
	return SWIG_TypeError;
}


SWIGINTERNINLINE int
SWIG_AsVal_size_t (PyObject * obj, size_t *val)
{
	unsigned long v;
	int res = SWIG_AsVal_unsigned_SS_long (obj, val ? &v : 0);
	if (SWIG_IsOK(res) && val) *val = static_cast< size_t >(v);
	return res;
}


#define SWIG_From_long   PyLong_FromLong


SWIGINTERNINLINE PyObject*
SWIG_From_bool  (bool value)
{
	return PyBool_FromLong(value ? 1 : 0);
}


SWIGINTERN int
SWIG_AsVal_long (PyObject *obj, long* val)
{
	if (PyInt_Check(obj)) {
		if (val) *val = PyInt_AsLong(obj);
		return SWIG_OK;
	} else if (PyLong_Check(obj)) {
		long v = PyLong_AsLong(obj);
		if (!PyErr_Occurred()) {
			if (val) *val = v;
			return SWIG_OK;
		} else {
			PyErr_Clear();
		}
	}
#ifdef SWIG_PYTHON_CAST_MODE
	{
		int dispatch = 0;
		long v = PyInt_AsLong(obj);
		if (!PyErr_Occurred()) {
			if (val) *val = v;
			return SWIG_AddCast(SWIG_OK);
		} else {
			PyErr_Clear();
		}
		if (!dispatch) {
			double d;
			int res = SWIG_AddCast(SWIG_AsVal_double (obj,&d));
			if (SWIG_IsOK(res) && SWIG_CanCastAsInteger(&d, LONG_MIN, LONG_MAX)) {
				if (val) *val = (long)(d);
				return res;
			}
		}
	}
#endif
	return SWIG_TypeError;
}

SWIGINTERN int
SWIG_AsVal_bool (PyObject *obj, bool *val)
{
	int r = PyObject_IsTrue(obj);
	if (r == -1)
		return SWIG_ERROR;
	if (val) *val = r ? true : false;
	return SWIG_OK;
}

SWIGINTERN swig_type_info*
SWIG_pchar_descriptor(void)
{
	static int init = 0;
	static swig_type_info* info = 0;
	if (!init) {
		info = SWIG_TypeQuery("_p_char");
		init = 1;
	}
	return info;
}

SWIGINTERN int
SWIG_AsCharPtrAndSize(PyObject *obj, char** cptr, size_t* psize, int *alloc)
{
#if PY_VERSION_HEX>=0x03000000
	if (PyUnicode_Check(obj))
#else
		if (PyString_Check(obj))
#endif
		{
			char *cstr; Py_ssize_t len;
#if PY_VERSION_HEX>=0x03000000
			if (!alloc && cptr) {
				/* We can't allow converting without allocation, since the internal
				 representation of string in Python 3 is UCS-2/UCS-4 but we require
				 a UTF-8 representation.
				 TODO(bhy) More detailed explanation */
				return SWIG_RuntimeError;
			}
			obj = PyUnicode_AsUTF8String(obj);
			PyBytes_AsStringAndSize(obj, &cstr, &len);
			if(alloc) *alloc = SWIG_NEWOBJ;
#else
			PyString_AsStringAndSize(obj, &cstr, &len);
#endif
			if (cptr) {
				if (alloc) {
					/*
					 In python the user should not be able to modify the inner
					 string representation. To warranty that, if you define
					 SWIG_PYTHON_SAFE_CSTRINGS, a new/copy of the python string
					 buffer is always returned.
					 
					 The default behavior is just to return the pointer value,
					 so, be careful.
					 */
#if defined(SWIG_PYTHON_SAFE_CSTRINGS)
					if (*alloc != SWIG_OLDOBJ)
#else
						if (*alloc == SWIG_NEWOBJ)
#endif
						{
							*cptr = reinterpret_cast< char* >(memcpy((new char[len + 1]), cstr, sizeof(char)*(len + 1)));
							*alloc = SWIG_NEWOBJ;
						}
						else {
							*cptr = cstr;
							*alloc = SWIG_OLDOBJ;
						}
				} else {
#if PY_VERSION_HEX>=0x03000000
					assert(0); /* Should never reach here in Python 3 */
#endif
					*cptr = SWIG_Python_str_AsChar(obj);
				}
			}
			if (psize) *psize = len + 1;
#if PY_VERSION_HEX>=0x03000000
			Py_XDECREF(obj);
#endif
			return SWIG_OK;
		} else {
			swig_type_info* pchar_descriptor = SWIG_pchar_descriptor();
			if (pchar_descriptor) {
				void* vptr = 0;
				if (SWIG_ConvertPtr(obj, &vptr, pchar_descriptor, 0) == SWIG_OK) {
					if (cptr) *cptr = (char *) vptr;
					if (psize) *psize = vptr ? (strlen((char *)vptr) + 1) : 0;
					if (alloc) *alloc = SWIG_OLDOBJ;
					return SWIG_OK;
				}
			}
		}
	return SWIG_TypeError;
}


SWIGINTERN int
SWIG_AsPtr_std_string (PyObject * obj, std::string **val)
{
	char* buf = 0 ; size_t size = 0; int alloc = SWIG_OLDOBJ;
	if (SWIG_IsOK((SWIG_AsCharPtrAndSize(obj, &buf, &size, &alloc)))) {
		if (buf) {
			if (val) *val = new std::string(buf, size - 1);
			if (alloc == SWIG_NEWOBJ) delete[] buf;
			return SWIG_NEWOBJ;
		} else {
			if (val) *val = 0;
			return SWIG_OLDOBJ;
		}
	} else {
		static int init = 0;
		static swig_type_info* descriptor = 0;
		if (!init) {
			descriptor = SWIG_TypeQuery("std::string" " *");
			init = 1;
		}
		if (descriptor) {
			std::string *vptr;
			int res = SWIG_ConvertPtr(obj, (void**)&vptr, descriptor, 0);
			if (SWIG_IsOK(res) && val) *val = vptr;
			return res;
		}
	}
	return SWIG_ERROR;
}


SWIGINTERNINLINE PyObject *
SWIG_FromCharPtrAndSize(const char* carray, size_t size)
{
	if (carray) {
		if (size > INT_MAX) {
			swig_type_info* pchar_descriptor = SWIG_pchar_descriptor();
			return pchar_descriptor ?
			SWIG_InternalNewPointerObj(const_cast< char * >(carray), pchar_descriptor, 0) : SWIG_Py_Void();
		} else {
#if PY_VERSION_HEX >= 0x03000000
			return PyUnicode_FromStringAndSize(carray, static_cast< int >(size));
#else
			return PyString_FromStringAndSize(carray, static_cast< int >(size));
#endif
		}
	} else {
		return SWIG_Py_Void();
	}
}


SWIGINTERNINLINE PyObject *
SWIG_From_std_string  (const std::string& s)
{
	return SWIG_FromCharPtrAndSize(s.data(), s.size());
}
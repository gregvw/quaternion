#ifndef QUATERNION_OBJECT_HPP
#define QUATERNION_OBJECT_HPP

#include <Python.h>
#include "structmember.h"

#ifdef __cplusplus 
extern "C" {
#endif

#ifndef Py_LIMITED_API

typedef struct {
  double w,x,y,z;
} Py_quaternion;

PyAPI_FUNC(Py_quaternion) q_sum( Py_quaternion a, Py_quaternion b);
PyAPI_FUNC(Py_quaternion) q_diff(Py_quaternion a, Py_quaternion b);
PyAPI_FUNC(Py_quaternion) q_prod(Py_quaternion a, Py_quaternion b);
PyAPI_FUNC(Py_quaternion) q_quot(Py_quaternion a, Py_quaternion b);
PyAPI_FUNC(Py_quaternion) q_neg( Py_quaternion a);
PyAPI_FUNC(Py_quaternion) q_conj(Py_quaternion a);
PyAPI_FUNC(Py_quaternion) q_inv( Py_quaternion a);
PyAPI_FUNC(Py_quaternion) q_log( Py_quaternion q);
PyAPI_FUNC(double) q_abs(Py_quaternion a);

typedef struct {
  PyObject_HEAD
  Py_quaternion qval;
} PyQuaternionObject;

PyAPI_DATA(PyTypeObject) PyQuaternion_Type;

#define PyQuaternion_Check(op) PyObject_TypeCheck(op, &PyQuaternion_Type)
#define PyQuaternion_CheckExact(op) (Py_TYPE(op) == &PyQuaternion_Type)

PyAPI_FUNC(PyObject *) PyQuaternion_FromCQuaternion(Py_quaternion);

#endif // Py_LIMITED_API


#ifdef __cplusplus
} // extern "C"
#endif



#endif // QUATERNION_OBJECT_HPP


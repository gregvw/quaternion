#include "quaternionobject.hpp"
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

#define  C_TEXT(text) ((char*)std::string(text).c_str())

/* Elementary operations on quaternions */

// Addition
Py_quaternion
q_sum(Py_quaternion a, Py_quaternion b) {
  Py_quaternion r;
  r.w = a.w+b.w;
  r.x = a.x+b.x;
  r.y = a.y+b.y;
  r.z = a.z+b.z;
  return r;
}

// Subtraction
Py_quaternion
q_diff(Py_quaternion a, Py_quaternion b) {
  Py_quaternion r;
  r.w = a.w-b.w;
  r.x = a.x-b.x;
  r.y = a.y-b.y;
  r.z = a.z-b.z;
  return r;
}

// Negation
Py_quaternion
q_neg(Py_quaternion a) {
  Py_quaternion r;
  r.w = -a.w;
  r.x = -a.x;
  r.y = -a.y;
  r.z = -a.z;
  return r;
}


// Multiplication
Py_quaternion
q_prod(Py_quaternion a, Py_quaternion b) {
  Py_quaternion r;
  r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
  r.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
  r.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
  r.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
  return r;
}

// Absolute value squared
double 
q_abs2(Py_quaternion a) {
  return a.w*a.w + a.x*a.x + a.y*a.y + a.z*a.z;
}

// Absolute value
double
q_abs(Py_quaternion a) {
  return std::sqrt(q_abs2(a));
}

// Conjugation
Py_quaternion
q_conj(Py_quaternion a) {
  Py_quaternion r;
  r.w = a.w;
  r.x = -a.x;
  r.y = -a.y;
  r.z = -a.z;
  return r;
}

// Inversion 
Py_quaternion
q_inv(Py_quaternion a) {
  double d = q_abs2(a);
  Py_quaternion r = q_conj(a);
  r.w /= d;
  r.x /= d;
  r.y /= d;
  r.z /= d;
  return r;
}

// Division 
Py_quaternion
q_quot(Py_quaternion a, Py_quaternion b) {
  Py_quaternion bi = q_inv(b);
  Py_quaternion r = q_prod(a,bi);
  return r;
}

// Exponential
Py_quaternion
q_exp(Py_quaternion q) {
  double a = q.w;
  double expa = std::exp(a);
  double nv = std::sqrt((q.x)*(q.x)+(q.y)*(q.y)+(q.z)*(q.z));
  double c = std::cos(nv)*expa;
  double s = std::sin(nv)*expa/nv;
  Py_quaternion r;
  r.w = c;
  r.x = s*q.x;
  r.y = s*q.y;
  r.z = s*q.z;
  return r;
}

// Logarithm
Py_quaternion
q_log(Py_quaternion q) {
  double a    = q.w;
  double nv2  = q.x*q.x + q.y*q.y + q.z*q.z;
  double nq2  = nv2+a*a;
  double nv   = std::sqrt(nv2);
  double nq   = std::sqrt(nq2);
  double ac   = std::acos(a/nq)/nv;
  Py_quaternion r;
  r.w = std::log(nq);
  r.x = q.x*ac;
  r.y = q.y*ac;
  r.z = q.z*ac;
  return r;
}


template<class Scalar>
Py_quaternion 
q_pow(Py_quaternion q, Scalar p) {
  double a     = q.w;
  double nv2   = q.x*q.x + q.y*q.y + q.z*q.z;
  double nq2   = nv2+a*a;
  double nv    = std::sqrt(nv2);
  double nq    = std::sqrt(nq2);
  double nqp   = std::pow(nq,p);
  double theta = std::acos(q.w/nq);
  double c = std::cos(p*theta)*nqp;
  double s = std::sin(p*theta)*nqp/nv;
  Py_quaternion r;
  r.w = c; 
  r.x = s*q.x;
  r.y = s*q.y;
  r.z = s*q.z;
  return r;
}


// Handles printing of negative components
void q_string(Py_quaternion v, char buf[]) {
  char sx='-',sy='-',sz='-';
  double w = v.w;
  double x = std::abs(v.x);
  double y = std::abs(v.y);
  double z = std::abs(v.z);
  if(v.x>=0) sx = '+';
  if(v.y>=0) sy = '+';
  if(v.z>=0) sz = '+';

  sprintf(buf,"%f%c%fi%c%fj%c%fk",w,sx,x,sy,y,sz,z);

}

#define TO_QUATERNION(obj,q) { \
  if  (PyQuaternion_Check(obj)) \
    q = ((PyQuaternionObject *)(obj))->qval; \
  else if (to_quaternion(&(obj),&(q)) < 0) \
    return (obj); \
}

static int
to_quaternion(PyObject **pobj, Py_quaternion *pq) {
  PyObject *obj = *pobj;
  pq->w = 0.0; pq->x = 0.0; pq->y = 0.0; pq->z = 0.0;
#if PY_MAJOR_VERSION >= 3
  if( PyLong_Check(obj) ) {
    pq->w = PyLong_AsDouble(obj);
    return 0;
  } 
#else
  if( PyInt_Check(obj) ) {
    pq->w = static_cast<double>(PyInt_AsLong(obj)); 
    return 0;
  }
#endif
  if( PyFloat_Check(obj) ) {
    pq->w = PyFloat_AsDouble(obj);
    return 0;
  } 
  Py_INCREF(Py_NotImplemented);
  *pobj = Py_NotImplemented;
  return -1;
}

PyObject* 
PyQuaternion_FromCQuaternion(Py_quaternion qval) {
 PyQuaternionObject *op;

  op = (PyQuaternionObject *)PyObject_MALLOC(sizeof(PyQuaternionObject));
  if( op == NULL ) 
    return PyErr_NoMemory();
  (void)PyObject_INIT(op, &PyQuaternion_Type);
  op->qval = qval;
  return (PyObject *)op;
}


static PyObject* 
quaternion_add(PyObject *v, PyObject* w) {
  Py_quaternion result;
  Py_quaternion a, b;
  TO_QUATERNION(v, a);
  TO_QUATERNION(w, b);
  PyFPE_START_PROTECT("quaternion_add", return 0)
  result = q_sum(a,b);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_sub(PyObject *v, PyObject* w) {
  Py_quaternion result;
  Py_quaternion a,b;
  TO_QUATERNION(v, a);
  TO_QUATERNION(w, b);
  PyFPE_START_PROTECT("quaternion_sub", return 0)
  result = q_diff(a,b);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_mul(PyObject *v, PyObject* w) {
  Py_quaternion result;
  Py_quaternion a,b;
  TO_QUATERNION(v, a);
  TO_QUATERNION(w, b);
  PyFPE_START_PROTECT("quaternion_mul", return 0)
  result = q_prod(a,b);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_div(PyObject *v, PyObject* w) {
  Py_quaternion result;
  Py_quaternion a,b;
  TO_QUATERNION(v, a);
  TO_QUATERNION(w, b);
  PyFPE_START_PROTECT("quaternion_div", return 0)
  result = q_quot(a,b);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_neg(PyQuaternionObject *a) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_neg", return 0)
  result = q_neg(a->qval);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_abs(PyQuaternionObject *a) {
  double result;
  PyFPE_START_PROTECT("quaternion_abs", return 0)
  result = q_abs(a->qval);
  PyFPE_END_PROTECT(result);
  return PyFloat_FromDouble(result);
}

static PyObject* 
quaternion_pos(PyQuaternionObject *a) {
  return PyQuaternion_FromCQuaternion(a->qval);
}

static PyObject* 
quaternion_inv(PyQuaternionObject *a) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_inv", return 0)
  result = q_inv(a->qval);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_conj(PyQuaternionObject *a) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_conj", return 0)
  result = q_conj(a->qval);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_exp(PyQuaternionObject *a) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_exp", return 0)
  result = q_exp(a->qval);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject* 
quaternion_log(PyQuaternionObject *a) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_log", return 0)
  result = q_log(a->qval);
  PyFPE_END_PROTECT(result);
  return PyQuaternion_FromCQuaternion(result);
}

static PyObject*
quaternion_pow(PyQuaternionObject *q, PyObject *p) {
  Py_quaternion result;
  PyFPE_START_PROTECT("quaternion_pow", return 0);
  if (PyLong_Check(p)) {
    result = q_pow(q->qval,PyLong_AsLong(p));
  }
  else if(PyFloat_Check(p)) {
    result = q_pow(q->qval,PyFloat_AsDouble(p));
  }
  return PyQuaternion_FromCQuaternion(result);
}

static void
quaternion_dealloc(PyObject* op) {
  op->ob_type->tp_free(op);
}

static PyObject*
quaternion_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyQuaternionObject *self;
  
  self = (PyQuaternionObject*)type->tp_alloc(type,0);
  if(self != NULL) {
    self->qval.w = 0.0;
    self->qval.x = 0.0;
    self->qval.y = 0.0;
    self->qval.z = 0.0;
  }
  
  return (PyObject*)self;
}

static int
quaternion_init(PyQuaternionObject *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {C_TEXT("w"),
                           C_TEXT("x"),
                           C_TEXT("y"),
                           C_TEXT("z"),
                           NULL};
  
  if(!PyArg_ParseTupleAndKeywords(args,kwds,"|dddd",kwlist,
     &self->qval.w,&self->qval.x,&self->qval.y,&self->qval.z)) 
    return -1;

  return 0;
}

static int
quaternion_print(PyQuaternionObject *v, FILE *file, int flags) {
  char buf[100];
  q_string(v->qval,buf);
  fprintf(file,buf);
  return 0;
}

static PyObject*
quaternion_str(PyQuaternionObject *v) {
  char buf[100];
  q_string(v->qval,buf);
  return PyUnicode_FromString(buf);
}

static PyMethodDef quaternion_methods[] = {
 {"conj",(PyCFunction)quaternion_conj, METH_VARARGS, "Return conjugate quaternion"},
 {"exp", (PyCFunction)quaternion_exp,  METH_VARARGS, "Exponential of quaternion"},
 {"log", (PyCFunction)quaternion_log,  METH_VARARGS, "Logarithm of quaternion"},
 {"pow", (PyCFunction)quaternion_pow,  METH_VARARGS, "Power of quaternion"},
 {NULL,NULL}
};

static PyMemberDef quaternion_members[] = {
  {C_TEXT("w"),T_DOUBLE,offsetof(PyQuaternionObject,qval.w),0,C_TEXT("quaternion w")},
  {C_TEXT("x"),T_DOUBLE,offsetof(PyQuaternionObject,qval.x),0,C_TEXT("quaternion x")},
  {C_TEXT("y"),T_DOUBLE,offsetof(PyQuaternionObject,qval.y),0,C_TEXT("quaternion y")},
  {C_TEXT("z"),T_DOUBLE,offsetof(PyQuaternionObject,qval.z),0,C_TEXT("quaternion z")},
  {NULL}
};


static PyNumberMethods quaternion_as_number = {
  (binaryfunc)quaternion_add,          /* nb_add                  */
  (binaryfunc)quaternion_sub,          /* nb_subtract             */
  (binaryfunc)quaternion_mul,          /* nb_multiply             */
#if PY_MAJOR_VERSION < 3       
  (binaryfunc)quaternion_div,          /* nb_divide               */
#endif  
  0,	                               /* nb_remainder            */
  0,	                               /* nb_divmod               */
  0,                                   /* nb_power                */
  (unaryfunc)quaternion_neg,	       /* nb_negative             */
  (unaryfunc)quaternion_pos,	       /* nb_positive             */
  (unaryfunc)quaternion_abs,	       /* nb_absolute             */
#if PY_MAJOR_VERSION >= 3
  0,                                   /* nb_bool                 */
#else 
  0,                                   /* nb_nonzero              */
#endif //  PY_MAJOR_VERSION >= 3
  (unaryfunc)quaternion_inv,	       /* nb_invert               */
  0,                                   /* nb_lshift               */
  0,                                   /* nb_rshift               */
  0,                                   /* nb_and                  */
  0,     	                       /* nb_xor                  */
  0,	                               /* nb_or                   */
#if PY_MAJOR_VERSION < 3
  0,                                   /* nb_coerce               */
#endif // PY_MAJOR_VERSION < 3
  0,                                   /* nb_int                  */
#if PY_MAJOR_VERSION >= 3 
  0,                                   /* nb_reserved             */
#else
  0,                                   /* nb_long                 */
#endif // PY_MAJOR_VERSION > 3
  0,                                   /* nb_float                */
#if PY_MAJOR_VERSION < 3
  0,                                   /* nb_oct                  */
  0,                                   /* nb_hex                  */
#endif // PY_MAJOR_VERSION < 3

#if PY_MAJOR_VERSION >= 2
  0,			               /* nb_inplace_add          */
  0,			               /* nb_inplace_subtract     */
  0,			               /* nb_inplace_multiply     */
  0, 			               /* nb_inplace_remainder    */
  0,			               /* nb_inplace_power        */
  0,			               /* nb_inplace_lshift       */
  0,			               /* nb_inplace_rshift       */
  0,			               /* nb_inplace_and          */
  0,			               /* nb_inplace_xor          */
  0,			               /* nb_inplace_or           */
#if PY_MINOR_VERSION >= 2 || PY_MAJOR_VERSION >= 3  // Python 2.2 and up
  0,                                   /* nb_floor_divide         */
  (binaryfunc)quaternion_div,          /* nb_true_divide          */
  0,		   	               /* nb_inplace_floor_divide */
  0,			               /* nb_inplace_true_divide  */
#if PY_MINOR_BERSION >=5 || PY_MAJOR_VERSION >= 3 // Python 2.5 and up
  0,                                   /* nb_index                */
#endif // PY_MINOR_BERSION >=5  || PY_MAJOR_VERSION >= 3
#endif // PY_MINOR_VERSION >= 2 || PY_MAJOR_VERSION >= 3
#endif // PY_MAJOR_VERSION >= 2 

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 5
  0,                                   /* nb_matrix_multiply      */
  0                                    /* nb_inplace_matrix_multiply */
#endif // PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 5
};

PyTypeObject PyQuaternion_Type = {
#if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(&PyType_Type,0)
#else
  PyObject_HEAD_INIT(NULL)
  0,                                   /* ob_size           */
#endif
  "quaternion.Quaternion",             /* tp_name           */
  sizeof(PyQuaternionObject),          /* tp_basicsize      */
  0,                                   /* tp_itemsize       */
  (destructor)quaternion_dealloc,      /* tp_dealloc        */
  (printfunc)quaternion_print,         /* tp_print          */
  0,                                   /* tp_getattr        */
  0,                                   /* tp_setattr        */
  0,                                   /* tp_reserved       */
  (reprfunc)quaternion_str,            /* tp_repr           */
  &quaternion_as_number,               /* tp_as_number      */
  0,                                   /* tp_as_sequence    */
  0,                                   /* tp_as_mapping     */
  0,                                   /* tp_hash           */
  0,                                   /* tp_call           */
  (reprfunc)quaternion_str,            /* tp_str            */
  0,                                   /* tp_getattro       */
  0,                                   /* tp_setattro       */
  0,                                   /* tp_as_buffer      */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags      */  
  "Quaternion objects",                /* tp_doc            */
  0,		                       /* tp_traverse       */     
  0,		                       /* tp_clear          */     
  0,		                       /* tp_richcompare    */     
  0,		                       /* tp_weaklistoffset */     
  0,		                       /* tp_iter           */    
  0,		                       /* tp_iternext       */     
  quaternion_methods,                  /* tp_methods        */     
  quaternion_members,                  /* tp_members        */     
  0,                                   /* tp_getset         */     
  0,                                   /* tp_base           */     
  0,                                   /* tp_dict           */     
  0,                                   /* tp_descr_get      */     
  0,                                   /* tp_descr_set      */     
  0,                                   /* tp_dictoffset     */     
  (initproc)quaternion_init,           /* tp_init           */     
  0,                                   /* tp_alloc          */     
  quaternion_new,                      /* tp_new            */ 
  PyObject_Del,                        /* tp_free           */
}; 

#if PY_MAJOR_VERSION >= 3
static PyModuleDef quaternion_module = {
  PyModuleDef_HEAD_INIT,
  "quaternion",
  "Implements a quaternion type",
  -1,
  NULL, NULL, NULL, NULL, NULL
#else
static PyMethodDef qmethods[] = {
  {NULL}
#endif
};


#if PY_MAJOR_VERSION < 3
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
#endif 


PyMODINIT_FUNC 
#if PY_MAJOR_VERSION >= 3
PyInit_quaternion(void)
#else
initquaternion(void)
#endif
{
  PyObject *m;
  if(PyType_Ready(&PyQuaternion_Type) < 0) 
#if PY_MAJOR_VERSION >= 3 
   return NULL;
#else
   return;
#endif

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&quaternion_module);
  if( m == NULL )
    return NULL;
#else
  m = Py_InitModule3("quaternion", qmethods, "Quaternion type");
  if( m == NULL ) 
    return;
#endif

  Py_INCREF(&PyQuaternion_Type);
  PyModule_AddObject(m,"Quaternion",(PyObject *)&PyQuaternion_Type);
#if PY_MAJOR_VERSION >= 3
  return m;
#endif
}

                                      





#include "Python.h"

#include "bf2c/c_emitter.h"
#include "bf2c/parser.h"

// NOLINTBEGIN

typedef struct py_program_t
{
    PyObject_HEAD;
    program_t program;
} py_program_t;

static void Program_dealloc(py_program_t* self)
{
    bf2c_program_destroy(&self->program);
    PyObject_Free(self);
}

static PyTypeObject ProgramType = {
    // Avoid reformatting the initializer due to MACRO
    // clang-format off
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "py_bfc2.Program",
    .tp_basicsize = sizeof(py_program_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = "bf2c's internal representation of the program",
    .tp_dealloc   = (destructor) Program_dealloc,
    // clang-format on
};

static py_program_t* Program_new(program_t program)
{
    py_program_t* self = (py_program_t*) PyObject_New(py_program_t, &ProgramType);
    if (!self)
    {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for Program object.");
        return NULL;
    }
    self->program = program;
    return self;
}

static PyObject* parse_file(PyObject* self, PyObject* args)
{
    const char* file_path;
    if (!PyArg_ParseTuple(args, "s", &file_path))
    {
        return NULL;
    }
    // TODO: handle "no such file" error
    return (PyObject*) Program_new(bf2c_parse_file_by_name(file_path));
}

static PyObject* parse_text(PyObject* self, PyObject* args)
{
    const char* text;
    if (!PyArg_ParseTuple(args, "s", &text))
    {
        return NULL;
    }
    return (PyObject*) Program_new(bf2c_parse_text(text));
}

static PyObject* print_code(PyObject* self, PyObject* args)
{
    PyObject* program_obj;
    if (!PyArg_ParseTuple(args, "O!", &ProgramType, &program_obj))
    {
        return NULL;
    }
    py_program_t* program = (py_program_t*) program_obj;
    bf2c_emit_c_to_file(stdout, &program->program);
    Py_RETURN_NONE;
}

static PyObject* emit_to_file(PyObject* self, PyObject* args)
{
    char const* file_path;
    PyObject* program_obj;
    if (!PyArg_ParseTuple(args, "O!s", &ProgramType, &program_obj, &file_path))
    {
        return NULL;
    }
    py_program_t* program = (py_program_t*) program_obj;
    bf2c_emit_c_to_filename(file_path, &program->program);
    Py_RETURN_NONE;
}

static PyMethodDef bf2cMethods[] = {
    {.ml_name  = "parse_file",
     .ml_meth  = parse_file,
     .ml_flags = METH_VARARGS,
     .ml_doc   = "Parse a Brainfuck file and return an internal representation."},
    {.ml_name  = "parse_text",
     .ml_meth  = parse_text,
     .ml_flags = METH_VARARGS,
     .ml_doc   = "Parse Brainfuck code and return an internal representation."},
    {.ml_name  = "print_code",
     .ml_meth  = print_code,
     .ml_flags = METH_VARARGS,
     .ml_doc   = "Print the C code from an internal representation."},
    {.ml_name  = "emit_to_file",
     .ml_meth  = emit_to_file,
     .ml_flags = METH_VARARGS,
     .ml_doc   = "Write the C code from an internal representation to a given file."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef bf2cModule = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "py_bfc2",
    .m_doc     = NULL, // module documentation, may be NULL
    .m_size    = -1,   // -1 if the module keeps state in global variables.
    .m_methods = bf2cMethods,
};

PyMODINIT_FUNC PyInit_py_bf2c(void)
{
    // Check whether ProgramType is ready, but do not add it to the module.
    // It is intended as an opaque handle.
    if (PyType_Ready(&ProgramType) < 0)
    {
        return NULL;
    }
    return PyModule_Create(&bf2cModule);
}

// NOLINTEND

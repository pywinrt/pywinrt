
static PyObject* _enter_@(%* self) noexcept
{
    Py_INCREF(self);
    return reinterpret_cast<PyObject*>(self);
}

using System.CodeDom.Compiler;

static class IterWriterExtensions
{
    public static void WriteIterGenericInterfaceImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("PyObject* dunder_iter() noexcept override");
        w.WriteBlock(() => w.WriteDunderIterBody(type));

        if (type.IsPyIterator)
        {
            w.WriteLine("PyObject* dunder_iternext() noexcept override");
            w.WriteBlock(() => w.WriteDunderIterNextBody(type));
        }
    }

    public static void WriteDunderIterBody(this IndentedTextWriter w, ProjectedType type)
    {
        if (type.IsPyMapping)
        {
            w.WriteMapDunderIterBody(type);
        }
        else if (type.IsPyIterator)
        {
            var self = type.IsGeneric ? "this" : "self";
            w.WriteLine($"return reinterpret_cast<PyObject*>({self});");
        }
        else if (type.IsPyIterable)
        {
            w.WriteTryCatch(() =>
            {
                var method = type.Methods.Single(m => m.Name == "First");
                var self = type.GetMethodInvokeContext(method);

                w.WriteLine("return py::convert([&]()");
                w.WriteBlock(
                    () =>
                    {
                        w.WriteLine("auto _gil = py::release_gil();");
                        w.WriteLine($"return {self}First();");
                    },
                    "());"
                );
            });
        }
        else
        {
            throw new NotImplementedException();
        }
    }

    public static void WriteDunderIterNextBody(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteTryCatch(() =>
        {
            var method = type.Methods.Single(m => m.Name == "MoveNext");
            var self = type.GetMethodInvokeContext(method);

            w.WriteLine($"if ({self}HasCurrent())");
            w.WriteBlock(() =>
            {
                w.WriteLine("return py::convert([&]()");
                w.WriteBlock(
                    () =>
                    {
                        w.WriteLine("auto _gil = py::release_gil();");
                        w.WriteLine($"auto cur = {self}Current();");
                        w.WriteLine($"{self}MoveNext();");
                        w.WriteLine("return cur;");
                    },
                    "());"
                );
            });
            w.WriteLine("else");
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
        });
    }
}

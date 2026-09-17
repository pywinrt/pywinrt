using System.CodeDom.Compiler;
using Mono.Cecil;

static class StructWriterExtensions
{
    public static void WritePythonStructTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var metaclass = "";

        if (type.PyRequiresMetaclass)
        {
            w.WriteLine("@typing.final");
            w.WriteLine($"class {type.Name}_Static(type):");
            w.Indent++;

            var pass = true;

            if (type.Type.IsCustomNumeric)
            {
                w.WriteNumberFactoryFunctionPyTyping(type, ref pass);
                w.WriteNumberCommonValuesPyTyping(type, ref pass);
            }

            if (pass)
            {
                w.WriteLine("pass");
            }

            w.Indent--;
            w.WriteBlankLine();

            metaclass = $"(metaclass={type.Name}_Static)";
        }

        w.WriteLine("@typing.final");
        w.WriteLine($"class {type.Name}{metaclass}:");
        w.Indent++;

        foreach (var field in type.Type.Fields)
        {
            w.WriteLine("@_property");
            w.WriteLine(
                $"def {field.Name.ToPythonIdentifier()}(self) -> {field.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(field.FieldType), packageMap)}: ..."
            );
        }

        w.WriteLine(
            $"def __new__(cls, {string.Join(", ", type.Type.Fields.Select(f => $"{f.Name.ToPythonIdentifier()}: {f.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(f.FieldType), packageMap)} = {f.FieldType.GetDefaultPyValue(ns, packageMap)}"))}) -> {type.Name}: ..."
        );

        w.WriteLine($"def __replace__(self, /, **changes: typing.Any) -> {type.Name}: ...");

        if (type.Type.IsCustomNumeric)
        {
            if (type.Name != "Plane")
            {
                w.WriteNumberSlotMethodsPyTyping(type);
            }

            w.WriteNumberMethodPyTyping(type);
        }

        if (type.Type.Fields.Count > 1)
        {
            w.WriteLine(
                $"def unpack(self) -> {type.Type.ToPyTupleTyping(ns, packageMap, isUnpack: true)}: ..."
            );
        }

        w.Indent--;
        w.WriteBlankLine();
    }
}

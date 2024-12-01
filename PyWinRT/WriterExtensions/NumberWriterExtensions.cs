using System.CodeDom.Compiler;

static class NumberWriterExtensions
{
    private record ParamInfo(string Name, string PyType)
    {
        public string CppWinrtType => cppWinrtTypeFromPyType[PyType];
    };

    private record FactoryInfo(
        string PyName,
        string CppWinrtName,
        IReadOnlyList<ParamInfo> Parameters,
        bool NoMinGW = false
    );

    private record MethodInfo(
        string Name,
        string ReturnPyType,
        IReadOnlyList<ParamInfo> Parameters,
        bool NoMinGW = false
    )
    {
        public string ReturnCppWinrtType => cppWinrtTypeFromPyType[ReturnPyType];
    }

    private static readonly IReadOnlyDictionary<string, string> cppWinrtTypeFromPyType =
        new Dictionary<string, string>
        {
            { "float", "float" },
            { "Vector2", "winrt::Windows::Foundation::Numerics::float2" },
            { "Vector3", "winrt::Windows::Foundation::Numerics::float3" },
            { "Vector4", "winrt::Windows::Foundation::Numerics::float4" },
            { "Matrix3x2", "winrt::Windows::Foundation::Numerics::float3x2" },
            { "Matrix4x4", "winrt::Windows::Foundation::Numerics::float4x4" },
            { "Plane", "winrt::Windows::Foundation::Numerics::plane" },
            { "Quaternion", "winrt::Windows::Foundation::Numerics::quaternion" }
        };

    private static readonly IReadOnlyDictionary<string, IReadOnlyCollection<string>> commonValues =
        new Dictionary<string, IReadOnlyCollection<string>>
        {
            {
                "Vector2",
                new List<string> { "zero", "one", "unit_x", "unit_y" }
            },
            {
                "Vector3",
                new List<string> { "zero", "one", "unit_x", "unit_y", "unit_z" }
            },
            {
                "Vector4",
                new List<string> { "zero", "one", "unit_x", "unit_y", "unit_z", "unit_w" }
            },
            {
                "Matrix3x2",
                new List<string> { "identity" }
            },
            {
                "Matrix4x4",
                new List<string> { "identity" }
            },
            {
                "Quaternion",
                new List<string> { "identity" }
            }
        };

    private static readonly IReadOnlyDictionary<
        string,
        IReadOnlyCollection<FactoryInfo>
    > factoryFunctions = new Dictionary<string, IReadOnlyCollection<FactoryInfo>>
    {
        {
            "Matrix3x2",
            new List<FactoryInfo>
            {
                new(
                    "make_translation",
                    "make_float3x2_translation",
                    [new ParamInfo("position", "Vector2")],
                    NoMinGW: true
                ),
                new(
                    "make_translation",
                    "make_float3x2_translation",
                    [new ParamInfo("x_position", "float"), new ParamInfo("y_position", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_scale",
                    "make_float3x2_scale",
                    [new ParamInfo("x_scale", "float"), new ParamInfo("y_scale", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_scale",
                    "make_float3x2_scale",
                    [
                        new ParamInfo("x_scale", "float"),
                        new ParamInfo("y_scale", "float"),
                        new ParamInfo("center_point", "Vector2")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_vector",
                    "make_float3x2_scale",
                    [new ParamInfo("scales", "Vector2")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_vector",
                    "make_float3x2_scale",
                    [new ParamInfo("scales", "Vector2"), new ParamInfo("center_point", "Vector2")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_scalar",
                    "make_float3x2_scale",
                    [new ParamInfo("scale", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_scalar",
                    "make_float3x2_scale",
                    [new ParamInfo("scale", "float"), new ParamInfo("center_point", "Vector2")],
                    NoMinGW: true
                ),
                new(
                    "make_skew",
                    "make_float3x2_skew",
                    [new ParamInfo("radians_x", "float"), new ParamInfo("radians_y", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_skew",
                    "make_float3x2_skew",
                    [
                        new ParamInfo("radians_x", "float"),
                        new ParamInfo("radians_y", "float"),
                        new ParamInfo("center_point", "Vector2")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_rotation",
                    "make_float3x2_rotation",
                    [new ParamInfo("radians", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation",
                    "make_float3x2_rotation",
                    [new ParamInfo("radians", "float"), new ParamInfo("center_point", "Vector2")],
                    NoMinGW: true
                ),
            }
        },
        {
            "Matrix4x4",
            new List<FactoryInfo>
            {
                new(
                    "make_billboard",
                    "make_float4x4_billboard",
                    [
                        new ParamInfo("object_position", "Vector3"),
                        new ParamInfo("camera_position", "Vector3"),
                        new ParamInfo("camera_up_vector", "Vector3"),
                        new ParamInfo("camera_forward_vector", "Vector3")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_constrained_billboard",
                    "make_float4x4_constrained_billboard",
                    [
                        new ParamInfo("object_position", "Vector3"),
                        new ParamInfo("camera_position", "Vector3"),
                        new ParamInfo("rotate_axis", "Vector3"),
                        new ParamInfo("camera_forward_vector", "Vector3"),
                        new ParamInfo("object_forward_vector", "Vector3")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_translation",
                    "make_float4x4_translation",
                    [new ParamInfo("position", "Vector3")]
                ),
                new(
                    "make_translation",
                    "make_float4x4_translation",
                    [
                        new ParamInfo("x_position", "float"),
                        new ParamInfo("y_position", "float"),
                        new ParamInfo("z_position", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_scale",
                    "make_float4x4_scale",
                    [
                        new ParamInfo("x_scale", "float"),
                        new ParamInfo("y_scale", "float"),
                        new ParamInfo("z_scale", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_scale",
                    "make_float4x4_scale",
                    [
                        new ParamInfo("x_scale", "float"),
                        new ParamInfo("y_scale", "float"),
                        new ParamInfo("z_scale", "float"),
                        new ParamInfo("center_point", "Vector3")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_vector",
                    "make_float4x4_scale",
                    [new ParamInfo("scales", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_vector",
                    "make_float4x4_scale",
                    [new ParamInfo("scales", "Vector3"), new ParamInfo("center_point", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_scalar",
                    "make_float4x4_scale",
                    [new ParamInfo("scale", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_scale_from_scalar",
                    "make_float4x4_scale",
                    [new ParamInfo("scale", "float"), new ParamInfo("center_point", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_x",
                    "make_float4x4_rotation_x",
                    [new ParamInfo("radians", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_x",
                    "make_float4x4_rotation_x",
                    [new ParamInfo("radians", "float"), new ParamInfo("center_point", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_y",
                    "make_float4x4_rotation_y",
                    [new ParamInfo("radians", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_y",
                    "make_float4x4_rotation_y",
                    [new ParamInfo("radians", "float"), new ParamInfo("center_point", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_z",
                    "make_float4x4_rotation_z",
                    [new ParamInfo("radians", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_rotation_z",
                    "make_float4x4_rotation_z",
                    [new ParamInfo("radians", "float"), new ParamInfo("center_point", "Vector3")],
                    NoMinGW: true
                ),
                new(
                    "make_from_axis_angle",
                    "make_float4x4_from_axis_angle",
                    [new ParamInfo("axis", "Vector3"), new ParamInfo("angle", "float")],
                    NoMinGW: true
                ),
                new(
                    "make_perspective_field_of_view",
                    "make_float4x4_perspective_field_of_view",
                    [
                        new ParamInfo("field_of_view", "float"),
                        new ParamInfo("aspect_ratio", "float"),
                        new ParamInfo("near_plane_distance", "float"),
                        new ParamInfo("far_plane_distance", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_perspective",
                    "make_float4x4_perspective",
                    [
                        new ParamInfo("width", "float"),
                        new ParamInfo("height", "float"),
                        new ParamInfo("near_plane_distance", "float"),
                        new ParamInfo("far_plane_distance", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_perspective_off_center",
                    "make_float4x4_perspective_off_center",
                    [
                        new ParamInfo("left", "float"),
                        new ParamInfo("right", "float"),
                        new ParamInfo("bottom", "float"),
                        new ParamInfo("top", "float"),
                        new ParamInfo("near_plane_distance", "float"),
                        new ParamInfo("far_plane_distance", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_orthographic",
                    "make_float4x4_orthographic",
                    [
                        new ParamInfo("width", "float"),
                        new ParamInfo("height", "float"),
                        new ParamInfo("z_near_plane", "float"),
                        new ParamInfo("z_far_plane", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_orthographic_off_center",
                    "make_float4x4_orthographic_off_center",
                    [
                        new ParamInfo("left", "float"),
                        new ParamInfo("right", "float"),
                        new ParamInfo("bottom", "float"),
                        new ParamInfo("top", "float"),
                        new ParamInfo("z_near_plane", "float"),
                        new ParamInfo("z_far_plane", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_look_at",
                    "make_float4x4_look_at",
                    [
                        new ParamInfo("camera_position", "Vector3"),
                        new ParamInfo("camera_target", "Vector3"),
                        new ParamInfo("camera_up_vector", "Vector3")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_world",
                    "make_float4x4_world",
                    [
                        new ParamInfo("position", "Vector3"),
                        new ParamInfo("forward", "Vector3"),
                        new ParamInfo("up", "Vector3")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_from_quaternion",
                    "make_float4x4_from_quaternion",
                    [new ParamInfo("quaternion", "Quaternion")]
                ),
                new(
                    "make_from_yaw_pitch_roll",
                    "make_float4x4_from_yaw_pitch_roll",
                    [
                        new ParamInfo("yaw", "float"),
                        new ParamInfo("pitch", "float"),
                        new ParamInfo("roll", "float")
                    ],
                    NoMinGW: true
                ),
                new(
                    "make_shadow",
                    "make_float4x4_shadow",
                    [new ParamInfo("light_direction", "Vector3"), new ParamInfo("plane", "Plane")],
                    NoMinGW: true
                ),
                new(
                    "make_reflection",
                    "make_float4x4_reflection",
                    [new ParamInfo("value", "Plane")],
                    NoMinGW: true
                )
            }
        },
        {
            "Plane",
            new List<FactoryInfo>
            {
                new(
                    "make_from_vertices",
                    "make_plane_from_vertices",
                    [
                        new ParamInfo("point1", "Vector3"),
                        new ParamInfo("point2", "Vector3"),
                        new ParamInfo("point3", "Vector3")
                    ],
                    NoMinGW: true
                )
            }
        },
        {
            "Quaternion",
            new List<FactoryInfo>
            {
                new(
                    "make_from_axis_angle",
                    "make_quaternion_from_axis_angle",
                    [new ParamInfo("axis", "Vector3"), new ParamInfo("angle", "float")]
                ),
                new(
                    "make_from_yaw_pitch_roll",
                    "make_quaternion_from_yaw_pitch_roll",
                    [
                        new ParamInfo("yaw", "float"),
                        new ParamInfo("pitch", "float"),
                        new ParamInfo("roll", "float")
                    ]
                ),
                new(
                    "make_from_rotation_matrix",
                    "make_quaternion_from_rotation_matrix",
                    [new ParamInfo("matrix", "Matrix4x4")]
                )
            }
        },
    };

    private static readonly IReadOnlyDictionary<string, IReadOnlyCollection<MethodInfo>> operators =
        new Dictionary<string, IReadOnlyCollection<MethodInfo>>
        {
            {
                "Vector2",
                new List<MethodInfo>
                {
                    new("__add__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__sub__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__mul__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__mul__", "Vector2", [new ParamInfo("other", "float")]),
                    new("__rmul__", "Vector2", [new ParamInfo("other", "float")]),
                    new("__truediv__", "Vector2", [new ParamInfo("value", "Vector2")]),
                    new("__truediv__", "Vector2", [new ParamInfo("value", "float")]),
                    new("__neg__", "Vector2", []),
                    new("__iadd__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__isub__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__imul__", "Vector2", [new ParamInfo("other", "Vector2")]),
                    new("__imul__", "Vector2", [new ParamInfo("other", "float")]),
                    new("__itruediv__", "Vector2", [new ParamInfo("value", "Vector2")]),
                    new("__itruediv__", "Vector2", [new ParamInfo("value", "float")]),
                    new("__abs__", "float", []),
                }
            },
            {
                "Vector3",
                new List<MethodInfo>
                {
                    new("__add__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__sub__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__mul__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__mul__", "Vector3", [new ParamInfo("other", "float")]),
                    new("__rmul__", "Vector3", [new ParamInfo("other", "float")]),
                    new("__truediv__", "Vector3", [new ParamInfo("value", "Vector3")]),
                    new("__truediv__", "Vector3", [new ParamInfo("value", "float")]),
                    new("__neg__", "Vector3", []),
                    new("__iadd__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__isub__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__imul__", "Vector3", [new ParamInfo("other", "Vector3")]),
                    new("__imul__", "Vector3", [new ParamInfo("other", "float")]),
                    new("__itruediv__", "Vector3", [new ParamInfo("value", "Vector3")]),
                    new("__itruediv__", "Vector3", [new ParamInfo("value", "float")]),
                    new("__abs__", "float", []),
                }
            },
            {
                "Vector4",
                new List<MethodInfo>
                {
                    new("__add__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__sub__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__mul__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__mul__", "Vector4", [new ParamInfo("other", "float")]),
                    new("__rmul__", "Vector4", [new ParamInfo("other", "float")]),
                    new("__truediv__", "Vector4", [new ParamInfo("value", "Vector4")]),
                    new("__truediv__", "Vector4", [new ParamInfo("value", "float")]),
                    new("__neg__", "Vector4", []),
                    new("__iadd__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__isub__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__imul__", "Vector4", [new ParamInfo("other", "Vector4")]),
                    new("__imul__", "Vector4", [new ParamInfo("other", "float")]),
                    new("__itruediv__", "Vector4", [new ParamInfo("value", "Vector4")]),
                    new("__itruediv__", "Vector4", [new ParamInfo("value", "float")]),
                    new("__abs__", "float", []),
                }
            },
            {
                "Matrix3x2",
                new List<MethodInfo>
                {
                    new("__add__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__sub__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__mul__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__mul__", "Matrix3x2", [new ParamInfo("other", "float")]),
                    new("__neg__", "Matrix3x2", []),
                    new("__iadd__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__isub__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__imul__", "Matrix3x2", [new ParamInfo("other", "Matrix3x2")]),
                    new("__imul__", "Matrix3x2", [new ParamInfo("other", "float")]),
                }
            },
            {
                "Matrix4x4",
                new List<MethodInfo>
                {
                    new("__add__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__sub__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__mul__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__mul__", "Matrix4x4", [new ParamInfo("other", "float")]),
                    new("__neg__", "Matrix4x4", []),
                    new("__iadd__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__isub__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__imul__", "Matrix4x4", [new ParamInfo("other", "Matrix4x4")]),
                    new("__imul__", "Matrix4x4", [new ParamInfo("other", "float")]),
                }
            },
            {
                "Quaternion",
                new List<MethodInfo>
                {
                    new("__add__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__sub__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__mul__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__mul__", "Quaternion", [new ParamInfo("other", "float")]),
                    new("__truediv__", "Quaternion", [new ParamInfo("value", "Quaternion")]),
                    new("__neg__", "Quaternion", []),
                    new("__iadd__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__isub__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__imul__", "Quaternion", [new ParamInfo("other", "Quaternion")]),
                    new("__imul__", "Quaternion", [new ParamInfo("other", "float")]),
                    new("__itruediv__", "Quaternion", [new ParamInfo("value", "Quaternion")]),
                    new("__abs__", "float", []),
                }
            }
        };

    private static readonly IReadOnlyDictionary<
        string,
        IReadOnlyCollection<MethodInfo>
    > extraMethods = new Dictionary<string, IReadOnlyCollection<MethodInfo>>
    {
        {
            "Vector2",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector2")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector2")]),
                new("dot", "float", [new ParamInfo("value", "Vector2")]),
                new("normalize", "Vector2", []),
                new("reflect", "Vector2", [new ParamInfo("normal", "Vector2")]),
                new("min", "Vector2", [new ParamInfo("value", "Vector2")]),
                new("max", "Vector2", [new ParamInfo("value", "Vector2")]),
                new(
                    "clamp",
                    "Vector2",
                    [new ParamInfo("min", "Vector2"), new ParamInfo("max", "Vector2")]
                ),
                new(
                    "lerp",
                    "Vector2",
                    [new ParamInfo("value", "Vector2"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector2", [new ParamInfo("matrix", "Matrix3x2")], NoMinGW: true),
                new("transform", "Vector2", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new("transform", "Vector2", [new ParamInfo("rotation", "Quaternion")]),
                new(
                    "transform_normal",
                    "Vector2",
                    [new ParamInfo("matrix", "Matrix3x2")],
                    NoMinGW: true
                ),
                new(
                    "transform_normal",
                    "Vector2",
                    [new ParamInfo("matrix", "Matrix4x4")],
                    NoMinGW: true
                ),
                new("transform4", "Vector4", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new("transform4", "Vector4", [new ParamInfo("rotation", "Quaternion")])
            }
        },
        {
            "Vector3",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector3")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector3")]),
                new("dot", "float", [new ParamInfo("value", "Vector3")]),
                new("cross", "Vector3", [new ParamInfo("value", "Vector3")]),
                new("normalize", "Vector3", []),
                new("reflect", "Vector3", [new ParamInfo("normal", "Vector3")]),
                new("min", "Vector3", [new ParamInfo("value", "Vector3")]),
                new("max", "Vector3", [new ParamInfo("value", "Vector3")]),
                new(
                    "clamp",
                    "Vector3",
                    [new ParamInfo("min", "Vector3"), new ParamInfo("max", "Vector3")]
                ),
                new(
                    "lerp",
                    "Vector3",
                    [new ParamInfo("value", "Vector3"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector3", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new(
                    "transform",
                    "Vector3",
                    [new ParamInfo("rotation", "Quaternion")],
                    NoMinGW: true
                ),
                new(
                    "transform_normal",
                    "Vector3",
                    [new ParamInfo("matrix", "Matrix4x4")],
                    NoMinGW: true
                ),
                new("transform4", "Vector4", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new("transform4", "Vector4", [new ParamInfo("rotation", "Quaternion")]),
            }
        },
        {
            "Vector4",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector4")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector4")]),
                new("dot", "float", [new ParamInfo("value", "Vector4")]),
                new("normalize", "Vector4", []),
                new("min", "Vector4", [new ParamInfo("value", "Vector4")]),
                new("max", "Vector4", [new ParamInfo("value", "Vector4")]),
                new(
                    "clamp",
                    "Vector4",
                    [new ParamInfo("min", "Vector4"), new ParamInfo("max", "Vector4")]
                ),
                new(
                    "lerp",
                    "Vector4",
                    [new ParamInfo("value", "Vector4"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector4", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new("transform", "Vector4", [new ParamInfo("rotation", "Quaternion")]),
            }
        },
        {
            "Matrix3x2",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("determinant", "float", []),
                new("translation", "Vector2", []),
                new("invert", "Matrix3x2", [], NoMinGW: true),
                new(
                    "lerp",
                    "Matrix3x2",
                    [new ParamInfo("value", "Matrix3x2"), new ParamInfo("amount", "float")],
                    NoMinGW: true
                ),
            }
        },
        {
            "Matrix4x4",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("determinant", "float", []),
                new("translation", "Vector2", []),
                new("invert", "Matrix4x4", [], NoMinGW: true),
                new("decompose", "typing.Tuple[Vector3, Quaternion, Vector3]", [], NoMinGW: true),
                new(
                    "transform",
                    "Matrix4x4",
                    [new ParamInfo("rotation", "Quaternion")],
                    NoMinGW: true
                ),
                new("transpose", "Matrix4x4", []),
                new(
                    "lerp",
                    "Matrix4x4",
                    [new ParamInfo("value", "Matrix4x4"), new ParamInfo("amount", "float")],
                    NoMinGW: true
                )
            }
        },
        {
            "Plane",
            new List<MethodInfo>
            {
                new("normalize", "Plane", []),
                new("transform", "Plane", [new ParamInfo("matrix", "Matrix4x4")], NoMinGW: true),
                new("transform", "Plane", [new ParamInfo("rotation", "Quaternion")]),
                new("dot", "float", [new ParamInfo("value", "Vector4")], NoMinGW: true),
                new("dot_coordinate", "float", [new ParamInfo("value", "Vector3")], NoMinGW: true),
                new("dot_normal", "float", [new ParamInfo("value", "Vector3")], NoMinGW: true),
            }
        },
        {
            "Quaternion",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("length", "float", []),
                new("length_squared", "float", []),
                new("dot", "float", [new ParamInfo("value", "Quaternion")]),
                new("normalize", "Quaternion", []),
                new("conjugate", "Quaternion", []),
                new("inverse", "Quaternion", []),
                new(
                    "slerp",
                    "Quaternion",
                    [new ParamInfo("value", "Quaternion"), new ParamInfo("amount", "float")]
                ),
                new(
                    "lerp",
                    "Quaternion",
                    [new ParamInfo("value", "Quaternion"), new ParamInfo("amount", "float")]
                ),
                new("concatenate", "Quaternion", [new ParamInfo("value", "Quaternion")]),
            }
        }
    };

    public static void WriteNumberCommonValuesPyTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        ref bool pass
    )
    {
        if (!commonValues.TryGetValue(type.Name, out var values))
        {
            return;
        }

        pass = false;

        foreach (var value in values)
        {
            w.WriteLine($"@_property");
            w.WriteLine($"def {value}(self) -> {type.Name}: ...");
        }
    }

    public static void WriteNumberCommonValueMethods(this IndentedTextWriter w, ProjectedType type)
    {
        if (!commonValues.TryGetValue(type.Name, out var values))
        {
            return;
        }

        foreach (var value in values)
        {
            w.WriteBlankLine();
            w.WriteLine(
                $"static PyObject* _get_{value}_{type.Name}(PyObject* /*unused*/, void* /*unused*/) noexcept"
            );
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine($"return py::convert({type.CppWinrtType}::{value}());");
            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteNumberCommonValuesGetSetDefs(
        this IndentedTextWriter w,
        ProjectedType type
    )
    {
        if (!commonValues.TryGetValue(type.Name, out var values))
        {
            return;
        }

        foreach (var value in values)
        {
            w.WriteLine($"{{ \"{value}\", _get_{value}_{type.Name}, nullptr, nullptr, nullptr }},");
        }
    }

    public static void WriteNumberFactoryFunctionPyTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        ref bool pass
    )
    {
        if (!factoryFunctions.TryGetValue(type.Name, out var functions))
        {
            return;
        }

        pass = false;

        foreach (var func in functions)
        {
            var parameters = string.Join(
                "",
                func.Parameters.Select(p => $", {p.Name}: {p.PyType}")
            );

            if (functions.Count(f => f.PyName == func.PyName) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def {func.PyName}(self{parameters}) -> {type.Name}: ...");
        }
    }

    public static void WriteNumberFactoryFunctionMethodDefs(
        this IndentedTextWriter w,
        ProjectedType type
    )
    {
        if (!factoryFunctions.TryGetValue(type.Name, out var functions))
        {
            return;
        }

        foreach (var overloads in functions.GroupBy(o => o.PyName))
        {
            var func = overloads.First();

            w.WriteBlankLine();
            w.WriteLine(
                $"static PyObject* {func.PyName}_{type.Name}(PyObject* /*unused*/, PyObject* args) noexcept"
            );
            w.WriteLine("{");
            w.Indent++;

            w.WriteTryCatch(() =>
            {
                w.WriteLine("auto n_args = PyTuple_Size(args);");
                w.WriteBlankLine();

                foreach (var (i, overload) in overloads.Select((o, i) => (i, o)))
                {
                    w.WriteLine($"if (n_args == {overload.Parameters.Count})");
                    w.WriteLine("{");
                    w.Indent++;

                    if (overload.NoMinGW)
                    {
                        w.WriteLineNoTabs("#if defined(__MINGW32__)");
                        w.WriteLine("(void)args;");
                        w.WriteLine(
                            $"PyErr_SetString(PyExc_NotImplementedError, \"Overload with {overload.Parameters.Count} args is not implemented on MinGW\");"
                        );
                        w.WriteLine("return nullptr;");
                        w.WriteLineNoTabs("#else");
                    }

                    foreach (var (j, param) in overload.Parameters.Select((p, j) => (j, p)))
                    {
                        w.WriteLine(
                            $"auto _arg{j} = py::convert_to<{param.CppWinrtType}>(args, {j});"
                        );
                    }

                    w.WriteLine(
                        $"auto _result = winrt::Windows::Foundation::Numerics::{overload.CppWinrtName}({string.Join(", ", Enumerable.Range(0, overload.Parameters.Count).Select(j => $"_arg{j}"))});"
                    );
                    w.WriteLine("return py::convert(_result);");

                    if (overload.NoMinGW)
                    {
                        w.WriteLineNoTabs("#endif");
                    }

                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                }

                w.WriteLine(
                    "PyErr_Format(PyExc_TypeError, \"No overload take %d args.\", n_args);"
                );
                w.WriteLine("return nullptr;");
            });

            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteNumberFactoryFunctionDefs(this IndentedTextWriter w, ProjectedType type)
    {
        if (!factoryFunctions.TryGetValue(type.Name, out var functions))
        {
            return;
        }

        foreach (var func in functions.DistinctBy(f => f.PyName))
        {
            w.WriteLine(
                $"{{ \"{func.PyName}\", {func.PyName}_{type.Name}, METH_VARARGS, nullptr }},"
            );
        }
    }

    public static void WriteNumberSlotMethodsPyTyping(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var method in operators[type.Name])
        {
            var parameters = string.Join(
                "",
                method.Parameters.Select(p => $", {p.Name}: {p.PyType}")
            );

            if (operators[type.Name].Count(m => m.Name == method.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def {method.Name}(self{parameters}) -> {method.ReturnPyType}: ...");
        }
    }

    public static void WriteNumberMethodPyTyping(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var method in extraMethods[type.Name])
        {
            var parameters = string.Join(
                "",
                method.Parameters.Select(p => $", {p.Name}: {p.PyType}")
            );

            if (extraMethods[type.Name].Count(m => m.Name == method.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def {method.Name}(self{parameters}) -> {method.ReturnPyType}: ...");
        }
    }

    public static void WriteNumberMethods(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var overloads in extraMethods[type.Name].GroupBy(m => m.Name))
        {
            var method = overloads.First();
            var args = method.Parameters.Count switch
            {
                0 => "/*unused*/",
                1 => "arg",
                _ => "args"
            };

            w.WriteBlankLine();
            w.WriteLine(
                $"static PyObject* {method.Name}_{type.Name}(winrt_struct_wrapper<{type.CppWinrtType}>* self, PyObject* {args}) noexcept"
            );
            w.WriteLine("{");
            w.Indent++;
            w.WriteTryCatch(() =>
            {
                if (overloads.Count() == 1)
                {
                    if (method.NoMinGW)
                    {
                        w.WriteLineNoTabs("#if defined(__MINGW32__)");
                        w.WriteLine("(void)self;");

                        if (method.Parameters.Count > 0)
                        {
                            w.WriteLine($"(void){args};");
                        }

                        w.WriteLine(
                            "PyErr_SetString(PyExc_NotImplementedError, \"This method is not implemented on MinGW\");"
                        );
                        w.WriteLine("return nullptr;");
                        w.WriteLineNoTabs("#else");
                    }

                    switch (method.Parameters.Count)
                    {
                        case 0:
                            if (method.Name == "invert")
                            {
                                w.WriteLine($"{method.ReturnCppWinrtType} _result;");
                                w.WriteBlankLine();
                                w.WriteLine(
                                    $"if (!winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, &_result))"
                                );
                                w.WriteLine("{");
                                w.Indent++;
                                w.WriteLine(
                                    "PyErr_SetString(PyExc_ValueError, \"Matrix is not invertible\");"
                                );
                                w.WriteLine("return nullptr;");
                                w.Indent--;
                                w.WriteLine("}");
                                w.WriteBlankLine();
                                w.WriteLine("return py::convert(_result);");
                            }
                            else if (method.Name == "decompose")
                            {
                                w.WriteLine("winrt::Windows::Foundation::Numerics::float3 _out0;");
                                w.WriteLine(
                                    "winrt::Windows::Foundation::Numerics::quaternion _out1;"
                                );
                                w.WriteLine("winrt::Windows::Foundation::Numerics::float3 _out2;");
                                w.WriteBlankLine();
                                w.WriteLine(
                                    $"if (!winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, &_out0, &_out1, &_out2))"
                                );
                                w.WriteLine("{");
                                w.Indent++;
                                w.WriteLine(
                                    "PyErr_SetString(PyExc_ValueError, \"Matrix is not decomposable\");"
                                );
                                w.WriteLine("return nullptr;");
                                w.Indent--;
                                w.WriteLine("}");
                                w.WriteBlankLine();
                                w.WriteLine("py::pyobj_handle out0{py::convert(_out0)};");
                                w.WriteLine("py::pyobj_handle out1{py::convert(_out1)};");
                                w.WriteLine("py::pyobj_handle out2{py::convert(_out2)};");
                                w.WriteBlankLine();
                                w.WriteLine(
                                    "return PyTuple_Pack(3, out0.get(), out1.get(), out2.get());"
                                );
                            }
                            else
                            {
                                w.WriteLine(
                                    $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj);"
                                );
                                w.WriteLine("return py::convert(_result);");
                            }

                            break;
                        case 1:
                            var param = method.Parameters[0];
                            w.WriteLine($"auto _arg = py::convert_to<{param.CppWinrtType}>(arg);");
                            w.WriteLine(
                                $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, _arg);"
                            );
                            w.WriteLine("return py::convert(_result);");

                            break;
                        case 2:
                            var param0 = method.Parameters[0];
                            var param1 = method.Parameters[1];

                            w.WriteLine(
                                $"auto _arg0 = py::convert_to<{param0.CppWinrtType}>(args, 0);"
                            );
                            w.WriteLine(
                                $"auto _arg1 = py::convert_to<{param1.CppWinrtType}>(args, 1);"
                            );
                            w.WriteLine(
                                $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, _arg0, _arg1);"
                            );
                            w.WriteLine("return py::convert(_result);");

                            break;
                        default:
                            throw new NotImplementedException();
                    }

                    if (method.NoMinGW)
                    {
                        w.WriteLineNoTabs("#endif");
                    }
                }
                else
                {
                    var allOverloadsNotImplemented = overloads.All(o => o.NoMinGW);

                    if (allOverloadsNotImplemented)
                    {
                        w.WriteLineNoTabs("#if defined(__MINGW32__)");
                        w.WriteLine("(void)self;");

                        if (method.Parameters.Count > 0)
                        {
                            w.WriteLine($"(void){args};");
                        }

                        w.WriteLine(
                            "PyErr_SetString(PyExc_NotImplementedError, \"This method is not implemented on MinGW\");"
                        );
                        w.WriteLine("return nullptr;");
                        w.WriteLineNoTabs("#else");
                    }

                    switch (method.Parameters.Count)
                    {
                        case 1:
                            foreach (var (i, overload) in overloads.Select((o, i) => (i, o)))
                            {
                                var param = overload.Parameters[0];

                                if (i > 0)
                                {
                                    w.WriteBlankLine();
                                }

                                w.WriteLine(
                                    $"if (std::string_view(Py_TYPE(arg)->tp_name) == \"{param.PyType}\" || std::string_view(Py_TYPE(arg)->tp_name) == \"winrt._winrt_windows_foundation_numerics.{param.PyType}\")"
                                );
                                w.WriteLine("{");
                                w.Indent++;

                                if (!allOverloadsNotImplemented && overload.NoMinGW)
                                {
                                    w.WriteLineNoTabs("#if defined(__MINGW32__)");
                                    w.WriteLine(
                                        $"PyErr_SetString(PyExc_NotImplementedError, \"Overload for {param.PyType} is not implemented on MinGW\");"
                                    );
                                    w.WriteLine("return nullptr;");
                                    w.WriteLineNoTabs("#else");
                                }

                                w.WriteLine(
                                    $"auto _arg = py::convert_to<{param.CppWinrtType}>(arg);"
                                );
                                w.WriteLine(
                                    $"auto _result = winrt::Windows::Foundation::Numerics::{overload.Name}(self->obj, _arg);"
                                );
                                w.WriteLine("return py::convert(_result);");

                                if (!allOverloadsNotImplemented && overload.NoMinGW)
                                {
                                    w.WriteLineNoTabs("#endif");
                                }

                                w.Indent--;
                                w.WriteLine("}");
                            }

                            w.WriteBlankLine();
                            w.WriteLine(
                                $"PyErr_Format(PyExc_TypeError, \"Expecting one of {string.Join(", ", overloads.Select(o => $"'winrt._winrt_windows_foundation_numerics.{o.Parameters[0].PyType}'"))} but got '%s'\", Py_TYPE(arg)->tp_name);"
                            );
                            w.WriteLine("return nullptr;");
                            break;
                        default:
                            throw new NotImplementedException();
                    }

                    if (allOverloadsNotImplemented)
                    {
                        w.WriteLineNoTabs("#endif");
                    }
                }
            });
            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteNumberMethodDefs(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var method in extraMethods[type.Name].DistinctBy(m => m.Name))
        {
            var flags = method.Parameters.Count switch
            {
                0 => "METH_NOARGS",
                1 => "METH_O",
                _ => "METH_VARARGS",
            };

            w.WriteLine(
                $"{{ \"{method.Name}\", reinterpret_cast<PyCFunction>({method.Name}_{type.Name}), {flags}, nullptr }},"
            );
        }
    }

    static void WriteReturnNotImplementedOnTypeError(this IndentedTextWriter w)
    {
        w.WriteLine("py::to_PyErr();");
        w.WriteBlankLine();
        w.WriteLine("if (PyErr_ExceptionMatches(PyExc_TypeError))");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("PyErr_Clear();");
        w.WriteLine("Py_RETURN_NOTIMPLEMENTED;");
        w.Indent--;
        w.WriteLine("}");
        w.WriteBlankLine();
    }

    static void WriteNumberAdd(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _add_{type.Name}(PyObject* left, PyObject* right) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _left = py::convert_to<{type.CppWinrtType}>(left);");
                w.WriteLine($"auto _right = py::convert_to<{type.CppWinrtType}>(right);");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left + _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberSub(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _sub_{type.Name}(PyObject* left, PyObject* right) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _left = py::convert_to<{type.CppWinrtType}>(left);");
                w.WriteLine($"auto _right = py::convert_to<{type.CppWinrtType}>(right);");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left - _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberMul(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _mul_{type.Name}(PyObject* left, PyObject* right) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                if (
                    type.Name != "Matrix3x2"
                    && type.Name != "Matrix4x4"
                    && type.Name != "Quaternion"
                )
                {
                    w.WriteLine("py::pyobj_handle left_float{PyNumber_Float(left)};");
                    w.WriteLine("if (left_float)");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("auto _left_float = PyFloat_AsDouble(left_float.get());");
                    w.WriteLine("if (_left_float == -1 && PyErr_Occurred())");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("return nullptr;");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine(
                        $"auto _result = static_cast<float>(_left_float) * py::convert_to<{type.CppWinrtType}>(right);"
                    );
                    w.WriteLine($"return py::convert(_result);");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine("else");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("PyErr_Clear();");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                }

                w.WriteLine($"auto _left = py::convert_to<{type.CppWinrtType}>(left);");
                w.WriteBlankLine();
                w.WriteLine("py::pyobj_handle right_float{PyNumber_Float(right)};");
                w.WriteLine("if (right_float)");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("auto _right_float = PyFloat_AsDouble(right_float.get());");
                w.WriteLine("if (_right_float == -1 && PyErr_Occurred())");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("return nullptr;");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left * static_cast<float>(_right_float);");
                w.WriteLine($"return py::convert(_result);");
                w.Indent--;
                w.WriteLine("}");
                w.WriteLine("else");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("PyErr_Clear();");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine($"auto _right = py::convert_to<{type.CppWinrtType}>(right);");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left * _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberDiv(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine(
            $"static PyObject* _truediv_{type.Name}(PyObject* left, PyObject* right) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _left = py::convert_to<{type.CppWinrtType}>(left);");

                if (type.Name != "Quaternion")
                {
                    w.WriteBlankLine();
                    w.WriteLine("py::pyobj_handle right_float{PyNumber_Float(right)};");
                    w.WriteLine("if (right_float)");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("auto _right_float = PyFloat_AsDouble(right_float.get());");
                    w.WriteLine("if (_right_float == -1 && PyErr_Occurred())");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("return nullptr;");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                    w.WriteLine($"auto _result = _left / static_cast<float>(_right_float);");
                    w.WriteLine($"return py::convert(_result);");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine("else");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("PyErr_Clear();");
                    w.Indent--;
                    w.WriteLine("}");
                }

                w.WriteLine($"auto _right = py::convert_to<{type.CppWinrtType}>(right);");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left / _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberNeg(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _neg_{type.Name}(PyObject* operand) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _operand = py::convert_to<{type.CppWinrtType}>(operand);");
                w.WriteLine($"auto _result = -_operand;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberAbs(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _abs_{type.Name}(PyObject* operand) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _operand = py::convert_to<{type.CppWinrtType}>(operand);");
                w.WriteLine(
                    $"auto _result = winrt::Windows::Foundation::Numerics::length(_operand);"
                );
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    public static void WriteNumberSlotMethods(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteNumberAdd(type);
        w.WriteBlankLine();
        w.WriteNumberSub(type);
        w.WriteBlankLine();
        w.WriteNumberMul(type);

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal))
        {
            w.WriteBlankLine();
            w.WriteNumberDiv(type);
        }

        w.WriteBlankLine();
        w.WriteNumberNeg(type);

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal))
        {
            w.WriteBlankLine();
            w.WriteNumberAbs(type);
        }
    }

    public static void WriteNumberSlots(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"{{ Py_nb_add, reinterpret_cast<void*>(_add_{type.Name}) }},");
        w.WriteLine($"{{ Py_nb_subtract, reinterpret_cast<void*>(_sub_{type.Name}) }},");
        w.WriteLine($"{{ Py_nb_multiply, reinterpret_cast<void*>(_mul_{type.Name}) }},");

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal))
        {
            w.WriteLine($"{{ Py_nb_true_divide, reinterpret_cast<void*>(_truediv_{type.Name}) }},");
        }

        w.WriteLine($"{{ Py_nb_negative, reinterpret_cast<void*>(_neg_{type.Name}) }},");

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal) && type.Name != "Plane")
        {
            w.WriteLine($"{{ Py_nb_absolute, reinterpret_cast<void*>(_abs_{type.Name}) }},");
        }
    }
}

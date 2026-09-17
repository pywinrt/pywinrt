# The projection table format

A projection package ships one table per WinRT namespace, next to the
`__init__.py` of that namespace. It holds the namespace's types, their members,
the vtable slots those members sit in, and everything the runtime needs to
convert arguments and results. That `__init__.py` hands the table to
`winrt-runtime`, which builds the Python types from it. Nothing in a projection
package is compiled, which is why its wheel is `py3-none-any`.

The table has two forms:

| form | file | written by | where it lives |
|---|---|---|---|
| text | `_table.pywinrt.txt` | `PyWinRT.exe` (`PyWinRT/TableWriter.cs`) | the repository, and a package's own source |
| binary | `_table.pywinrt` | `winrt/table.py` in `winrt-runtime` | whatever a build installs |

**The text is the form that is kept.** A generated binary in a commit is a diff
nobody can read, and so a place to hide things; what a reviewer has to see when
the generator changes is what the generator now says about a type. The binary is
compiled from the text when the projection is built, and it is the only form
the runtime reads, because it is mapped rather than parsed: everything in it is
an offset or an index.

The two forms say the same things. The text names its records where the binary
numbers them, and the order the records are written in is the order they are
numbered in, so nothing else in the binary - the pools, the offsets, the sizes -
is a decision the compiler makes.

This file is the contract between the three ends: the generator that writes the
text, `winrt/table.py` that compiles it, and `runtime/src/table.cpp` that reads
the result. It is the table's equivalent of what `pywinrt/abi.h` is for compiled
consumers, and it carries a version of its own for the same reason.

## Versioning

The header carries a major and a minor version.

- **Minor** is bumped when something is *added*: a new section, a new type code,
  a new flag, a new field at the end of a record. A runtime refuses a table
  whose minor is higher than its own, because such a table may say something
  the runtime would silently ignore. It reads any lower minor, because
  everything a lower minor says is still said the same way.
- **Major** is bumped when anything already written changes meaning, moves or
  goes away. A runtime refuses any table whose major is not its own.

The refusal is an `ImportError` that names the package and both versions, the
same shape as the message a compiled module gets from `import_winrt_runtime()`.

The major is the **compatibility generation**. It is the same number as the C
ABI major in `pywinrt/abi.h` and as the epoch in every projection package's
version, as in `winrt-Microsoft.UI.Xaml==4!2.4.0`. All three change on one event:
the one where every projection package has to be republished. Breaking either
contract is that event, so they move together rather than becoming two
generations that have to be explained separately. `table.cpp` asserts the two
majors against each other at compile time.

The minors are independent, and they are checked in opposite directions. The C
ABI minor counts capsule additions, and a *compiled module* refuses a runtime
whose minor is lower than the one it was built against. The format minor counts
additions to a table, and the *runtime* refuses a table whose minor is higher
than its own. A capsule addition changes nothing in a table and a new table
section changes nothing in the capsule, so neither minor says anything about the
other.

A projection package does not report an `_abi_version_` at all. It contains no
compiled code, so the C ABI is not a contract it is party to.

## Shape ids are append-only

A member's descriptor carries the id of its **ABI call shape**, which is an
index into the `forward_shapes[]` array that `runtime/src/shapes-generated.h`
defines. The same is true of the reverse trampoline id that a delegate or an
implementable interface carries.

Those ids are assigned once, in `runtime/src/shapes.json`, and never renumbered.
A projection package pins `winrt-runtime` with a lower bound rather than an
exact version, so a table written today has to be readable by a runtime built
from a census that has since grown. Appending is safe; renumbering would
silently call the wrong trampoline. `shapes.json` is therefore checked in, and
the census that `PyWinRT.exe` takes while it generates only ever adds to it.

The header records one past the highest id the table uses, so that a runtime
whose `shapes-generated.h` is older than the table can say so at import rather
than index out of bounds on the first call.

## What a shape is

Every WinRT method is `HRESULT __stdcall f(void* this, ...)` and every output is
a pointer, so the only thing that varies between two calls is the list of
arguments. A shape is that list, and two members with the same shape are called
by the same trampoline whatever they mean.

An argument is one of:

| code | C++ spelling | what it covers |
|---|---|---|
| `p` | `void*` | an interface pointer, an `HSTRING`, any output pointer, either half of an array parameter, and every integer that fits in a register: a boolean, a `char16`, an enum, an `int8` through `uint32` |
| `i8` | `int64_t` | a 64 bit integer, which is one register but **two** stack slots on x86 |
| `f4` | `float` | |
| `f8` | `double` | |
| `{...}` | a generated struct | a struct passed by value, spelled faithfully from its flattened field list |

Folding every register-width integer into `p` is what keeps the census small.
All three supported calling conventions put a narrow integer in the low bits of
one register or of one four byte stack slot and let the callee ignore the rest,
so a trampoline that passes the widened value calls a member that takes a
boolean, an enum, an `int32` or an interface pointer equally well. **The
interpreter therefore widens to register width when it fills the argument
buffer**: it writes the whole slot, not just the bytes the parameter's own type
occupies.

A struct is not widened and not summarised by its size, because a calling
convention looks at the field types and the alignment: a `{float,float}` Point
travels in one general purpose register on x64 and in two floating point
registers on arm64, and a 24 byte struct travels as a pointer to a copy on x64
and in three general purpose registers on arm64. So `shapes-generated.h` spells
each layout as a C++ struct and lets the compiler implement the convention. The
flattened field list is faithful: WinRT structs are never packed, so a nested
struct has the same size, alignment and floating point homogeneity as its fields
spelled in place.

The shape is therefore the same on every architecture, which is what lets one
`py3-none-any` table serve all of them. What differs per architecture is only
how many distinct machine level trampolines survive the linker folding identical
bodies, which is a size number and not part of the contract.

## The text form

A table is its list of type records, written in the order they are numbered,
each one followed by everything that belongs to it. Indentation says what a line
is about: a type at the left margin, then what the type says about itself and
the groups of members it binds, then the members of one of those groups, then
the parameters of one of those members. Only the fixed head of a line is read by
position. Everything optional after it is a `name=value` token or a bare flag
word, so a flag is never mistaken for a name.

```
format 4.0
generator 3.2.1
namespace Windows.Foundation

class Windows.Foundation.Uri python_type stringable
    py Uri
    signature rc(Windows.Foundation.Uri;{9e365e57-48b2-4160-956f-c7385120bbfc})
    default Windows.Foundation.IUriRuntimeClass
    requires Windows.Foundation.IUriRuntimeClass
    requires Windows.Foundation.IStringable
    factory Windows.Foundation.IUriRuntimeClassFactory
    constructor -
        ctor .ctor slot=6 inputs=1 outputs=1 declaring=Windows.Foundation.IUriRuntimeClassFactory shape=51
            in string name=uri
            out class type=Windows.Foundation.Uri return
    property absolute_uri
        get get_AbsoluteUri slot=6 inputs=0 outputs=1 declaring=Windows.Foundation.IUriRuntimeClass shape=4
            out string return
```

The three lines before the first type are the header: the format version the
text is written to, the version of the generator that wrote it, and the WinRT
namespace the table is for. A blank line, and a line whose first non-space
character is `#`, say nothing.

### How a type is named

A type is written as its WinRT namespace and name, and a parameterized one
carries the names of its arguments in angle brackets as well -
`Windows.Foundation.Collections.IVector<Windows.Foundation.Uri>` - which is what
tells the record of an instance from the record of the definition it is an
instance of. That name is unique within a table and is what every reference to
the record is written as; the compiler splits the namespace and the name back
out of it and numbers the records in the order they appear. A type argument that
is still standing in for a type belongs to no namespace, and is written as the
bare name the metadata gives it, `T`.

### The lines of a type

A type's own line is its category - `enum`, `struct`, `interface`, `class` or
`delegate` - then its name, then its flags. The lines indented under it are:

| line | what it says |
|---|---|
| `py <name>` | the attribute this type is bound to in its module |
| `guid <guid>` | its IID |
| `signature <text>` | its WinRT signature |
| `base <type>` | its base type |
| `default <type>` | its default interface |
| `requires <type>` | one required interface, in the order to try them |
| `argument <type>` | one type argument |
| `factory <type>` | one activation factory interface |
| `composable <type>` | one composable factory interface |
| `overridable <type>` | one overridable or protected interface |
| `field <py> <winrt> <code>` | one struct field, with `type=` when the code names a type |
| `<kind> <name>` | a group of members bound to one Python attribute, then the group's flags |

A group's name is a dash when the group is bound to no name of its own, which a
constructor group and the `Invoke` of a delegate are.

A member's line is its kind, the WinRT name of the method, then `slot=`,
`inputs=`, `outputs=`, `declaring=`, `shape=` and `reverse=` - of which only the
first three are always written - then its flags. A parameter's line is its
category, its type code, then `type=` and `name=` where it has them, then
`return`, `implicit` and `by_reference`.

The categories, kinds, codes, roles and flags are named in the tables of the
next section, beside the numbers they are written as in the binary.

## The binary layout

Every integer is little-endian. Every offset is a byte offset from the start of
the file. Nothing in the file is a pointer, and nothing in it depends on the
width of one, so the same bytes are read the same way by an x86, an x64 and an
arm64 runtime.

### Header, 48 bytes

| offset | type | field |
|---|---|---|
| 0 | `char[8]` | magic, `PYWINRT\x1a` |
| 8 | `u16` | format major |
| 10 | `u16` | format minor |
| 12 | `u32` | file size, which must equal the size of the file |
| 16 | `u32` | section count |
| 20 | `u32` | offset of the section directory |
| 24 | `u32` | generator version, a string ref |
| 28 | `u32` | WinRT namespace, a string ref, e.g. `Windows.Foundation` |
| 32 | `u32` | one past the highest forward shape id used |
| 36 | `u32` | one past the highest reverse shape id used |
| 40 | `u32` | reserved, zero |
| 44 | `u32` | reserved, zero |

### Section directory

`section_count` entries of twelve bytes each, sorted by tag:

| offset | type | field |
|---|---|---|
| 0 | `u32` | tag, four ASCII characters, first character in the low byte |
| 4 | `u32` | offset of the section |
| 8 | `u32` | size of the section in bytes |

Every section starts on an eight byte boundary. A reader looks a section up by
tag and ignores tags it does not know, which is how a minor version adds one. A
tag appears at most once.

| tag | contents |
|---|---|
| `STRS` | the string blob |
| `GUID` | 16 byte GUIDs, in the memory order `winrt::guid` uses |
| `REFS` | a pool of `u32` that the variable length lists point into |
| `TYPE` | type records |
| `GRUP` | member group records |
| `MEMB` | member records |
| `PARM` | parameter records |
| `FLDS` | struct field records |

### References

- A **string ref** is a `u32` byte offset into `STRS`. The blob begins with a
  NUL, so offset zero is the empty string. Strings are UTF-8 and NUL terminated.
- A **type ref** is a `u32` index into `TYPE`, or `0xFFFFFFFF` for none.
- A **list** is a `u32` index into `REFS` together with a `u32` count. The
  elements are type refs unless the field says otherwise. An empty list has a
  count of zero and an ignored index.
- A **guid ref** is a `u32` index into `GUID`, or `0xFFFFFFFF` for none.

### Type record, 88 bytes

| index | field |
|---|---|
| 0 | flags, see below |
| 1 | WinRT name, a string ref, without the generic arity suffix, e.g. `IIterable` |
| 2 | WinRT namespace, a string ref |
| 3 | Python name, a string ref: the attribute this type is bound to in its module, which for an interface is the name with a leading underscore |
| 4 | WinRT signature, a string ref, or empty when it was not computed |
| 5 | guid ref |
| 6 | base type, a type ref |
| 7 | default interface, a type ref |
| 8, 9 | required interfaces, a list, transitively closed, in the order to try them |
| 10, 11 | type arguments of a concrete instance, a list |
| 12, 13 | member groups, a first index into `GRUP` and a count |
| 14, 15 | struct fields, a first index into `FLDS` and a count |
| 16, 17 | activation factory interfaces, a list |
| 18, 19 | composable factory interfaces, a list |
| 20, 21 | overridable and protected interfaces, a list |

The records of the types the namespace defines come first, sorted by WinRT name,
so that a lookup by name is a binary search. The external references follow in
the order they were first needed.

Type flags, and the words the text writes them as:

| bit | name | meaning |
|---|---|---|
| 0-2 | | category: 0=`enum`, 1=`struct`, 2=`interface`, 3=`class`, 4=`delegate`, which is the first word of a type's line |
| 3 | `external` | the type belongs to another namespace and is resolved by name through the type registry |
| 4 | `static` | static class |
| 5 | `composable` | composable class |
| 6 | `parameterized` | parameterized type |
| 7 | `concrete` | concrete instance |
| 8 | `activatable` | default activatable |
| 9 | `python_type` | the type gets a Python type object of its own in this module |
| 10 | `iterable` | implements `IIterable<T>` |
| 11 | `iterator` | implements `IIterator<T>` |
| 12 | `sequence` | implements `IVectorView<T>` |
| 13 | `mutable_sequence` | implements `IVector<T>` |
| 14 | `mapping` | implements `IMapView<K, V>` |
| 15 | `mutable_mapping` | implements `IMap<K, V>` |
| 16 | `awaitable` | implements `IAsyncInfo` and is not `IAsyncInfo` |
| 17 | `closeable` | implements `IClosable` |
| 18 | `stringable` | implements `IStringable` |
| 19 | `buffer` | supports the buffer protocol |
| 20 | `buffer_length` | the buffer length is `Length` rather than `Capacity` |

### Group record, 16 bytes

A group is one Python attribute. Its members are contiguous in `MEMB`.

| index | field |
|---|---|
| 0 | Python name, a string ref; empty for a constructor group |
| 1 | flags |
| 2 | first member, an index into `MEMB` |
| 3 | member count |

Group flags:

| bit | name | meaning |
|---|---|---|
| 0-2 | | kind: 0=`method`, 1=`property`, 2=`event`, 3=`constructor`, which is the first word of a group's line |
| 3 | `static` | the attribute belongs on the metaclass |
| 4 | `deprecated` | |

A property group holds the getter and, if there is one, the setter, in that
order. An event group holds the adder and the remover, in that order. A method
or constructor group holds one member per overload, sorted by the number of
Python input arguments, which is what selects between them at the call.

### Member record, 40 bytes

| index | field |
|---|---|
| 0 | flags |
| 1 | WinRT method name, a string ref, e.g. `get_Length`, for the error path |
| 2 | declaring interface, a type ref: the interface to query the object for |
| 3 | vtable slot |
| 4 | forward shape id |
| 5 | first parameter, an index into `PARM` |
| 6 | parameter count, including the return value |
| 7 | number of Python input arguments |
| 8 | number of Python output values |
| 9 | reverse shape id, or `0xFFFFFFFF` when nothing implements the member |

Member flags:

| bit | name | meaning |
|---|---|---|
| 0-2 | | kind: 0=`method`, 1=`get` (property), 2=`put` (property), 3=`add` (event), 4=`remove` (event), 5=`ctor`, which is the first word of a member's line |
| 3 | `static` | |
| 4 | `overridable` | |
| 5 | `protected` | |
| 6 | `deprecated` | |
| 7 | `default_overload` | |

The vtable slot is six plus the position of the method in the metadata of the
interface that declares it, because the first six entries are `IUnknown`'s three
and `IInspectable`'s three. A delegate's `Invoke` is slot three, since a
delegate does not derive from `IInspectable`.

### Parameter record, 16 bytes

| index | field |
|---|---|
| 0 | flags |
| 1 | type code |
| 2 | type ref, or `0xFFFFFFFF` when the type code says everything |
| 3 | WinRT parameter name, a string ref, for the error path; empty for the return value |

A parameter is not named in Python - the projection passes them positionally -
so, unlike a struct field, only the name the metadata gives it is here.

Parameter flags:

| bit | name | meaning |
|---|---|---|
| 0-2 | | category: 0=`in`, 1=`out`, 2=`pass_array`, 3=`fill_array`, 4=`receive_array`, which is the first word of a parameter's line |
| 3 | `return` | this is the return value rather than a declared parameter |
| 4 | `implicit` | the ABI passes it, but Python neither supplies nor receives it |
| 5 | `by_reference` | the ABI passes a pointer to the value rather than the value |

A by-reference parameter is an input the metadata declares `ref`, which the
ABI passes as `T const*` - `T const&` in C++/WinRT's spelling. The type code
still names the value's own type; only how it travels is different. It matters
because on x86 a by-value struct is pushed inline and the callee pops it, so
passing one where a pointer is expected corrupts the stack. On x64 a struct
too wide for a register is passed as a pointer to a copy either way, which is
why getting this wrong is invisible there. Every other category is a pointer
already, so only an input carries the bit.

An implicit parameter is one the projection fills in itself. A composable
factory takes two of them after the constructor's own arguments - the outer
object a derived class is composed into and the non-delegating inner object it
gets back - so they are in the shape and in the parameter list, but they are not
counted in the Python input or output counts.

### Struct field record, 16 bytes

| index | field |
|---|---|
| 0 | Python name, a string ref |
| 1 | WinRT name, a string ref |
| 2 | type code |
| 3 | type ref, or `0xFFFFFFFF` |

### Type codes

The name is what the text writes; code 26 is a concrete parameterized interface
and code 27 is an `IReference<T>`.

| code | name | size | alignment |
|---|---|---|---|
| 0 | void | - | - |
| 1 | boolean | 1 | 1 |
| 2 | int8 | 1 | 1 |
| 3 | uint8 | 1 | 1 |
| 4 | int16 | 2 | 2 |
| 5 | uint16 | 2 | 2 |
| 6 | int32 | 4 | 4 |
| 7 | uint32 | 4 | 4 |
| 8 | int64 | 8 | 8 |
| 9 | uint64 | 8 | 8 |
| 10 | single | 4 | 4 |
| 11 | double | 8 | 8 |
| 12 | char16 | 2 | 2 |
| 13 | string | pointer | pointer |
| 14 | guid | 16 | 4 |
| 15 | hresult | 4 | 4 |
| 16 | event_token | 8 | 8 |
| 17 | object | pointer | pointer |
| 18 | datetime | 8 | 8 |
| 19 | timespan | 8 | 8 |
| 20 | enum32 | 4 | 4 |
| 21 | enum_u32 | 4 | 4 |
| 22 | struct | recursive | recursive |
| 23 | interface | pointer | pointer |
| 24 | class | pointer | pointer |
| 25 | delegate | pointer | pointer |
| 26 | parameterized | pointer | pointer |
| 27 | reference | pointer | pointer |

A struct's layout is **computed by the reader**, never written down, because the
width of a pointer is not the same on every architecture the table serves. The
rule is the ordinary one: each field starts at the next offset that is a
multiple of its alignment, and the size of the struct is the end of the last
field rounded up to the largest alignment in it. A boolean is one byte here,
whatever a given C++ compiler makes of `bool`, and a GUID is sixteen bytes
aligned to four, not to sixteen.

The one thing to be careful of is that none of this may be derived from how a
field is converted to or from Python. The width a converter asks for in a local
variable - an `int` to parse a boolean into, say - is not the width of the
field. The type code is the only thing that says how wide a field is.

## Validation

A reader checks, before it trusts anything:

- the magic, the format major, and that the format minor is not ahead of its own;
- that the recorded file size is the size of the buffer;
- that every section lies inside the buffer, is eight byte aligned and, for the
  record sections, has a size that is a whole number of records;
- that both shape id limits are within the arrays in `shapes-generated.h`;
- that the string blob begins with a NUL and ends with a NUL.

Everything else - a type ref, a list, a string ref - is bounds checked where it
is read, and an out of range reference raises rather than reading past the end.
The table is generated, so none of these are expected to fire; they are there so
that a mismatched pair of packages says what is wrong instead of crashing.

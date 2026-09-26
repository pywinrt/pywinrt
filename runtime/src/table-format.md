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
is a decision the compiler makes. The one thing the binary says that the text
does not is which version of the compiler wrote it, which is a fact about the
build and cannot be known to a file that is committed.

This file is the contract between the three ends: the generator that writes the
text, `winrt/table.py` that compiles it, and `runtime/src/table.cpp` that reads
the result, and it carries a version of its own.

## Versioning

The header carries a major and a minor version.

- **Minor** is bumped when something is *added*: a new section, a new type code,
  a new flag, a new field at the end of a record. A runtime refuses a table
  whose minor is higher than its own, because such a table may say something
  the runtime would silently ignore. It reads any lower minor, because
  everything a lower minor says is still said the same way.
- **Major** is bumped when anything already written changes meaning, moves or
  goes away. A runtime refuses any table whose major is not its own.

The minor starts counting at the first release of the format. Before that there
is no table anywhere that a runtime has to keep reading, so a change is not an
addition to anything and the records simply change at the minor the format
already has. The history of this file below its first release is therefore not a
sequence of minor bumps and does not read as one.

The refusal is an `ImportError` that names the package and both versions.

The major is the **compatibility generation**. It is the same number as the
epoch in every projection package's version, as in
`winrt-Microsoft.UI.Xaml==4!2.4.0`, and as the major of every hand-written
package's version. They all change on one event: the one where every projection
package has to be republished. `scripts/versions.py` checks them against each
other.

The table format is the only contract between the runtime and a projection
package. A projection package contains no compiled code, and the interop
packages that do reach the runtime through Python functions, so there is no C
ABI version beside it.

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

That bound catches a table that asks for more than the runtime has. It does not
catch a table whose ids were assigned by a *different* census, which is what a
generation run against an empty directory produces: the ids start at zero again
in whatever order that run's metadata happened to be walked, the highest of them
is below the runtime's own count, and every call goes through a trampoline for
another signature. So the census carries an identity that the table records and
the runtime checks.

The identity is a **lineage**, a GUID generated once when a census file is
created and never changed afterwards, and a **revision**, an integer that
increments each time ids are appended. The rule the runtime applies is: the same
lineage, and a revision no higher than its own.

It is an identity and not a hash of the contents, because a census that grows is
exactly what the append-only rule is for. A package generated against this
census as it stands has to keep loading against a runtime built from it after
another winmd has been projected into it, and a content hash would refuse that.
The revision is what a lineage alone misses: someone who starts from this census
and lets their own run append to it keeps the lineage, so only the revision says
that the table names ids the runtime has no trampoline for.

`shapes-generated.h` carries the same pair as `census_lineage` and
`census_revision`, so a runtime is built with the identity of the census its
trampolines came from.

A table records the revision it **needs**, which is the oldest one that has
every id it names, rather than the one the census stood at when it was
written. `shapes.json` keeps a `revisions` list of how many ids there were at
the end of each revision, and ids are handed out in order, so which revision
first covered a given id is a lookup. Two things follow. A namespace stays
readable by every runtime that has its ids, instead of being refused by one
that could have run it; and its table stops changing when the census grows for
some other namespace, which is what makes one generator run over every family
settle in one pass however the runs are ordered.

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
census 2e30db26-66c8-4551-85b9-a537376d8f99 1
namespace Windows.Foundation

enum Windows.Foundation.AsyncStatus python_type
    py AsyncStatus
    signature enum(Windows.Foundation.AsyncStatus;i4)
    constant CANCELED 2
    constant COMPLETED 1
    constant ERROR 3
    constant STARTED 0

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
text is written to, the lineage of the shape census the members' shape ids
were assigned by and the oldest revision of it that has them, and the WinRT
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
| `overridable <type>` | one overridable interface, which a Python subclass answers |
| `field <py> <winrt> <code>` | one struct field, with `type=` when the code names a type |
| `constant <py> <value>` | one constant of an enum |
| `<kind> <name>` | a group of members bound to one Python attribute, then the group's flags |

A group's name is a dash when the group is bound to no name of its own, which a
constructor group and the `Invoke` of a delegate are.

A constant carries only the name it is reached by, where a struct field carries
its WinRT name as well: an enum constant is never named on the ABI, so the
spelling the metadata gives it is of no use to anyone reading the table. Its
value is written the way the enum is read - a flags enum in hexadecimal,
because it is a set of bits, and every other enum as a signed decimal - and
both are kept as the thirty-two bits they occupy.

A member's line is its kind, the WinRT name of the method, then `slot=`,
`inputs=`, `outputs=`, `declaring=`, `shape=`, `reverse=` and `role=` - of which
only the first three are always written - then its flags. A parameter's line is
its category, its type code, then `type=` and `name=` where it has them, then
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
| 24 | `u32` | compiler version, a string ref |
| 28 | `u32` | WinRT namespace, a string ref, e.g. `Windows.Foundation` |
| 32 | `u32` | one past the highest forward shape id used |
| 36 | `u32` | one past the highest reverse shape id used |
| 40 | `u32` | census lineage, a string ref |
| 44 | `u32` | census revision |

The compiler version is the version of `winrt-table-compiler` that wrote the
file, which is the one thing here that is not a translation of something the
text said. It is the version of the tool that produced the artifact, so a bug
report about an installed package names something that was true of that build
rather than of whatever happened to be committed.

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
| `CNST` | enum constant records |

### References

- A **string ref** is a `u32` byte offset into `STRS`. The blob begins with a
  NUL, so offset zero is the empty string. Strings are UTF-8 and NUL terminated.
- A **type ref** is a `u32` index into `TYPE`, or `0xFFFFFFFF` for none.
- A **list** is a `u32` index into `REFS` together with a `u32` count. The
  elements are type refs unless the field says otherwise. An empty list has a
  count of zero and an ignored index.
- A **guid ref** is a `u32` index into `GUID`, or `0xFFFFFFFF` for none.

### Type record, 96 bytes

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
| 10, 11 | type arguments, a list: what a parameterized type's own parameters are, or what a concrete instance of one fills them in with |
| 12, 13 | member groups, a first index into `GRUP` and a count |
| 14, 15 | struct fields, a first index into `FLDS` and a count |
| 16, 17 | activation factory interfaces, a list |
| 18, 19 | composable factory interfaces, a list |
| 20, 21 | overridable interfaces, a list: the ones a class derived from this one implements rather than calls |
| 22, 23 | enum constants, a first index into `CNST` and a count |

The records of the types the namespace defines come first, sorted by WinRT name,
so that a lookup by name is a binary search. The external references follow in
the order they were first needed.

A **concrete parameterized type** - `IVector<String>` - carries a full set of
member groups of its own, which are the members of the parameterized type it is
an instance of with every type argument filled in. It has to: what a member of
`IVector<String>` passes is not what the same member of `IVector<Point>` passes,
so the two have different type codes and different call shapes, and nothing but
the instance knows which. Its base type is the parameterized type it is an
instance of, whose Python type the runtime derives the instance's from, and its
Python name is fully qualified, because the type belongs to the module that
defines the parameterized type rather than to whichever module first names the
instance. Every namespace that passes one carries a record for it; the runtime
builds one Python type per instance, keyed by the signature.

Type flags, and the words the text writes them as:

| bit | name | meaning |
|---|---|---|
| 0-2 | | category: 0=`enum`, 1=`struct`, 2=`interface`, 3=`class`, 4=`delegate`, which is the first word of a type's line |
| 3 | `external` | the type belongs to another namespace and is resolved by name through the type registry |
| 4 | `static` | static class |
| 5 | `composable` | composable class |
| 6 | `parameterized` | the type still takes type arguments, which is the definition itself, a type argument standing in for one, and an instance that names either |
| 7 | `concrete` | every type argument is a real type, so the record has an IID and a signature of its own |
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
| 21 | `integer` | a struct that is one integer, projected as a subclass of `int` |
| 22 | `flags_enum` | an enum that is a set of bits, projected as an `enum.IntFlag` |

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
| 4 | forward shape id, or `0xFFFFFFFF` when the member has none |
| 5 | first parameter, an index into `PARM` |
| 6 | parameter count, including the return value |
| 7 | number of Python input arguments |
| 8 | number of Python output values |
| 9 | reverse shape id, or `0xFFFFFFFF` when nothing implements the member |

A member with no forward shape cannot be called, and one with no reverse
shape cannot be answered from Python. Two different things leave a member
that way. A member of a **parameterized definition** has no ABI until its
type arguments are known - `IVector<T>.Append` passes a `T` - so the
definition's record carries none and the record of each concrete instance
carries its own. Otherwise the table was generated against a **census that
does not have the shape**, which only a third-party projection can be,
since the census is taken over everything this tree projects. The generator
warns in that case rather than refusing, so the rest of the namespace is
still projected; a reader tells the two apart by the `parameterized` flag on
the declaring type, and raises either way when the member is used.

Member flags:

| bit | name | meaning |
|---|---|---|
| 0-2 | | kind: 0=`method`, 1=`get` (property), 2=`put` (property), 3=`add` (event), 4=`remove` (event), 5=`ctor`, which is the first word of a member's line |
| 3 | `static` | |
| 4 | `overridable` | |
| 5 | `protected` | |
| 6 | `deprecated` | |
| 7 | `default_overload` | |
| 8-12 | `role=` | protocol role, see below |

The protocol role says which Python operation, if any, this member is the one
behind. It is read in both directions: `len(v)` on a projected collection calls
the member whose role is `size`, and a Python list handed to WinRT where an
`IVector<T>` is expected answers that interface's `Size` slot from the same
role. The second direction is why roles 21 to 29 are here at all - nothing on
the Python side of a projected collection calls `GetView` or `IndexOf`, but the
vtable of a collection the runtime assembles over a Python object has a slot
for each of them and has to know what to put there.

| value | role | declared by |
|---|---|---|
| 0 | `none` | |
| 1 | `size` | `IVector<T>`, `IVectorView<T>`, `IMap<K, V>`, `IMapView<K, V>` |
| 2 | `get_at` | `IVector<T>`, `IVectorView<T>` |
| 3 | `set_at` | `IVector<T>` |
| 4 | `remove_at` | `IVector<T>` |
| 5 | `insert_at` | `IVector<T>` |
| 6 | `first` | `IIterable<T>` |
| 7 | `current` | `IIterator<T>` |
| 8 | `has_current` | `IIterator<T>` |
| 9 | `move_next` | `IIterator<T>` |
| 10 | `lookup` | `IMap<K, V>`, `IMapView<K, V>` |
| 11 | `has_key` | `IMap<K, V>`, `IMapView<K, V>` |
| 12 | `insert` | `IMap<K, V>` |
| 13 | `remove` | `IMap<K, V>` |
| 14 | `status` | `IAsyncInfo` |
| 15 | `completed` | the four async interfaces |
| 16 | `get_results` | the four async interfaces |
| 17 | `to_string` | `IStringable` |
| 18 | `value` | `IReference<T>` |
| 19 | `close` | `IClosable` |
| 20 | `get_many` | `IVector<T>`, `IVectorView<T>`, `IIterator<T>` |
| 21 | `get_view` | `IVector<T>`, `IMap<K, V>` |
| 22 | `index_of` | `IVector<T>`, `IVectorView<T>` |
| 23 | `append` | `IVector<T>` |
| 24 | `remove_at_end` | `IVector<T>` |
| 25 | `clear` | `IVector<T>`, `IMap<K, V>` |
| 26 | `replace_all` | `IVector<T>` |
| 27 | `split` | `IMapView<K, V>` |
| 28 | `pair_key` | `IKeyValuePair<K, V>` |
| 29 | `pair_value` | `IKeyValuePair<K, V>` |

The type flags say which protocols a type implements and the roles say which
members those protocols call, so the two are written together. A name would
not do instead: a runtime class is free to have a `Size` property or a `Remove`
method of its own, and what makes a member the one behind `len()` is the
interface that declares it rather than what it is called. The role is on the
member and not on the group, because a group is one Python attribute and may
hold overloads reached through more than one interface.

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

The three array categories are one parameter record and two ABI arguments, a
count and a pointer to the elements, and they differ in who owns those
elements. A `pass_array` lends the callee elements to read; a `fill_array`
lends it elements to write, so what it writes has to reach the caller's own
memory rather than a copy of it; a `receive_array` is an output, and its two
arguments are pointers the callee stores a count and a freshly allocated
buffer through, which the caller then frees. The type code names the element,
not the array.

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

### Enum constant record, 8 bytes

| index | field |
|---|---|
| 0 | Python name, a string ref |
| 1 | the constant, as the thirty-two bits it occupies |

The constants of an enum are the whole of what the table says about it - an
enum has no interface, no IID and no members - and a reader builds an
`enum.IntEnum` of them, or an `enum.IntFlag` where the `flags_enum` bit says
so. What the bits mean is the enum's to say rather than the constant's: a WinRT
enum is a signed `int32` unless it carries `FlagsAttribute`, in which case it
is an unsigned `uint32`, so the same record serves both and the type's flags
decide. The order the constants are written in is the order they are declared
in, which is what settles which of two constants with one value is the name and
which is the alias.

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

An instance of a parameterized delegate - `TypedEventHandler<S, R>` - is code
25 and not 26, because what a caller passes where one is expected is a
callable and not an interface. Its type arguments change the signature of its
`Invoke` and nothing else, and the record for the instance is what says what
that signature is.

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

Two structs are not laid out at all. `Windows.Foundation.HResult` and
`Windows.Foundation.EventRegistrationToken` hold one integer each and are
projected as subclasses of `int`, so their records carry the `integer` flag and
a reader builds no wrapper for them. Their codes - `hresult` and `event_token`
- are the integer on the ABI and name the type the same way `enum32` does, so a
value of one is built by calling that type with the integer.

## Validation

A reader checks, before it trusts anything:

- the magic, the format major, and that the format minor is not ahead of its own;
- that the recorded file size is the size of the buffer;
- that every section lies inside the buffer, is eight byte aligned and, for the
  record sections, has a size that is a whole number of records;
- that the census lineage is the one `shapes-generated.h` was written from and
  that the census revision is not ahead of its own;
- that both shape id limits are within the arrays in `shapes-generated.h`;
- that the string blob begins with a NUL and ends with a NUL.

Everything else - a type ref, a list, a string ref - is bounds checked where it
is read, and an out of range reference raises rather than reading past the end.
The table is generated, so none of these are expected to fire; they are there so
that a mismatched pair of packages says what is wrong instead of crashing.

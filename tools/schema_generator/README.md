# Schema Generator

This Python utility parses Google API Discovery documents which describe
particular APIs, and generates C++ parsers and serializers for the
schemas defined there.

The tool has been written initially to generate C++ code for schemas for the
People API (which contains over 40 schemas that would otherwise have to be
written out by hand, including serializers and parsers), so it may contain
logic that is specific to this API. It's perfectly fine to tweak this tool
to support other APIs.

This tool is not currently intended to generate C++ code at build-time,
instead, its intended to generate classes, properties, getters and setters
for all the schemas (which is otherwise tedious and error-prone work), but
there's no guarantee that the generated code will compile - usually it will
need some manual editing afterwards that would otherwise be too complex or
unfeasible to implement into this tool in some generic way (or I'm just too
lazy to do that).

However, the long-term plan for this tool is to generate C++ code for all
the supported APIs at compile time based on the Discovery documents, but
that's a distant future...

## API definition file

The API definition file is a custom YAML file that describes the API
discovery document and some additional configuration for the tool.
The file looks like this:

```yaml
# Name of the API
name: PeopleV1

# C++ namespace (will be automatically prefixed with KGAPI2::)
namespace: People

# Name of the C++ export macro
export_macro: KGAPIPEOPLE_EXPORT

# URL of the API's discovery document
discovery_uri: https://people.googleapis.com/$discovery/rest?version=v1

# URL of the API's reference documentation
reference_base_uri: https://developers.google.com/people/api/rest/v1/people

# The version of LibKGAPI where the API first appeared, used in @since doxygen tags.
since: "5.19.0"

# List of classes that should inherit KGAPI2::Object
kgapiobjects:
    - Person

# List of schema names that should be ignored. The name can contain an
# wildcard symbol (*) at the beginning or end to exclude all schemas ending
# or starting with the specified name.
excludeObjects:
    - "*Request"
    - "*Response"
    - "ContactToCreate"
    - "SearchResult"
    - "Status"
    - "Empty"
    - "Date"

# List of OldName: NewName pairs. The generator will rename any object or reference
# with the old name to the new name. Can be used to avoid conflicts with C++/Qt/system
# names.
renameObjects:
    Locale: PersonLocale
```

#!python3
from typing import List
import requests
import textwrap
import itertools

#from property import Property
from schema_types import *

def generate_schema_code(schema, cppfile, hppfile, kobject=True):
    properties = list(map(lambda p: Property(*p), schema['properties'].items()))
    with open(hppfile, 'w') as header, open(cppfile, 'w') as source:
        header.write(textwrap.dedent("""\
            #pragma once

            // This is a generated file. Do not edit!
        """))
        if kobject:
            header.write("#include \"object.h\"\n")
        else:
            header.write('#include <QSharedDataPointer>\n')

        includes = set(itertools.chain.from_iterable(map(lambda p: p.includes, properties)))
        for include in includes:
            header.write(f"#include {include}\n")

        header.write("\nnamespace KGAPI2::People {\n\n")
        classname = schema['id'][0].upper() + schema['id'][1:]
        if kobject:
            header.write(textwrap.dedent(f"""\
                class {classname} : public KGAPI2::Object {{
                    Q_OBJECT
            """))
        else:
            header.write(textwrap.dedent(f"""\
                class {classname} {{
                    Q_GADGET
            """))

        for prop in properties:
            header.write(f"    /** {prop.description} **/\n")
            header.write(f"    {prop.qproperty_string(notify=kobject)}\n")

        # First, write nested types
        header.write(textwrap.dedent(f"""\
            public:
                explicit {schema['id']}();
                ~{schema['id']}();
        """))

        for prop in properties:
            header.write("\n")
            header.write(f"    {prop.type} {prop.getter}() const;\n")
            if not prop.read_only:
                header.write(f"    void {prop.setter}(const {prop.type} &{prop.name});\n")
            if prop.is_array:
                var_name = f"{prop.array_type[0].lower()}{prop.array_type[1:]}"
                header.write(f"    void add{prop.array_type}(const {prop.array_type} &{var_name});\n")
                header.write(f"    void remove{prop.array_type}(const {prop.array_type} &{var_name});\n")
                header.write(f"    void clear{prop.name[0].upper()}{prop.name[1:]}();\n")

        ###### Privaet part
        header.write(textwrap.dedent(f"""\
            private:
                class Private;
        """))

        if kobject:
            header.write('    Private const *d;\n')
        else:
            header.write('    const QSharedDataPointer<Private> d;\n')

        header.write(textwrap.dedent(f"""\
            }}; // class {schema['id']}

            }} // namespace KGAPI2::People
        """))



def parse_enums(schemas: dict):
    def find_enum(subtree):
        for name, value in subtree.items():
            if name == 'properties':
                for type, type_def in value.items():
                    if 'enum' in type_def:
                        values = type_def['enum']
                        # v contains enum, p is the name of the prop
                # look for enum props now
            elif isinstance(value, dict):
                find_enum(value)


def _parse_object_schema(schema):
    o = Object()
    
def _parse_schema_type(schema):
    if schema['type'] == 'object':
        return Object(schema)
    else:
        print(f"Unknown schema type {schema['type']}")
        

def parse_schema(uri):
    disc_req = requests.get(uri)
    disc_doc = disc_req.json()

    results = []
    for name, schema in disc_doc['schemas'].items():
        # Skip schemas representing API call requests/respones
        if name.endswith('Request') or name.endswith('Response'):
            continue

        results.append(_parse_schema_type(schema))

    return results
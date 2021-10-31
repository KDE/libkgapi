# SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
#
# SPDX-License-Identifier: LGPL-2.1-only
# SPDX-License-Identifier: LGPL-3.0-only
# SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL

from typing import Dict, List, Optional, cast

class Type:
    """
    Base class for all representation of schema types.
    """

    _is_array = False
    _is_ref = False
    _is_object = False
    _is_enum = False
    _is_builtin = False

    def __init__(self, name):
        self._name = name

    def __lt__(self, other: "Type") -> bool:
        # pylint: disable=no-member
        if (isinstance(self, BuiltinType) and cast(BuiltinType, self).is_qt_type) and (
            not isinstance(other, BuiltinType)
            or not cast(BuiltinType, other).is_qt_type
        ):
            return True
        if isinstance(self, BuiltinType) and not isinstance(other, BuiltinType):
            return True
        return self._name < other._name

    def rename_to(self, new_name: str) -> None:
        self._name = new_name

    @classmethod
    @property
    def is_array(cls) -> bool:
        return cls._is_array

    @classmethod
    @property
    def is_ref(cls) -> bool:
        return cls._is_ref

    @classmethod
    @property
    def is_object(cls) -> bool:
        return cls._is_object

    @classmethod
    @property
    def is_enum(cls) -> bool:
        return cls._is_enum

    @classmethod
    @property
    def is_builtin(cls) -> bool:
        return cls._is_builtin

    @staticmethod
    def _map_to_builtin(name: str, orig_ref: "Ref") -> "Optional[Type]":
        if name == "Date":
            return QDate(orig_ref=orig_ref)
        return None

    @staticmethod
    def parse(
        type_schema: Dict, name: str, schema_name: str, ref_visitor: "RefVisitor"
    ):
        # pylint: disable=too-many-return-statements
        if "enum" in type_schema:
            return Enum(name, schema_name, type_schema)
        if "type" in type_schema:
            prop_type = type_schema["type"]
            if prop_type == "array":
                return QVector(type_schema["items"], name, schema_name, ref_visitor)
            if prop_type == "boolean":
                return Boolean()
            if prop_type == "integer":
                return Integer()
            if prop_type == "object":
                if "additionalProperties" in type_schema:
                    return QVariantMap()
                return Object(type_schema, ref_visitor)
            if prop_type == "string":
                return QString()
            raise RuntimeError(f'Unknown type "{prop_type}"')
        if "$ref" in type_schema:
            builtin = Type._map_to_builtin(
                type_schema["$ref"], Ref(type_schema["$ref"], ref_visitor)
            )
            return builtin if builtin else Ref(type_schema["$ref"], ref_visitor)

        raise RuntimeError(f"Missing type information in schema {type_schema}.")

    @property
    def name(self) -> str:
        return self._name

    @property
    def full_name(self) -> str:
        return self.name


class Property:
    """
    Represents a single property of an Object type.

    A property has a name, and a type, described by instance of a Type subclass.
    """

    def __init__(
        self, name: str, prop: Dict, schema_name: str, ref_visitor: "RefVisitor"
    ):
        self._prop = prop
        self._name = name
        self._orig_name = name
        self._description = prop["description"]
        self._read_only = prop.get("readOnly", False)
        self._type = Type.parse(self._prop, self._name, schema_name, ref_visitor)
        # Avoid some C++ reserved words
        if self._name == "default":
            self._name = "isDefault"

    def __repr__(self) -> str:
        return f"{self._name}({self._type.name})"

    @property
    def name(self) -> str:
        return self._name

    @property
    def capitalized_name(self) -> str:
        return self._name[0].upper() + self._name[1:]

    @property
    def description(self) -> str:
        return self._description

    @property
    def type(self):
        return self._type

    @property
    def read_only(self) -> bool:
        return self._read_only

    @property
    def orig_name(self) -> str:
        return self._orig_name


class Object(Type):
    """
    Maps to the "object" type as it appears in the schema documents.
    An Object type is composed of multiple properties.
    """

    _is_object = True

    def __init__(self, schema: dict, ref_visitor: "RefVisitor"):
        super().__init__(schema["id"])
        assert schema["type"] == "object"
        self._description: str = schema["description"]
        self._properties = list(
            map(
                lambda tpl: Property(
                    tpl[0], tpl[1], schema_name=self._name, ref_visitor=ref_visitor
                ),
                schema["properties"].items(),
            )
        )

    def __repr__(self) -> str:
        return f"<{self._name} {{ {','.join(map(str, self._properties))} }}>"

    @property
    def description(self) -> str:
        return self._description

    @property
    def properties(self) -> List[Property]:
        return self._properties

    @property
    def nested_enums(self) -> "List[Enum]":
        enums = []
        for prop in self._properties:
            if isinstance(prop.type, Enum):
                enums.append(prop.type)
            elif prop.type.is_array and isinstance(prop.type.element_type, Enum):
                enums.append(prop.type.element_type)
        return enums

    @property
    def dependencies(self) -> List[Type]:
        deps = []
        known = set()

        def add_dep(dep_type) -> None:
            if dep_type.name not in known:
                known.add(dep_type.name)
                deps.append(dep_type)

        for prop in self._properties:
            if prop.type.is_array:
                add_dep(prop.type)
                add_dep(prop.type.element_type)
            elif prop.type.is_ref or (prop.type.is_builtin and prop.type.is_qt_type):
                add_dep(prop.type)
        return sorted(deps)

    @property
    def hhhhas_primary(self) -> bool:
        for prop in self._properties:
            if prop.name == "primary":
                return True
        return False


class EnumValue:
    """
    A single value of an enumerator.
    """

    def __init__(self, value, name, description):
        self._value = value
        self._name = name
        self._description = description

    @property
    def value(self) -> str:
        return self._value

    @property
    def name(self) -> str:
        return self._name

    @property
    def description(self) -> str:
        return self._description


class Enum(Type):
    """
    Represents an enum in a property.
    """

    is_enum = True

    def __init__(self, name: str, schema_name: str, schema: Dict):
        super().__init__(name[0].upper() + name[1:])
        self._schema_name = schema_name
        self._values = []
        for i, value_name in enumerate(schema["enum"]):
            self._values.append(EnumValue(i, value_name, schema["enumDescriptions"][i]))

    @property
    def values(self) -> List[EnumValue]:
        return self._values

    @property
    def full_name(self) -> str:
        return f"{self._schema_name}::{self._name}"



class Ref(Type):
    """
    Ref is a reference to a type (usually an Object) that is defined elsewhere
    in the schema.

    The referenced type can be accessed through ref_type property.
    """

    _is_ref = True

    def __init__(self, name: str, ref_visitor: "RefVisitor"):
        super().__init__(name)
        self._ref_type = None
        ref_visitor.add_ref(self)

    @property
    def ref_type(self) -> Optional[Type]:
        return self._ref_type

    @property
    def has_primary(self) -> bool:
        return self._ref_type is not None and self._ref_type.has_primary


class BuiltinType(Type):
    """
    Base class for builtin types

    Builtin types are types that are built in the C++ language (int, bool)
    and, for the purpose of the schema_generator, also any known Qt classes.
    """

    _is_builtin: bool = True
    _include_name: str
    _is_qt_type: bool = False

    def __init__(self, name, orig_ref: Ref = None):
        super().__init__(name)
        self._orig_ref = orig_ref

    @classmethod
    @property
    def include_name(cls) -> str:
        return cls._include_name

    @classmethod
    @property
    def is_qt_type(cls) -> bool:
        return cls._is_qt_type

    @property
    def has_primary(self) -> bool:
        return False

    @property
    def orig_ref(self):
        return self._orig_ref


class Boolean(BuiltinType):
    """Represents a C++ bool type."""

    def __init__(self):
        super().__init__("bool")


class Integer(BuiltinType):
    """Represents a C++ int type."""

    def __init__(self):
        super().__init__("int")


class QVector(BuiltinType):
    """
    Represents a QVector class.

    The element_type property holds the type of the elements in the array.
    """

    _include_name = "QVector"
    _is_qt_type = True
    _is_array = True

    def __init__(
        self,
        element_type: Dict,
        name_hint: str,
        schema_name: str,
        ref_visitor: "RefVisitor",
    ):
        super().__init__("QVector")
        self._element_type = Type.parse(
            element_type, name_hint, schema_name, ref_visitor
        )

    @property
    def element_type(self) -> Type:
        return self._element_type

    @property
    def full_name(self) -> str:
        return f"QVector<{self._element_type.full_name}>"


class QVariantMap(BuiltinType):
    """
    Represents a QVariantMap class.
    """

    _include_name = "QVariantMap"
    _is_qt_type = True

    def __init__(self):
        super().__init__("QVariantMap")


class QDate(BuiltinType):
    """
    Represents a QDate class.
    """

    _include_name = "QDate"
    _is_qt_type = True

    def __init__(self, orig_ref: Ref = None):
        super().__init__("QDate", orig_ref)


class QString(BuiltinType):
    """
    Represents a QString class.

    This maps a plain string (in the schema) to QString.
    """

    _include_name = "QString"
    _is_qt_type = True

    def __init__(self, orig_ref: Ref = None):
        super().__init__("QString", orig_ref)

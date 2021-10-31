from .type import Type
from .builtins import QVector, QString, Integer, Boolean
from .enum import Enum
from .ref import Ref

class Property:
    def __init__(self, name, prop):
        self._prop = prop
        self._name = name
        self._description = prop['description']
        self._type = None
        self._read_only = prop.get('readOnly', False)

        self._parse_type()

    def __repr__(self) -> str:
        return f'{self._name}({self._type.name})'

    @property
    def name(self) -> str:
        return self._name

    @property
    def description(self) -> str:
        return self._description

    @property
    def type(self):
        return self._type

    @property
    def read_only(self) -> bool:
        return self._read_only
        
    def _parse_type(self):
        if 'enum' in self._prop:
            self._type = Enum(self._name, self._prop)
        elif 'type' in self._prop:
            prop_type = self._prop['type']
            if prop_type == 'array':
                self._type = QVector(self._prop['items'])
            elif prop_type == 'boolean':
                self._type = Boolean()
            elif prop_type == 'integer':
                self._type = Integer()
            elif prop_type == 'object':
                self._type = Object(self._prop)
            elif prop_type == 'string':
                self._type = QString()
            else:
                raise RuntimeError(f'Unknown type "{prop_type}"')
        elif '$ref' in self._prop:
            self._type = Ref(self._prop['$ref'])
        else:
            raise RuntimeError(f'Property {self.name} is missing type information.')

    def qproperty_string(self, notify):
        prop = f'Q_PROPERTY({self.type} {self.name} READ {self.getter} '
        if self.read_only:
            prop += 'CONSTANT'
        else:
            prop += f'WRITE {self.setter}'
            if notify:
                prop += f' NOTIFY {self.notify}'
        prop += ')'

        return prop


class Object(Type):
    _is_object = True

    def __init__(self, schema: dict):
        assert schema['type'] == 'object'
        self._name: str = schema['id']
        self._description: str = schema['description']
        self._nested_objects = set()
        self._nested_enums = set()
        self._dependencies = set()
        self._properties = []
        
        self._parse_properties(schema['properties'])

    def __repr__(self):
        return f'<{self._name} {{ {",".join(map(lambda p: str(p), self._properties))} }}>'

    @property
    def name(self) -> str:
        return self._name

    @property
    def description(self) -> str:
        return self._description

    def _parse_properties(self, properties_schema: dict):
        self._properties = list(map(Property, properties_schema.items()))
           
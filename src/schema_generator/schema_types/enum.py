from typing import List
from .type import Type


class EnumValue:
    """ Represents an enum value """

    def __init__(self, value, name, description):
        self._value = value
        self._name = name
        self._description = description

    @property
    def name(self) -> str:
        return self._name

    @property
    def description(self) -> str:
        return self._description

class Enum(Type):
    """ Represents an enum type """

    def __init__(self, name, schema):
        self._name = name
        self._values = []
        for i, value_name in enumerate(schema['enum']):
            self._values.append(EnumValue(i, value_name, schema['enumDescriptions'][i]))

    @property
    def name(self) -> str:
        return self._name

    @property
    def values(self) -> List[EnumValue]:
        return self._values

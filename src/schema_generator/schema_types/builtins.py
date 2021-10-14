from .type import Type

class BuiltinType(Type):
    _is_builtin = True
    _name = None
    _include_name = None
    _is_qt_type = False

    @classmethod
    @property
    def name(cls):
        return cls._name

    @classmethod
    @property
    def include_name(cls):
        return cls._include_name

    @classmethod
    @property
    def is_qt_type(cls):
        return cls._is_qt_type

class Boolean(BuiltinType):
    _name = "bool"
    _include_name = None

class Integer(BuiltinType):
    _name = "int"
    _include_name = None

class QVector(BuiltinType):
    _name = "QVector"
    _include_name = "QVector"
    _is_qt_type = True
    _is_array = True

    def __init__(self, nested_type):
        self._nested_type = nested_type

    @property
    def nested_type(self):
        return self._nested_type

class QDate(BuiltinType):
    _name = "QDate"
    _include_name = "QDate"
    _is_qt_type = True

class QString(BuiltinType):
    _name = "QString"
    _include_name = "QString"
    _is_qt_type = True
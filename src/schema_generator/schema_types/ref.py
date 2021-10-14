from .type import Type

class Ref(Type):
    _is_ref = True

    def __init__(self, name):
        self._name = name

    @property
    def name(self):
        return self._name
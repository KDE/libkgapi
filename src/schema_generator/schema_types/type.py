class Type:
    _is_array = False
    _is_ref = False
    _is_object = False
    _is_enum = False
    _is_builtin = False

    @classmethod
    @property
    def is_array(cls):
        return cls._is_array

    @classmethod
    @property
    def is_ref(cls):
        return cls._is_ref

    @classmethod
    @property
    def is_object(cls):
        return cls._is_object

    @classmethod
    @property
    def is_enum(cls):
        return cls._is_enum

    @classmethod
    @property
    def is_builtin(cls):
        return cls._is_builtin
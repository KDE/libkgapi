# SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
#
# SPDX-License-Identifier: LGPL-2.1-only
# SPDX-License-Identifier: LGPL-3.0-only
# SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL

from typing import Dict, List
from functools import partial

from schema_types import Object, Ref, Type


class ParserError(Exception):
    """
    An exception that is raised when the schema cannot be parsed.
    """


class RefVisitor:
    """
    RefVisitor is a helper class for binding Refs with referenced types.

    A Ref type is a reference to another type (usually Object) defined elsewhere
    in the schema. Each Ref type is registered into the RefVisitor. Once the entire
    document is parsed, the RefVisitor is called to update all Refs with links to
    definitions of the actual type.
    """

    def __init__(self, api):
        self._refs: List[Ref] = []
        self._api = api

    def add_ref(self, ref: "Ref") -> None:
        self._refs.append(ref)

    def _update_ref_types(self, schemas: List[Type]) -> None:
        def match_ref(ref: Ref, schema: Object):
            return ref.name == schema.name

        for ref in self._refs:
            # pylint: disable=protected-access # friend access
            ref._ref_type = next(filter(partial(match_ref, ref), schemas), None)  # type: ignore
            if not ref._ref_type:
                raise RuntimeError(f"Failed to find type for ref {ref.name}.")


    def _handle_renamed_objects(self, schemas: List[Type]) -> None:
        for old_name, new_name in self._api["renameObjects"].items():
            for ref in self._refs:
                if ref.name == old_name:
                    ref.rename_to(new_name)

    def update_refs(self, schemas: List[Type]) -> None:
        self._handle_renamed_objects(schemas)
        self._update_ref_types(schemas)


class Parser:
    """
    Parser for the schema described in the Discovery document.
    """

    def __init__(self, api: dict, schema: dict):
        self._api = api
        self._schema = schema

    def _handle_renamed_objects(self, obj: Object):
        for old_name, new_name in self._api["renameObjects"].items():
            if obj.name == old_name:
                obj.rename_to(new_name)

    def _parse_schema_type(self, schema_name: str, schema: Dict, ref_visitor):
        if schema["type"] == "object":
            obj = Object(schema, ref_visitor)
            self._handle_renamed_objects(obj)
            return obj

        raise ParserError(
            f"Unknown schema type \"{schema['type']}\" for schema {schema_name}"
        )

    def parse_schema(self) -> List[Object]:
        ref_visitor = RefVisitor(self._api)
        schemas = list(
            map(
                lambda item: self._parse_schema_type(
                    item[0], item[1], ref_visitor=ref_visitor
                ),
                self._schema["schemas"].items(),
            )
        )
        ref_visitor.update_refs(schemas)

        return schemas

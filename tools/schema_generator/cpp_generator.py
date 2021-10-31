# SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
#
# SPDX-License-Identifier: LGPL-2.1-only
# SPDX-License-Identifier: LGPL-3.0-only
# SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL

import os
import pathlib
from datetime import datetime
from jinja2 import Environment, FileSystemLoader, select_autoescape

from schema_types import Object


class Generator:
    """
    Generates C++ code for given schema.
    """

    def __init__(self, api, outdir):
        self._env = Environment(
            loader=FileSystemLoader(
                f"{pathlib.Path(__file__).parent.resolve()}/templates/"
            ),
            autoescape=select_autoescape(),
        )
        self._api = api
        self._outdir = outdir
        os.makedirs(outdir, exist_ok=True)

        self._api["date"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S %z")

    def generate_schema(self, schema: Object):
        template_base = "object"
        if schema.name in self._api["kgapiobjects"]:
            template_base = "kgapiobject"

        filename = schema.name.lower()
        for ext in ["h", "cpp"]:
            with open(
                os.path.join(self._outdir, f"{filename}.{ext}"), "w"
            ) as output_file:
                template = self._env.get_template(f"{template_base}.{ext}")
                output_file.write(template.render(api=self._api, schema=schema))

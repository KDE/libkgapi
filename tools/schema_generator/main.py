# SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
#
# SPDX-License-Identifier: LGPL-2.1-only
# SPDX-License-Identifier: LGPL-3.0-only
# SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL

import argparse
import requests
import yaml

from schema_parser import Parser
from cpp_generator import Generator


def _skip_schema(name: str, api: dict) -> bool:
    for excl in api.get("excludeObjects", []):
        if excl.startswith("*") and name.endswith(excl[1:]):
            return True
        if excl.endswith("*") and name.startswith(excl[:-1]):
            return True
        if excl == name:
            return True
    return False


def main():
    argparser = argparse.ArgumentParser(add_help=True)
    argparser.add_argument(
        "--apidef", metavar="FILE", help="KGAPI API Definition file", required=True
    )
    argparser.add_argument(
        "--outdir", metavar="DIR", help="Output directory", default="."
    )
    argparser.add_argument(
        "--print-schemas",
        action="store_true",
        help='Discover and list schemas to be generated. Doesn"t generate any code',
    )
    args = argparser.parse_args()

    with open(args.apidef) as apidef_file:
        api = yaml.load(apidef_file, Loader=yaml.FullLoader)

    disc_req = requests.get(api["discovery_uri"])
    schema = disc_req.json()

    parser = Parser(api, schema)
    schemas = parser.parse_schema()
    filtered_schemas = filter(lambda s: not _skip_schema(s.name, api), schemas)

    if args.print_schemas:
        for schema in filtered_schemas:
            print(schema.name)
    else:
        generator = Generator(api, args.outdir)
        for schema in filtered_schemas:
            generator.generate_schema(schema)


if __name__ == "__main__":
    main()

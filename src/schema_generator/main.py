import argparse
from parser import parse_schema

def main():
    argparser = argparse.ArgumentParser(add_help=True)
    argparser.add_argument('--discovery', metavar='FILE_OR_URL', help='File or URL of the Discovery Document', required=True)
    argparser.add_argument('--outdir', metavar='DIR', help='Output directory', default='.')
    args = argparser.parse_args()


    schema = parse_schema(args.discovery)

    print(schema)

if __name__ == "__main__":
    main()

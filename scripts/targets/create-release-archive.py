#!/usr/bin/env python3

import argparse
from os import getcwd
from ntpath import basename
from zipfile import ZipFile
import json


def parse_args():
    parser = argparse.ArgumentParser(description='Create a firmware zip file with a manifest.')
    parser.add_argument('target', type=str)
    parser.add_argument('partitions', metavar='N', type=str, nargs='+',
                        help='List of partitions in the format <offset> <path>')
    return parser.parse_args()


def main():
    args = parse_args()

    n = 2
    partitions_arg = args.partitions
    partitions = [partitions_arg[i * n:(i + 1) * n] for i in range((len(partitions_arg) + n - 1) // n)]

    path = args.target

    with ZipFile(path, 'w') as archive:
        print(f'Creating "{archive.filename}"', end='\n')
        parts = []

        for offset, path in partitions:
            filename = basename(path)
            archive.write(path, filename)
            partition = {
                'path': filename,
                'offset': int(offset, 16),
            }
            parts.append(partition)

        manifest = {
            'chipFamily': 'ESP32',
            'parts': parts,
        }
        archive.writestr('manifest.json', json.dumps(manifest))


if __name__ == '__main__':
    main()

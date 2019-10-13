#!/usr/bin/env python3

# Requires at least python3.7

import sys
import argparse
import subprocess

def test(executable, arg, out):
    instance = subprocess.run([executable, arg], capture_output=True)

    if instance.returncode:
        if out.strip() == 'error':
            return 1
        else:
            print('TEST {:s} "{:s}" FAILED: Expected output {:s} got error'\
                  .format(executable, arg, out.strip()))
            return 0
    else:
        if out.strip() == 'error':
            print('TEST {:s} "{:s}" FAILED: Expected error got {:s}'\
                  .format(executable, arg, instance.stdout.strip().decode()))
            return 0
        elif out.strip() == instance.stdout.strip().decode():
            return 1
        else:
            print('TEST {:s} "{:s}" FAILED: Expected output {:s} got {:s}'\
                  .format(executable, arg, out.strip(), instance.stdout.strip().decode()))
            return 1

def main():
    if len(sys.argv) != 3:
        print('Usage: {:s} executable test_file'.format(sys.argv[0]))
        return

    passed = 0
    total = 0
    with open(sys.argv[2], 'r') as f:
        for line in f:
            inp, out = line.split(',')
            passed += test(sys.argv[1], inp, out)
            total += 1

        print('{:s} passed {:d} out of {:d} tests'.format(sys.argv[1], passed, total))


if __name__ == '__main__':
    main()
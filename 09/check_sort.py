import argparse
import os
import numpy as np

def main():
    parser = argparse.ArgumentParser(description="Checks if binary file of np.uint64 is sorted")
    parser.add_argument('p', help="path to binary file")

    args = parser.parse_args()

    if os.path.exists:
        arr = np.fromfile(args.p, dtype=np.uint64)
        print("Array is sorted" if np.all(arr[:-1] <= arr[1:]) else "Array is not sorted")
        # print(arr)
    else:
        raise ValueError(f"File {args.p} doesn't exist")

if __name__ == '__main__':
    main()
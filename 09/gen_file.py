import argparse
import numpy as np

def main():
    parser = argparse.ArgumentParser(description="Generate binary array of np.uint64")
    parser.add_argument('N', type=int, help="length of an array to generate")
    parser.add_argument('-r', type=int, help="maximum value of an array to generate, max uint64 by default",
                        default=np.iinfo(np.uint64).max)
    parser.add_argument('-p', type=str, help="path to save generated sequence, 'seq' by default",
                        default='seq')

    args = parser.parse_args()

    if args.N <= 0:
        raise ValueError("N must be a positive integer")
    if args.r <= 0:
        raise ValueError("range must be non-negative")

    a = np.random.randint(args.r, size=args.N, dtype=np.uint64)
    # print(a)
    a.tofile(args.p)

if __name__ == '__main__':
    main()
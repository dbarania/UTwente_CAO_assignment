import os
import subprocess
import sys
import time
from typing import Literal
from itertools import product

BlockType = Literal["uint8_t", "uint16_t", "uint32_t", "uint64_t"]

def compile(file, exec_name, block_type:BlockType="uint16_t"):
    gcc_command = [
        "gcc",
        file,
        "-o",
        exec_name,
        f"-DBLOCK_TYPE={block_type}",
        "-O3"
    ]

    result = subprocess.run(gcc_command, capture_output=True, text=True)
    if result.returncode != 0:
        print("Compilation failed with errors:")
        print(result.stderr)
        return
    print("Compilation successful.")

def run(exec:str, length, number, seed):
    run_command = [f"./{exec}", f"-L {length}", f"-N {number}", f"-s {seed}"]
    
    t1 = time.time()
    result = subprocess.run(run_command, capture_output=True, text=True)
    t2 = time.time()
    print(f"L = {number}, N = {length}, s = {seed}")
    print(result.stdout)
    print(f"Program ran for {t2-t1}s\n")


if __name__ =="__main__":
    c_file_name = "main.c"
    work_directory = os.path.dirname(os.path.realpath(__file__))
    
    c_file_path = f"{work_directory}/{c_file_name}"
    exec_name = "hamming_distance"

    compile(c_file_path,exec_name)

    length_to_run = [64, 89, 256]
    number_to_run = [100,1000]
    seed = 21

    for N,L in product(number_to_run, length_to_run):
        run(exec_name,L,N,seed)


    #TODO Look into where the exec is saved how to change it


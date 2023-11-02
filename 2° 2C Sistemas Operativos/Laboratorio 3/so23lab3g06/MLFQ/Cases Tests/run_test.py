from io import FileIO
from subprocess import Popen, PIPE
import time, sys, json, os
from typing import Union

###################
#### IMPORTANT ####
###################

# Before running execute 'make CPUS=1 qemu' at least once

# HOW TO RUN
#   python run_experiments.py

# Constants
DURATION = 5
COMMANDS = [
    "iobench",
    "cpubench",
    "iobench &\n cpubench &",
    "cpubench &\n cpubench &",
    "iobench &\n cpubench &\n cpubench &",
]

WORKDIR = "../xv6-riscv/"
XV6_CMD = ["make", "CPUS=1", "qemu"]
INITIAL_WAIT = 2

ENABLE_PROCDUMP = False
PROCDUMP_INTERVAL = 15


def skip_xv6_init_messages(stdout: FileIO) -> None:
    """Skips compiling commands and xv6 init messages

    It skips everything until it reaches a line that contains 'starting sh'
    """
    print("Skipping xv6 init messages...")
    while b"starting sh" not in stdout.readline():
        next(stdout)


def clean_output(output):
    return output.decode().replace("$ ", "").strip()


def round_to_multiple(number, multiple):
    return multiple * round(number / multiple)


def get_experiment_data(command: str, duration: Union[int, float]):
    """Executes command inside xv6 and keeps storing output for the specified duration

    :command: string of text containing the command to be executed
    :duration: ammount of time in minutes
    """

    result = {"command": command, "output": {}}

    qemu = Popen(XV6_CMD, stdout=PIPE, stdin=PIPE)
    skip_xv6_init_messages(qemu.stdout)
    time.sleep(INITIAL_WAIT)

    duration_in_secs = duration * 60
    end_time = time.time() + duration_in_secs

    qemu.stdin.write(f"{command}\n".encode())
    qemu.stdin.flush()

    start_time = time.time()
    current_time = time.time()
    prev_procdump_time = 0

    total_procs = 0

    if "iobench" == command:
        total_procs = 1

    elif "cpubench" == command:
        total_procs = 1

    elif "iobench &\n cpubench &" == command:
        total_procs = 2

    elif "cpubench &\n cpubench &" == command:
        total_procs = 2

    elif "iobench &\n cpubench &\n cpubench &" == command:
        total_procs = 3

    finish_procs = 0

    while finish_procs != total_procs:
        line = qemu.stdout.readline()
        if not line:
            break
        output = clean_output(line)
        print(output, end="")
        time_passed = round(current_time - start_time, 2)
        completion_percetage = time_passed * 100 / duration_in_secs
        term_size = os.get_terminal_size()
        print(
            f"{round(completion_percetage, 2)}%".rjust(
                term_size.columns - len(output) - 1
            )
            + " "
        )

        its_time_to_procdump = time_passed >= prev_procdump_time + PROCDUMP_INTERVAL
        if ENABLE_PROCDUMP and its_time_to_procdump:
            qemu.stdout.flush()
            qemu.stdin.write(chr(16).encode())
            qemu.stdin.flush()
            prev_procdump_time = time_passed

        current_time = time.time()

        pid = output.split(":")[0]

        if "OPW" in output:
            destination = result["output"].setdefault(f"iobench{pid}", [])

        elif "MFLOP" in output:
            destination = result["output"].setdefault(f"cpubench{pid}", [])

        else:
            destination = result["output"].setdefault("other", [])

        if "PID" in output:
            finish_procs += 1

        destination.append({"time": time_passed, "content": output})

    print("DONE")
    qemu.terminate()
    qemu.wait()
    print()
    return result


def write_json(data: list[dict], filename: str) -> None:
    """jsonify data and append it to filename

    Adds an empty line at the end when appending data
    """
    with open(filename, "a") as output_file:
        formatted_data = json.dumps(data, indent=4)
        output_file.write(formatted_data)
        output_file.write("\n\n")


def main():
    os.chdir(WORKDIR)
    results = [get_experiment_data(command, DURATION) for command in COMMANDS]
    write_json(results, "test.json")
    print(results)


if __name__ == "__main__":
    main()

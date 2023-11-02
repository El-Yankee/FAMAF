import json, pprint

DATA_DIR = "Data"

QUANTUMS = [
    "ORIGINAL",
    "REDUCE",
]

def generate_filenames():
    return [
        f"{DATA_DIR}/cases_{quantum}_quantum.json"
        for quantum in QUANTUMS
    ]

JSON_FILES = [*generate_filenames()]


def read_experiment_data_from_json(filename: str) -> list[dict]:
    with open(filename, "r") as json_file:
        file_contents = json_file.read()
        parsed_json = json.loads(file_contents)
        return parsed_json


def get_average(output):
    if output:
        values = [int(value["content"].split(" ")[1]) for value in output]
        return round(sum(values) / len(values), 2)


def get_averages(experiment_data, process_name):
    result = []
    for experiment in experiment_data:
        averages = [
            get_average(value)
            for process, value in experiment["output"].items()
            if process_name in process
        ]
        result.append(averages)
    return result


def main(json_file, process):
    data = read_experiment_data_from_json(json_file)
    print(f"{json_file.split('/')[-1]}: {process}")
    for index, case in enumerate(get_averages(data, process)):
        print(f"Case {index + 1}: {case}")
    print()


if __name__ == "__main__":
    for json_file in JSON_FILES:
        main(json_file, "iobench")

    for json_file in JSON_FILES:
        main(json_file, "cpubench")

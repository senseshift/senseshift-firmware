import json
import pandas as pd
import argparse


# Function to load JSON data from files
def load_json(filename):
    with open(filename, 'r') as f:
        return json.load(f)


# Function to compare values and create formatted strings
def compare_values(base, current, key):
    if key not in base and key not in current:
        return None
    if key not in base:
        return f"🆕 {current[key]}"
    if key not in current:
        return f"❌"

    base_value = base[key]
    current_value = current[key]

    if base_value == current_value:
        return f"🆗 {base_value}"

    change = current_value - base_value
    sign = "+" if change > 0 else "-"
    symbol = "⚠️" if change > 0 else "⬇️"

    # round to 2 decimal places
    change = round(abs(change), 2)
    return f"{symbol} {current_value} <sub>{sign}{change}</sub>"


# Function to generate Markdown table
def generate_markdown_table(base_data, current_data):
    table_data = []

    all_keys = set(base_data.keys()).union(set(current_data.keys()))
    all_keys = sorted(all_keys)

    for key in all_keys:
        base_ram = base_data.get(key, {}).get("ram", {})
        current_ram = current_data.get(key, {}).get("ram", {})

        base_flash = base_data.get(key, {}).get("flash", {})
        current_flash = current_data.get(key, {}).get("flash", {})

        row = [
            key.replace("-usage", ""),
            compare_values(base_ram, current_ram, "percentage"),
            compare_values(base_ram, current_ram, "used"),
            compare_values(base_flash, current_flash, "percentage"),
            compare_values(base_flash, current_flash, "used")
        ]
        table_data.append(row)

    df = pd.DataFrame(table_data, columns=["Variant", "RAM, %", "RAM, bytes", "Flash, %", "Flash, bytes"])

    return df.to_markdown(index=False)


# Main function to handle CLI arguments and execute the script
def main():
    parser = argparse.ArgumentParser(description='Compare JSON files and generate a markdown table.')
    parser.add_argument('base', type=str, help='Path to the base JSON file')
    parser.add_argument('current', type=str, help='Path to the current JSON file')
    parser.add_argument('--output', type=str, help='Path to the output markdown file')
    args = parser.parse_args()

    base_data = load_json(args.base)
    current_data = load_json(args.current)

    markdown_table = generate_markdown_table(base_data, current_data)

    if args.output:
        with open(args.output, 'w') as f:
            f.write(markdown_table)
        print(f"Markdown table generated and saved to {args.output}")
    else:
        print(markdown_table)


# usage: python compare-memory-usage.py base.json current.json --output table.md
if __name__ == "__main__":
    main()

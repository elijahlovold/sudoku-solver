#!/bin/python3

from copy import copy
from io import StringIO
import requests
import pandas as pd
from bs4 import BeautifulSoup
from bs4.element import Tag 
import hashlib 

def html_table_to_string(table: Tag) -> str: 
    table_io = StringIO(str(table))

    # Convert the modified HTML table to a DataFrame
    df = pd.read_html(table_io)[0]
    return df.to_string(index=False, header=False)


def generate_short_uuid(input_string):
    # Hash the input string using a hash function (e.g., SHA-256)
    hash_object = hashlib.sha256(input_string.encode())
    hash_hex = hash_object.hexdigest()

    # Return the first 8 characters of the hex digest
    return hash_hex[:8]

# URL of the website containing the table
url = "https://www.sudokuweb.org"

# Fetch the webpage
response = requests.get(url)
if response.status_code != 200:
    print("Failed to retrieve the webpage")
    exit()

# Parse the webpage content
soup = BeautifulSoup(response.text, "html.parser")

# Find the table by ID
table: Tag = soup.find("table")  # You may need to refine this with a class or ID

if table:
    table_solutions = copy(table)
    # replace unknown with -
    for span in table.find_all("span", class_="true"):
        span.string = "-"  # Replace the content inside the <span> with '-'
    
    grid_str = html_table_to_string(table)
    grid_solution_str = html_table_to_string(table_solutions)

    grid_hash = generate_short_uuid(grid_solution_str)

    print("Grid: \n" + grid_str)
    print("\n\nSolution: \n" + grid_solution_str)
    grid_file_name = "grid_" + grid_hash + ".grd"
    with open(grid_file_name, "w") as f: 
        f.write("9\n\n" + grid_str)

    grid_file_name = "grid_" + grid_hash + ".sln.grd"
    with open(grid_file_name, "w") as f: 
        f.write("9\n\n" + grid_solution_str)
else:
    print("No table found.")

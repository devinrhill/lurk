import os

for root, dirs, files in os.walk("."):
    for file in files:
        full = os.path.join(root, file)

        os.system(f"nvim {full}")

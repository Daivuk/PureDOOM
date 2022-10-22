# This scripts generates single header doom.


import glob
import re
import os


files = []
code_files = []
done_files = []
final:str = ""


class File:
    def __init__(self, filename) -> None:
        with open(filename, "r") as file:
            self.content = file.read()
        
        # Find headers
        self.headers = re.findall(r'#include \"(\w+\.h)\"', self.content)

        # Comment out includes
        self.content = re.sub(r'#include', '//#include', self.content)

        # Remove copyrighted block of text at the top (We include copyright at the top of the PureDOOM.h)
        self.content = self.content[self.content.find("#ifndef __"):]

        # Clean up some "logs" at the end of the file
        self.content = re.sub(r'\/\/-----------------------------------------------------------------------------\n\/\/\n\/\/ \$Log\:\$\n\/\/\n\/\/-----------------------------------------------------------------------------', '', self.content)

        self.name = os.path.basename(filename)

    def __repr__(self) -> str:
        return self.name + " -> " + str(self.headers)


def concat_file(filename:str):
    global final
    with open(filename, "r") as file:
        file.read()


# Collect headers
headers = glob.glob(r'../src/DOOM/*.h')
for header in headers:
    if header.endswith("DOOM.h") or header.endswith("d_french.h"):
        continue # We'll do that one manually later, and first
    files.append(File(header))

# Collect code files
codes = glob.glob(r'../src/DOOM/*.c')
for code in codes:
    code_files.append(File(code))

# Start with DOOM.h
with open("../src/DOOM/DOOM.h", "r") as header:
    final += header.read()
print("Concat: DOOM.h")
done_files.append("DOOM.h")

# Concat files as long as there are to concat
while len(files) > 0:
    # Find the first file where it's dependencies have been done
    found = False
    for file in files:
        has_dependencies = False
        for header in file.headers:
            if header not in done_files:
                has_dependencies = True
                break
        if has_dependencies:
            continue # Pick a different file
        found = True
        print("Concat: " + file.name)
        final += file.content
        done_files.append(file.name)
        files.remove(file)
        break
    if not found:
        print("Cycling includes: " + str(files))
        exit(1)

final += "\n#if defined(DOOM_IMPLEMENTATION)\n"

# Append all code files after
for file in code_files:
    print("Concat: " + file.name)
    final += file.content

final += "\n#endif // DOOM_IMPLEMENTATION\n"

# Output single header
with open("../PureDOOM.h", "w") as file:
    file.write(final)

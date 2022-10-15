import sys
import subprocess

nm = subprocess.run(['nm', '-C', sys.argv[1]], check=True, capture_output=True)
grep = subprocess.run(['grep', " INLINE_TESTER"],
                      input=nm.stdout, capture_output=True)

raw_fns = grep.stdout.decode('utf-8').strip().split()

# print(raw_fns)

raw_fns = [rf for idx, rf in enumerate(raw_fns) if idx % 4 == 3]

for idx, f in enumerate(raw_fns):
    raw_fns[idx] = f[0:f.find('<')]

print(raw_fns)

f = open(sys.argv[2], "w")

f.write("#include <iostream>\n\n\n")

for fn in raw_fns:
    f.write(f"template<class T> void {fn}(std::ostream&);\n")

f.write("\n\n")


f.write("int main() {\n")

for fn in raw_fns:
    f.write(f"  {fn}<int>(std::cout);\n")

f.write("\n")

f.write('  std::cout << "All tests passed." << std::endl;\n\n')
f.write("  return 0;\n}")

import os


def countLines(file):
    with open(file) as infile:
        data = infile.read()
    characters = len(data)
    data = data.split("\n")
    data = [x for x in data if x.strip()]
    lines = len(data)
    data = [item for sublist in data for item in sublist]
    words = len(data)
    return lines, words, characters


data = []
alllines = 0
allchars = 0
allwords = 0
print("%-50s %10s %10s    %10s" % ("Filename", "Lines", "Words", "Characters"))
print("-" * 86)
for root, dirs, files in os.walk(os.getcwd(), topdown=False):
    for name in files:
        curFilename = os.path.join(root, name)
        if ('.git' in curFilename): continue
        if ('.idea' in curFilename): continue
        if ('cmake' in curFilename): continue
        if ('linecounter.py' in curFilename): continue
        if (__file__ in curFilename): continue
        lines, words, characters = countLines(curFilename)
        alllines += lines
        allchars += characters
        allwords += words
        curFilename = curFilename.split("emulator\\")[-1]
        print("%-50s %10d %10d    %10d" % (curFilename, lines, words, characters))
print("-" * 86)
print("%-50s %10d %10d    %10d" % ("Total:", alllines, allwords, allchars))

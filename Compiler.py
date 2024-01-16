import sys
import os

MD = sys.argv[1]
HTML = sys.argv[1].replace(".md", ".html")

print("File:" + MD)

if os.path.isfile("./" + HTML):
    os.remove("./" + HTML)

def find_between(s, first, last):
    try:
        start = s.index( first ) + len( first )
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""

# Using readlines()
file1 = open(MD, 'r')
oldLines = file1.readlines()

# Using readlines()
file2 = open(sys.argv[2], 'r')
headerLines = file2.readlines()

newLines = ""
count = 0
# Strips the newline character
for line in oldLines:
    count += 1
    line = line.strip()
    print("Line{}: {}".format(count, line.strip()))

    # Header level 1
    if line[:2] == "# ":
        line = line[2:]
        line = f"<h1>{line}"
        line = f"{line}</h1>"
    # Header level 2
    elif line[:3] == "## ":
        line = line[3:]
        line = f"<h2>{line}"
        line = f"{line}</h2>"
    # Header level 3
    elif line[:4] == "### ":
        line = line[4:]
        line = f"<h3>{line}"
        line = f"{line}</h3>"
    # Header level 4
    elif line[:5] == "#### ":
        line = line[5:]
        line = f"<h4>{line}"
        line = f"{line}</h4>"
    # Header level 5
    elif line[:6] == "##### ":
        line = line[6:]
        line = f"<h5>{line}"
        line = f"{line}</h5>"
    # Header level 6
    elif line[:7] == "###### ":
        line = line[7:]
        line = f"<h6>{line}"
        line = f"{line}</h6>"
    elif line[:2] == "> ":
        line = line[2:]
        line = f'<div class="blockquote"><p>{line}'
        line = f'{line}</p></div>'
    elif "***" in line:
        string = find_between(line, "***", "***")
        newString = f"<b><i>{string}</b></i>"
        line = line.replace("***" + string + "***", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    elif "**" in line:
        string = find_between(line, "**", "**")
        newString = f"<b>{string}</b>"
        line = line.replace("**" + string + "**", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    elif "*" in line:
        string = find_between(line, "*", "*")
        newString = f"<i>{string}</i>"
        line = line.replace("*" + string + "*", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    elif "---" in line:
        line = '<hr class="Seperator">'
    elif line[:1].isdecimal() and line[1:3] == ". ":
        line = f"<li>{line}"
        line = f"{line}</li>"
    elif line[:2] == "- ":
        line = f"<li>{line}"
        line = f"{line}</li>"
    elif "`" in line:
        string = find_between(line, '`', '`')
        newString = f'<b><i>{string}</b></i>'
        line = line.replace('`' + string + '`', newString)
        line = f'<div class="codeBlock">{line}'
        line = f'{line}</div>'
    elif line[:1] == "[" and "](" in line and line[-1] == ")":
        link = find_between(line, "(", ")")
        string = find_between(line, "[", "]")
        line = f"<a href='{link}'><p>{string}"
        line = f"{line}</p></a>"
    elif line[:2] == "![" and "](" in line and line[-1] == ")":
        link = find_between(line, "(", ")")
        string = find_between(line, "[", "]")
        line = f"<img src='{link}' alt='{string}'>"
        line = f"{line}</img>"
    else:
        line = f"<p>{line}"
        line = f"{line}</p>"
    
    if newLines == "":
        newLines = line
    else:
        newLines = f"{newLines}\n{line}"

file1.close()

file1 = open(HTML, 'w')
file1.writelines(headerLines)
file1.writelines(newLines)
file1.close()
import sys
import os
import re

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

    # Header levels
    if line[:1] == "#":
        n = len(str(re.findall("^#\S*", line)).replace("['", "").replace("']", ""))
        line = line[n+1:]
        line = f"<h{n}>{line}"
        line = f"{line}</h{n}>"
    # Blockquotes
    elif line[:2] == "> ":
        line = line[2:]
        line = f'<div class="blockquote"><p>{line}'
        line = f'{line}</p></div>'
    # italized and bold
    elif "***" in line:
        string = find_between(line, "***", "***")
        newString = f"<b><i>{string}</b></i>"
        line = line.replace("***" + string + "***", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    # bold
    elif "**" in line:
        string = find_between(line, "**", "**")
        newString = f"<b>{string}</b>"
        line = line.replace("**" + string + "**", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    #italized
    elif "*" in line:
        string = find_between(line, "*", "*")
        newString = f"<i>{string}</i>"
        line = line.replace("*" + string + "*", newString)
        line = f"<p>{line}"
        line = f"{line}</p>"
    # seperator
    elif "---" in line:
        line = '<hr class="Seperator">'
    # number list
    elif line[:1].isdecimal() and line[1:3] == ". ":
        line = f"<li>{line}"
        line = f"{line}</li>"
    # normal list
    elif line[:2] == "- ":
        line = f"<li>{line}"
        line = f"{line}</li>"
    # code block
    elif line[:3] == "```":
        string = find_between(line, '`', '`')
        line = line.replace('`', '')
        line = f'<div class="codeBlock">{line}'
        line = f'{line}</div>'
    # code section
    elif "`" in line:
        string = find_between(line, "`", "`")
        newString = f"<div class='codeSegment'>{string}</div>"
        line = line.replace('`' + string + '`', newString)
        line = f"<p class='codeSegmentWrapper'>{line}"
        line = f"{line}</p>"
    # link
    elif line[:1] == "[" and "](" in line and line[-1] == ")":
        link = find_between(line, "(", ")")
        string = find_between(line, "[", "]")
        line = f"<a href='{link}'>{string}"
        line = f"{line}</a>"
    # image
    elif line[:2] == "![" and "](" in line and line[-1] == ")":
        link = find_between(line, "(", ")")
        string = find_between(line, "[", "]")
        line = f"<img src='{link}' alt='{string}'>"
        line = f"{line}</img>"
    # empty line
    elif line == "":
        line = line
    # text
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
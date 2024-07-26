package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strings"
)

var (
	inputFlag  string
	headerFlag string
	outputFlag string
)

var rules [16][2]string

func main() {
	flag.StringVar(&inputFlag, "input", "false", "(Required) Input file")
	flag.StringVar(&outputFlag, "output", "false", "(Optional) Output file/folder, defaults to current")
	flag.StringVar(&headerFlag, "header", "false", "(Optional) Header File")

	flag.Usage = func() {
		flagSet := flag.CommandLine
		fmt.Printf("%s Usage:\n", "CoffParser")
		order := []string{"input", "output", "header"}
		for _, name := range order {
			flag := flagSet.Lookup(name)
			fmt.Printf("-%s\n", flag.Name)
			fmt.Printf("  %s\n", flag.Usage)
		}
	}

	flag.Parse()
	var strFalse = "false"

	if inputFlag == strFalse {
		fmt.Println("Input file is required.")
		flag.Usage()
		return
	}
	if _, err := os.Stat(inputFlag); err == nil {
		file, err := os.Open(inputFlag)
		if err != nil {
			// error handling
		}
		fileInfo, err := file.Stat()
		if err != nil {
			// error handling
		}

		//header rules
		rules[0][0] = "###### TEXT"
		rules[0][1] = "<h6>TEXT</h6>"
		rules[1][0] = "##### TEXT"
		rules[1][1] = "<h5>TEXT</h5>"
		rules[2][0] = "#### TEXT"
		rules[2][1] = "<h4>TEXT</h4>"
		rules[3][0] = "### TEXT"
		rules[3][1] = "<h3>TEXT</h3>"
		rules[4][0] = "## TEXT"
		rules[4][1] = "<h2>TEXT</h2>"
		rules[5][0] = "# TEXT"
		rules[5][1] = "<h1>TEXT</h1>"

		//bold, italics and paragragh rules
		rules[6][0] = "**TEXT**"
		rules[6][1] = "<b>TEXT</b>"
		rules[7][0] = "*TEXT*"
		rules[7][1] = "<i>TEXT</i>"
		rules[8][0] = "__TEXT__"
		rules[8][1] = "<b>TEXT</b>"
		rules[9][0] = "_TEXT_"
		rules[9][1] = "<i>TEXT</i>"
		rules[10][0] = "´TEXT´"
		rules[10][1] = "<b>TEXT</b>"

		//links
		rules[11][0] = "Link[TEXT](LINK)"
		rules[11][1] = "<a href='LINK' style='color:#2A5DB0;text-decoration: none;'>TEXT</a>"

		//highlights
		rules[12][0] = "==TEXT=="
		rules[12][1] = "<a style='background-color:grey;color:black;text-decoration: none;border-radius: 3px;padding:0 2px;'>$2</a>"

		//Lists
		rules[13][0] = " + TEXT"
		rules[13][1] = "<ul><li>$TEXT</li></ul>"
		rules[14][0] = " * TEXT"
		rules[14][1] = "<ul><li>TEXT</li></ul>"

		//Image
		//rules[15][0] = "![TEXT](LINK)"
		//rules[15][0] = "<img src='LINK' alt='TEXT'/>"

		if fileInfo.IsDir() {
			err := filepath.Walk(inputFlag,
				func(path string, info os.FileInfo, err error) error {
					if err != nil {
						return err
					}
					file, err := os.Open(path)
					if err != nil {
						return err
					}
					fileInfo, err := file.Stat()
					if err != nil {
						return err
					}
					if !fileInfo.IsDir() {
						if strings.HasSuffix(path, ".md") {
							ParseMD(path)
						}
					}
					return nil
				})
			if err != nil {
				log.Println(err)
			}
		}

	} else {
		fmt.Println("Input file is not valid.")
		flag.Usage()
		return
	}
}

func ParseMD(File string) {
	b, err := os.ReadFile(File) // just pass the file name
	if err != nil {
		fmt.Print(err)
	}
	str := string(b) // convert content to a 'string'

	fmt.Println(str) // print the content as a 'string'

	for _, a := range rules {
		rule1 := strings.ReplaceAll(a[0], "TEXT", ":")
		r1array := strings.Split(rule1, ":")
		rule2 := strings.ReplaceAll(a[1], "TEXT", ":")
		r2array := strings.Split(rule2, ":")
		for i, b := range r1array {
			if b != "" {
				str = strings.ReplaceAll(str, b, r2array[i])
				fmt.Println("A" + b + "A")
			}
		}
		//str = strings.ReplaceAll(str, a[0], a[1])
	}

	f, err := os.Create(strings.ReplaceAll(File, ".md", ".html"))

	f.WriteString(str)
}

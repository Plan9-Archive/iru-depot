// Traslate a word into all languages supported by Google Translate
package main

import (
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"http"
)

func readurl(url string) (os.Error, string) {
	var r io.Reader
	i := strings.Index(url, "://")
	if i == -1 {
		f, err := os.Open(url)
		defer f.Close()
		if err != nil {
			return err, ""
		}
		r = f
	} else if url[0:i] == "http" {
		resp, err := http.Get(url)
		defer resp.Body.Close()
		if err != nil {
			return err, ""
		}
		r = resp.Body
	} else {
		return os.NewError("invalid protocol"), ""
	}

	b, err := ioutil.ReadAll(r)
	if err != nil {
		return err, ""
	}
	return nil, string(b)
}

type ISO639 struct {
	Code  string
	Names []string
}

func line2iso(line string) ISO639 {
	if line == "" {
		return ISO639{"", nil}
	}

	s := strings.Split(line, "\t")
	names := strings.Split(s[1], ";")
	for i, name := range names {
		names[i] = strings.TrimSpace(name)
	}
	return ISO639{s[0], names}
}

func url2isotab(url string) (os.Error, []ISO639) {
	err, s := readurl(url)
	if err != nil {
		return err, []ISO639{ISO639{"", nil}}
	}
	stab := strings.Split(s, "\n")
	tab := make([]ISO639, len(stab))

	for i, s := range stab {
		if s == "" {
			continue
		}
		tab[i] = line2iso(s)
	}
	return nil, tab[:]
}

func code2names(isotab []ISO639, code string) []string {
	for _, iso := range isotab {
		if iso.Code == code {
			return iso.Names
		}
	}
	return nil
}

func name2code(isotab []ISO639, name string) string {
	for _, iso := range isotab {
		for _, s := range iso.Names {
			if s == name {
				return iso.Code
			}
		}
	}
	return ""
}

func main() {
	flag.Parse()
	if flag.NArg() == 0 {
		fmt.Fprintf(os.Stderr, "usage: wlt word\n")
		os.Exit(1)
	}

	err, tab := url2isotab("iso639-1.pt")
	if err != nil {
		fmt.Fprintf(os.Stderr, "error in ISO639-1 table, %s\n", err)
		os.Exit(1)
	}

	word := flag.Arg(0)
	notrans := "We are not yet able to translate"

	for _, iso := range tab {
		if iso.Code == "pt" || iso.Names == nil {
			continue
		}

		url := "http://translate.google.com/translate_a/t?client=t&text=" + word + "&sl=pt&tl=" + iso.Code
		err, s := readurl(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "error in translation using url %s, %s\n", url, err)
			os.Exit(1)
		}

		s = s[4:] // skip [[["
		if s[:len(notrans)] == notrans {
			continue
		}

		end := strings.Index(s, "\"")
		fmt.Printf("%s: %s\n", iso.Names[0], s[:end])
	}
}

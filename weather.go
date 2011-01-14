//
// Retrieve weather information for a given city from weather.com

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
	"http"
	"xml"
)

var partner int = 0000000000
var key string = "deadbeef0000"

type Head struct {
	XMLName xml.Name "head"
	Locale	string
	Form	string
	Ut		string
	Ud		string
	Us		string
	Up		string
	Ur		string
}

func (h *Head) String() string {
	return h.Ut
}

func readurl(url string) string {
	r, _, err := http.Get(url)
	var b []byte
	if err == nil {
		b, err = ioutil.ReadAll(r.Body)
		r.Body.Close()
	}
	if err != nil {
		fmt.Fprintf(os.Stderr, "reading url")
		os.Exit(1)
	}
	return string(b)
}

func readcity(name string) string {
	s := readurl("http://xoap.weather.com/search/search?where=" + http.URLEscape(name))
	
	locs := "<loc id=\""
	i := strings.Index(s, locs)
	if i == -1 {
		return ""
	}
	s = s[i+len(locs):]
	s = s[:strings.Index(s, "\"")]
	println(s)
	
	s = readurl("http://xoap.weather.com/weather/local/" + s + "?cc=*&dayf=5&par=" + string(partner) + "&key=" + key + "&unit=m")
	return s
}
	
func main() {
	s := readcity("rome,italy")
	
	hstart := strings.Index(s, "<head>")
	var h *Head
	if err := xml.Unmarshal(strings.NewReader(s[hstart:]), &h); err != nil {
		fmt.Fprintf(os.Stderr, "xml.Unmarshal: ", err)
		os.Exit(1)
	}
	fmt.Printf("Temperature in %s\n", h.Ut)
	fmt.Printf("%s", s[strings.Index(s, "<day d=\"4\""):])
}
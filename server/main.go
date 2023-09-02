package main

import (
	"html/template"
	"log"
	"net/http"
)

func rootHandler(w http.ResponseWriter, r *http.Request) {
	tmpl := template.Must(template.ParseFiles("html/index.html"))
	tmpl.Execute(w, r)
}

func main() {
	http.HandleFunc("/", rootHandler)
	log.Fatal(http.ListenAndServe(":8000", nil))
}

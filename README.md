# Skeddy
Skeddy is a work schedule creation tool. It assigns personnel to work shifts for the week to meet certain criteria:
* Schedules personnel for the preferred number of hours
* Schedules around time off
* Schedules a sufficient number of personnel for each role each day
* Takes into account personnel skill levels

Eventually the application will have a web interface and store a database of personnel.
For now, the application will have a CLI and take personnel and scheduling requirements as input every time.

# Architecture

* Backend core scheduling library written in C++
* Web server layer written in Go
* Frontend written with Go HTML templates with HTMX for interactivity
* Go/C++ interface generated with SWIG
* SQLite "database"
* Multi-user support/authentication TBD

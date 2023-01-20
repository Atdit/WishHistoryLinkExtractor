# WishHistoryLinkExtractor
Extracts the *unmodified* last wish history link from the data_2 cache file. Open it in a browser and append "log" after loading the site, to view the wish history

# Running
Run the program without any arguments to use the default `.local/share` path. Run it with a quoted string argument to point to a specific data_2 file.

# Compilation under Linux
Compilation doesn't require anything besides a C compiler and libc:
```bash
cc extractor.c -o aag-extract-wish-link
```
Run the script with
```bash
./aag-extract-wish-link
```
Install it under `/usr/local/bin` to call the program from anywhere with `aag-extract-wish-link`
```bash
sudo cp aag-extract-wish-link /usr/local/bin/aag-extract-wish-link
```

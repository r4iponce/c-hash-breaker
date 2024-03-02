# Hash breaker

A school project (code is not clean, I know) for break md5 with a client server program. Support wordlist or basic bruteforce based on array of char.

## Configuration :

### Client :

- -a: Server IP
- -p: Server port
- -l: Maximum password length
- -e: Ascii generation end (default 127)
- -b: Ascii generation begin (default 32)
- -f: Output error to file
- -h: Help

### Serveur :

- -i: Listen IP
- -p: Listen port
- -s: Hash to break
- -f: Output log to file
- -h: Help

# Build
## Install Dependencies
### Debian :
`sudo apt install build-essential libbsd-dev libssl-dev clang`

## Compile

- `make build` output binary in `build/{server,client}`
- `make build-dbg` output binary in `build/{server,client}` with debug symbol
- `make lint` lint with `clang-tidy`
- `make clean` delete `build/`
- `make` ou `make all` clean+lint+build 
- `make dev` clean+build-dbg

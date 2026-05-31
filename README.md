# YAKeyDB

In-memory key-value store.

## Methods

1. Set key and value

```
SET <key> <value>
```

2. Get value by key

```
GET <key>
```

3. Delete key

```
DELETE <key>
```

4. List all entries

```
LIST
```

5. Count entries

```
COUNT
```

6. Check if key exists

```
EXISTS <key>
```

7. Save and exit

```
EXIT
```

8. Save to file
```
SAVE
```

## CLI

```
./main <filename> [--log] [--port]
```

**`<filename>`** — store file. Loaded on startup (if present), overwritten on `EXIT`.

```
./main data.txt
```

If you start without a filename, `EXIT` asks for a path to save.

**`--log`** — append each command to a timestamped log file (e.g. `1780237006.log`) in the working directory.


**`--port`** — launch as server with specified host. Clients can then make requests with the same API as in REPL.

```
./main data.txt --log
```

Put the filename before flags (`./main data.txt --log`, not `./main --log data.txt`).

## Sanity test

To check for hard regressions use test script to launch simple scenario:
```
chmod +x test.sh
./test.sh
```
It should output
```
>> success
>> bar
>> success
>> False
0
```

## Using server 

To launch the in-memory db in server mode, use option `--port` with some numeric value.

Here is a simple example how client can establish connection:
```
curl telnet://127.0.0.1:8080
```
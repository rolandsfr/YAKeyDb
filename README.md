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
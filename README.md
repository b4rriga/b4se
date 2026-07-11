# b4se (a.k.a. b4)

Core libraries you need before everything else.

## value

`value.h` provides typed storage and conversion for textual values.

Each entry is identified by a string key and stores the original string representation together with every valid conversion derived from it. For example, the string `"1"` is automatically recognized as an integer and a floating-point value, while `true` is also available as a boolean. Conversions are performed only once when the value is stored.

The module recognizes the following primitive types:

- **Integer**: `10`, `0b1010`, `012`, `0xA` (all evaluate to decimal `10`)
- **Float**: `12.34`
- **String**: `"text"` or `raw text`
- **Boolean**: `true`, `false`, `on`, `off`, `yes`, `no`, `0`, `0.0`, `1`, `1.0`

Hexadecimal digits, integer base prefixes and boolean literals are all case-insensitive.

Quoted strings have their outer quotes removed during parsing. To preserve literal quotation marks, wrap the value in an additional pair of quotes.

The module also exposes validation helpers for primitive types:

- `b4val_is_bool()`
- `b4val_is_int()`
- `b4val_is_float()`

Values are inserted with:

```c
b4val_store(vals, "threads", "8");    // 8, 8.0, "8"
b4val_store(vals, "verbose", "true"); // true, "true"
b4val_store(vals, "pi", "3.14159");   // 3.14159, "3.14159"
b4val_store(vals, "name", "John");    // "John"
```

and retrieved through type-specific accessors:

```c
int64_t threads = b4val_get_int(vals, "threads", &err);
bool verbose = b4val_get_bool(vals, "verbose", &err);
double pi = b4val_get_float(vals, "pi", &err);
char *name = b4val_get_str(vals, "name", &err);
```

For example, the textual value "42" may be retrieved both as an integer and as a floating-point number:

```c
printf("integer = %10ld", b4val_get_int(vals, "number", &err));
printf(" [status: %s]\n", b4val_error(err));
printf("(float)integer = %10lf", b4val_get_float(vals, "number", &err));
printf(" [status: %s]\n", b4val_error(err));
printf("(bool)integer = %10s", b4val_boolstr(b4val_get_bool(vals, "number", &err));
printf(" [status: %s]\n", b4val_error(err));
```

producing:

```text
integer        =         42 [status: OPERATION OK]
(float)integer =  42.000000 [status: OPERATION OK]
(bool)integer  =      false [status: CONVERSION NOT POSSIBLE]
```

The original string representation is always preserved and can be retrieved regardless of any successful conversions.

Failed lookups and invalid conversions are reported through `B4ValueStatus`, allowing callers to distinguish between a missing key and an unsupported conversion. Human-readable descriptions are available through `b4val_error()`.

The internal representation is intentionally opaque. Users interact only with `B4Values`, while memory management is handled through `b4val_free()`.

## Others

**WORK IN PROGRESS**

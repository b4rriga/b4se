# b4se (a.k.a. b4)

Core libraries you need before everything else.

## value

`value.h` provides typed storage and conversion for textual values.

Each entry is identified by a string key and stores the original string representation together with every valid conversion derived from it. For example, the string `"1"` is automatically recognized as an integer and a floating-point value, while `true` is also available as a boolean. Conversions are performed only once when the value is stored.

The module exposes validation helpers for primitive types:

- `b4_value_is_bool()`
- `b4_value_is_int()`
- `b4_value_is_float()`

Values are inserted with:

```c
b4_value_store(vals, "threads", "8");    // 8, 8.0, "8"
b4_value_store(vals, "verbose", "true"); // true, "true"
b4_value_store(vals, "pi", "3.14159");   // 3.14159, "3.14159"
b4_value_store(vals, "name", "John");    // "John"
```

and retrieved through type-specific accessors:

```c
int64_t threads = b4_value_get_int(vals, "threads", &err);
bool verbose = b4_value_get_bool(vals, "verbose", &err);
double pi = b4_value_get_float(vals, "pi", &err);
char *name = b4_value_get_str(vals, "name", &err);
```

Each accessor reports failures through `B4ValueStatus`, allowing callers to distinguish between a missing key and an invalid conversion. Human-readable descriptions are available through `b4_value_error()`.

The internal representation is intentionally opaque. Users interact only with `B4Values`, while memory management is handled through `b4_value_free()`.

## Others

**WORK IN PROGRESS**

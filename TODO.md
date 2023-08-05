# TODO

* Add deleter argument to csp\_make\_\*\_for\_overwrite functions:

```c
csp_unique_ptr csp_make_unique_for_overwrite(size_t _size, csp_exception *_e);

csp_unique_ptr csp_make_unique_for_overwrite_d(size_t _size, csp_unique_ptr_D _d, csp_exception *_e);
```

* Consider renaming csp_make\_\*\_for\_overwrite functions to csp\_make\_\*.

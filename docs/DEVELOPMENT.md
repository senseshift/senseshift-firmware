## Useful Scripts

### Fix `clang-format`

```shell
find lib include firmware test examples -type f -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' ! -regex '^lib/frozen\(/.*\)' -exec clang-format-16 -style=file -i {} \;
```

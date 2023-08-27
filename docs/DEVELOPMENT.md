# Development notes

## Useful scripts/shortcuts

### Apply `clang-format`

```
find lib include firmware test examples -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format-16 -style=file -i {} \;
```
